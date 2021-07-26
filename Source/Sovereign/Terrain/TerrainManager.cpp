// Fill out your copyright notice in the Description page of Project Settings.

#include "./TerrainManager.h"

UTerrainManager::UTerrainManager() {

	// Initialize references to terrain static mesh actors
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName(TEXT("Terrain")), Actors);
	for (AActor* Actor : Actors) {
		AStaticMeshActor* TerrainActor = dynamic_cast<AStaticMeshActor*>(Actor);
		if (TerrainActor) TerrainActors.Add(TerrainActor);
	}

	// Initialize TerrainAccessoriesActorRef
	TArray<AActor*> TerrainAccessoriesActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName(TEXT("TerrainAccessories")), TerrainAccessoriesActors);

	if (!TerrainAccessoriesActors.Num())
		UE_LOG(LogTemp, Warning, TEXT("TerrainAccessories not found in Level"));

	if (TerrainAccessoriesActors.Num() > 1)
		UE_LOG(LogTemp, Warning, TEXT("More than one instance TerrainAccessories found"));

	if (TerrainAccessoriesActors.Num()) {
		ATerrainAccessories* TerrainAccessoriesRef = dynamic_cast<ATerrainAccessories*>(TerrainAccessoriesActors[0]);
		if (TerrainAccessoriesRef) {
			TerrainAccessories = TerrainAccessoriesRef;
			UE_LOG(LogTemp, Warning, TEXT("TerrainAccessories ref initialized"));
		}
	}

	TArray<AActor*> LandscapeActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName(TEXT("Landscape")), LandscapeActors);

	if (!LandscapeActors.Num())
		UE_LOG(LogTemp, Warning, TEXT("Landsacape not found in Level"));

	if (LandscapeActors.Num() > 1)
		UE_LOG(LogTemp, Warning, TEXT("More than one instance Landsacape found"));

	if (LandscapeActors.Num()) {
		ALandscape* LandscapeRef = dynamic_cast<ALandscape*>(LandscapeActors[0]);
		if (LandscapeRef) {
			Landscape = LandscapeRef;
			UE_LOG(LogTemp, Warning, TEXT("Landcape ref initialized"));
		}
	}

}

void UTerrainManager::SetSeason(uint8 Season) {

	if (!TerrainAccessories) {
		UE_LOG(LogTemp, Warning, TEXT("SetSeason pass with no effect, no TerrainAccessories found"), Season);
		return;
	}

	UMaterialInstanceDynamic* DynamicMaterialInst = TerrainAccessories->GetSeasonTerrainMaterialInst(Season);
	if (!DynamicMaterialInst) {
		UE_LOG(LogTemp, Warning, TEXT("TerrainMaterial for season:%d in TerrainAccessories not selected"), Season);
		return;
	}

	for (AStaticMeshActor* TerrainActor : TerrainActors) {
		UStaticMeshComponent* Component = TerrainActor->GetStaticMeshComponent();
		if (Component)
			Component->SetMaterial(0, DynamicMaterialInst);
	}

	DynamicMaterialInst = TerrainAccessories->GetSeasonLandscapeMaterialInst(Season);
	if (!DynamicMaterialInst) {
		UE_LOG(LogTemp, Warning, TEXT("Landscape for season:%d in TerrainAccessories not selected"), Season);
		return;
	}

	if (!Landscape) {
		UE_LOG(LogTemp, Warning, TEXT("SetSeason pass with no effect, no Landscape found"), Season);
		return;
	}

	for (ULandscapeComponent* LandscapeComponent : Landscape->LandscapeComponents) {
		LandscapeComponent->SetMaterial(0, DynamicMaterialInst);
	}
}


// Fill out your copyright notice in the Description page of Project Settings.

#include "./TerrainManager.h"

const int32 UTerrainManager::ONE_DAY = 1;
const uint8 UTerrainManager::NUM_SEASONS = 4;
const uint8 UTerrainManager::NUM_CHANGE_SEASON_INDICES = 8;
const int32 UTerrainManager::WINTER_REMAINING_DAYS = 60;
const int32 UTerrainManager::NUM_DAYS_IN_SEASON = 90;
const int32 UTerrainManager::HOURS_IN_DAY = 24;
const int32 UTerrainManager::MAX_WINTER_SEASON_PRECISION = 335;
const int32 UTerrainManager::MAX_WINTER_SEASON_CONTINUANCE = 345;

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

	// Fill ChangeSeasonSchedule with zero values
	for (uint8 i = 0; i < UTerrainManager::NUM_CHANGE_SEASON_INDICES; ++i)
		ChangeSeasonSchedule.Add(0);

}

void UTerrainManager::CreateChangeSeasonShedule(uint8 Precision, uint8 Continuance) {

	int32 NumDays = UTerrainManager::WINTER_REMAINING_DAYS;

	for (uint8 i = 0; i < UTerrainManager::NUM_SEASONS; ++i) {
		int32 SeasonPrecision = FMath::RoundToInt(Precision * FMath::RandRange(-1, 1));
		NumDays += SeasonPrecision;
		ChangeSeasonSchedule[(2 * i)] // Constant Season
			= FMath::Clamp(NumDays, 1, UTerrainManager::MAX_WINTER_SEASON_PRECISION);
		ChangeSeasonSchedule[(2 * i) + 1] // Off-season
			= FMath::Clamp(NumDays + Continuance, 1, UTerrainManager::MAX_WINTER_SEASON_CONTINUANCE);
		NumDays -= SeasonPrecision;
		NumDays += UTerrainManager::NUM_DAYS_IN_SEASON;
	}
}

int32 UTerrainManager::GetCurrentChangeSeasonIndex(int32 NumDays) {
	for (uint8 i = 0; i < UTerrainManager::NUM_CHANGE_SEASON_INDICES; ++i)
		if (NumDays < ChangeSeasonSchedule[i]) return i;
	return ChangeSeasonSchedule.Num() - UTerrainManager::ONE_DAY;
}

int32 UTerrainManager::GetChangeSeasonTotalHours(int32 NumDays) {
	for (uint8 i = 0; i < UTerrainManager::NUM_CHANGE_SEASON_INDICES; ++i)
		if (NumDays < ChangeSeasonSchedule[i])
			return (ChangeSeasonSchedule[i] - UTerrainManager::ONE_DAY) * UTerrainManager::HOURS_IN_DAY;

	return  (ChangeSeasonSchedule[ChangeSeasonSchedule.Num() - 1]
		- UTerrainManager::ONE_DAY)
		* UTerrainManager::HOURS_IN_DAY;
}

inline void UTerrainManager::ChangeSeasonEntryDateTime(const FDateTime& InDateTime) {
	CurrentChangeSeasonIndex = GetCurrentChangeSeasonIndex(InDateTime.GetDayOfYear());
}

void UTerrainManager::ChangeSeasonProgressEveryDay(const FDateTime& InDateTime) {

	const int32 DayOfYear = InDateTime.GetDayOfYear();

	int32 NewChangeSeasonIndex = GetCurrentChangeSeasonIndex(DayOfYear);
	if (CurrentChangeSeasonIndex != NewChangeSeasonIndex) {
		CurrentChangeSeasonIndex = NewChangeSeasonIndex;

		bChangeSeasonInProgress = (CurrentChangeSeasonIndex % 2) ? true : false;
		if (bChangeSeasonInProgress) {
			ChangeSeasonInitialHours = (InDateTime.GetDayOfYear()
				- UTerrainManager::ONE_DAY)
				* UTerrainManager::HOURS_IN_DAY;
			ChangeSeasonTotalHours = GetChangeSeasonTotalHours(DayOfYear);
		}

		TerrainMaterialInst = TerrainAccessories->GetTerrainSeason_MI(CurrentChangeSeasonIndex);

		if (TerrainMaterialInst) {
			for (AStaticMeshActor* TerrainActor : TerrainActors) {
				UStaticMeshComponent* Component = TerrainActor->GetStaticMeshComponent();
				if (Component)
					Component->SetMaterial(0, TerrainMaterialInst);
			}
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("BadCast TerrainMaterialInst to MaterialInterface"));
		}

	}
}

void UTerrainManager::ChangeSeasonProgressEveryHour(const FDateTime& InDateTime) {
	if (bChangeSeasonInProgress) {
		int32 CurrentHours
			= (InDateTime.GetDayOfYear() - UTerrainManager::ONE_DAY)
			* UTerrainManager::HOURS_IN_DAY
			+ InDateTime.GetHour();

		float ChangeSeasonProgress = UKismetMathLibrary::NormalizeToRange(
			CurrentHours,
			ChangeSeasonInitialHours,
			ChangeSeasonTotalHours);

		if (ChangeSeasonProgress > 1.0) {
			ChangeSeasonProgress = 1.0;
			bChangeSeasonInProgress = false;
		}

		auto SeasonMaterial = Cast<UMaterialInstanceDynamic>(TerrainMaterialInst);
		if (SeasonMaterial)
			SeasonMaterial->SetScalarParameterValue("BlendAlpha", ChangeSeasonProgress);
	}
}



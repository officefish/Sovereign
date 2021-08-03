// Fill out your copyright notice in the Description page of Project Settings.


#include "./TerrainAccessories.h"

// Sets default values
ATerrainAccessories::ATerrainAccessories()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// SetTag
	this->Tags.Add(FName("TerrainAccessories"));

}

UMaterialInterface* ATerrainAccessories::GetTerrainConstantSeason_MI(uint8 Season) {

	if (!TerrainDiffuseConstMaterialInst) {
		TerrainDiffuseConstMaterialInst = UMaterialInstanceDynamic::Create(TerrainDiffuseConstMaterial, GetWorld());
		TerrainDiffuseConstMaterialInst->SetTextureParameterValue("BaseNormal", BaseNormal);
		TerrainDiffuseConstMaterialInst->SetTextureParameterValue("AdditionalNormal", AdditionalNormal);
	}

	UMaterialInstanceDynamic* SeasonMaterial = TerrainDiffuseConstMaterialInst;
	UTexture* SeasonDiffuse = GetTerrainSeasonTexture(Season);
	SeasonMaterial->SetTextureParameterValue("Diffuse", SeasonDiffuse);
	return SeasonMaterial;
}

UMaterialInterface* ATerrainAccessories::GetTerrainBlendSeason_MI(uint8 FromSeason, uint8 ToSeason, float Progress) {

	if (!TerrainDiffuseBlendMaterialInst) {
		TerrainDiffuseBlendMaterialInst = UMaterialInstanceDynamic::Create(TerrainDiffuseBlendMaterial, GetWorld());
		TerrainDiffuseBlendMaterialInst->SetTextureParameterValue("BaseNormal", BaseNormal);
		TerrainDiffuseBlendMaterialInst->SetTextureParameterValue("AdditionalNormal", AdditionalNormal);
	}

	UMaterialInstanceDynamic* SeasonMaterial = TerrainDiffuseBlendMaterialInst;
	UTexture* TopSeasonDiffuse = GetTerrainSeasonTexture(FromSeason);
	UTexture* BottomSeasonDiffuse = GetTerrainSeasonTexture(ToSeason);

	if (!TopSeasonDiffuse)
		UE_LOG(LogTemp, Warning, TEXT("TopSeasonDiffuse is not available"));

	if (!BottomSeasonDiffuse)
		UE_LOG(LogTemp, Warning, TEXT("BottomSeasonDiffuse is not available"));

	if (SeasonMaterial) {
		SeasonMaterial->SetTextureParameterValue("DiffuseA", TopSeasonDiffuse);
		SeasonMaterial->SetTextureParameterValue("DiffuseB", BottomSeasonDiffuse);
		SeasonMaterial->SetScalarParameterValue("BlendAlpha", Progress);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("TerrainDiffuseBlendMaterialInst is not available"));
	}

	return SeasonMaterial;

}

UMaterialInterface* ATerrainAccessories::GetTerrainSeason_MI(uint8 SeasonIndex) {
	UMaterialInterface* MaterialInstance = nullptr;
	switch (SeasonIndex) {
	case 1:	MaterialInstance = GetTerrainBlendSeason_MI(0, 1, 0.0f); break;
	case 2:	MaterialInstance = GetTerrainConstantSeason_MI(1); break;
	case 3:	MaterialInstance = GetTerrainBlendSeason_MI(1, 2, 0.0f); break;
	case 4:	MaterialInstance = GetTerrainConstantSeason_MI(2); break;
	case 5:	MaterialInstance = GetTerrainBlendSeason_MI(2, 3, 0.0f); break;
	case 6:	MaterialInstance = GetTerrainConstantSeason_MI(3); break;
	case 7:	MaterialInstance = GetTerrainBlendSeason_MI(3, 0, 0.0f); break;
	default:
		MaterialInstance = GetTerrainConstantSeason_MI(0); break;
	}
	return MaterialInstance;
}

UTexture* ATerrainAccessories::GetTerrainSeasonTexture(uint8 Season) {
	UTexture* SeasonTexture = nullptr;
	switch (Season) {
	case 1: SeasonTexture = SpringTerrainTexture; break;
	case 2: SeasonTexture = SummerTerrainTexture; break;
	case 3: SeasonTexture = FallTerrainTexture; break;
	default: SeasonTexture = WinterTerrainTexture;
	}
	return SeasonTexture;
}


void ATerrainAccessories::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) {
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(ATerrainAccessories, TerrainDiffuseConstMaterialInst)) {
		// check if instance has texture parameters: Diffuse, BaseNormal, AdditionalNormal
	}

	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(ATerrainAccessories, TerrainDiffuseBlendMaterialInst)) {
		// check if instance has texture parameters: DiffuseA, DiffuseB, BaseNormal, AdditionalNormal
		// check if instance has scalar parameter: BlendAlpha
	}


}
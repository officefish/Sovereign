// Fill out your copyright notice in the Description page of Project Settings.


#include "./TerrainAccessories.h"

// Sets default values
ATerrainAccessories::ATerrainAccessories()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Initialize MaterialFactory
	MaterialFactory = CreateDefaultSubobject<UMaterialFactoryNewExtend>(TEXT("MaterialFactory"));

	// SetTag
	this->Tags.Add(FName("TerrainAccessories"));
}

UMaterialInstanceDynamic* ATerrainAccessories::GetSeasonTerrainMaterialInst(uint8 Season) {

	ValidateTerrainInstances();

	UMaterialInstanceDynamic* MaterialInstance = nullptr;
	if (Season >= 0 && Season <= 3)
		MaterialInstance = TerrainMaterialInstances[Season];
	return MaterialInstance;
}

UMaterialInstanceDynamic* ATerrainAccessories::GetSeasonLandscapeMaterialInst(uint8 Season) {

	ValidateLandscapeInstances();

	UMaterialInstanceDynamic* MaterialInstance = nullptr;
	if (Season >= 0 && Season <= 3)
		MaterialInstance = LandscapeMaterialInstances[Season];
	return MaterialInstance;
}

void ATerrainAccessories::ValidateLandscapeInstances() {
	if (!LandscapeMaterialInstances.Num()) {
		LandscapeMaterialInstances.Add(UMaterialInstanceDynamic::Create(LandscapeWinterMaterial, this));
		LandscapeMaterialInstances.Add(UMaterialInstanceDynamic::Create(LandscapeSpringMaterial, this));
		LandscapeMaterialInstances.Add(UMaterialInstanceDynamic::Create(LandscapeSummerMaterial, this));
		LandscapeMaterialInstances.Add(UMaterialInstanceDynamic::Create(LandscapeFallMaterial, this));
	}
}

void ATerrainAccessories::ValidateTerrainInstances() {
	if (!TerrainMaterialInstances.Num()) {
		TerrainMaterialInstances.Add(UMaterialInstanceDynamic::Create(TerrainWinterMaterial, this));
		TerrainMaterialInstances.Add(UMaterialInstanceDynamic::Create(TerrainSpringMaterial, this));
		TerrainMaterialInstances.Add(UMaterialInstanceDynamic::Create(TerrainSummerMaterial, this));
		TerrainMaterialInstances.Add(UMaterialInstanceDynamic::Create(TerrainFallMaterial, this));
	}
}

void ATerrainAccessories::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) {
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(ATerrainAccessories, TerrainWinterMaterial)) {
		generate_TerrainFallToWinterMaterial();
		generate_TerrainWinterToSpringMaterial();
	}

	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(ATerrainAccessories, TerrainSpringMaterial)) {
		generate_TerrainWinterToSpringMaterial();
		generate_TerrainSpringToSummerMaterial();
	}

	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(ATerrainAccessories, TerrainSummerMaterial)) {
		generate_TerrainSpringToSummerMaterial();
		generate_TerrainSummerToFallMaterial();
	}

	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(ATerrainAccessories, TerrainFallMaterial)) {
		generate_TerrainSummerToFallMaterial();
		generate_TerrainFallToWinterMaterial();
	}

}

void ATerrainAccessories::generate_TerrainFallToWinterMaterial() {

	FBlendMaterialInputs Inputs;
	Inputs.MaterialBaseName = "M_FallToWinter_Blend";
	Inputs.Path = "/Game/Generic/Materials/Terrain/";
	Inputs.TopMaterial = TerrainFallMaterial;
	Inputs.BottomMaterial = TerrainWinterMaterial;

	UMaterial* Material = MaterialFactory->FactoryCreateNewBlend(Inputs);

}
void ATerrainAccessories::generate_TerrainWinterToSpringMaterial() {

	FBlendMaterialInputs Inputs;
	Inputs.MaterialBaseName = "M_WinterToSpring_Blend";
	Inputs.Path = "/Game/Generic/Materials/Terrain/";
	Inputs.TopMaterial = TerrainWinterMaterial;
	Inputs.BottomMaterial = TerrainSpringMaterial;

	UMaterial* Material = MaterialFactory->FactoryCreateNewBlend(Inputs);

}
void ATerrainAccessories::generate_TerrainSpringToSummerMaterial() {

	FBlendMaterialInputs Inputs;
	Inputs.MaterialBaseName = "M_SpringToSummer_Blend";
	Inputs.Path = "/Game/Generic/Materials/Terrain/";
	Inputs.TopMaterial = TerrainSpringMaterial;
	Inputs.BottomMaterial = TerrainSummerMaterial;

	UMaterial* Material = MaterialFactory->FactoryCreateNewBlend(Inputs);
}
void ATerrainAccessories::generate_TerrainSummerToFallMaterial() {

	FBlendMaterialInputs Inputs;
	Inputs.MaterialBaseName = "M_SummerToFall_Blend";
	Inputs.Path = "/Game/Generic/Materials/Terrain/";
	Inputs.TopMaterial = TerrainFallMaterial;
	Inputs.BottomMaterial = TerrainWinterMaterial;

	UMaterial* Material = MaterialFactory->FactoryCreateNewBlend(Inputs);
}




// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Materials/Material.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "./../Generic/MaterialFactoryNewExtend.h"
#include "TerrainAccessories.generated.h"

UCLASS()
class SOVEREIGN_API ATerrainAccessories : public AActor
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Generic")
	UMaterialFactoryNewExtend* MaterialFactory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerrainMaterial")
	UMaterial* TerrainWinterMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerrainMaterial")
	UMaterial* TerrainSpringMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerrainMaterial")
	UMaterial* TerrainSummerMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerrainMaterial")
	UMaterial* TerrainFallMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LandscapeMaterial")
	UMaterial* LandscapeWinterMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LandscapeMaterial")
	UMaterial* LandscapeSpringMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LandscapeMaterial")
	UMaterial* LandscapeSummerMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LandscapeMaterial")
	UMaterial* LandscapeFallMaterial;

	UPROPERTY()
	TArray<UMaterialInstanceDynamic*> TerrainMaterialInstances;

	UPROPERTY()
	TArray<UMaterialInstanceDynamic*> LandscapeMaterialInstances;

public:
	// Sets default values for this actor's properties
	ATerrainAccessories();

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	UFUNCTION(BlueprintCallable, Category = "Material")
	UMaterialInstanceDynamic* GetSeasonTerrainMaterialInst(uint8 Season);

	UFUNCTION(BlueprintCallable, Category = "Material")
	UMaterialInstanceDynamic* GetSeasonLandscapeMaterialInst(uint8 Season);

	UFUNCTION()
	void ValidateTerrainInstances();

	UFUNCTION()
	void ValidateLandscapeInstances();

private:
	void generate_TerrainFallToWinterMaterial();
	void generate_TerrainWinterToSpringMaterial();
	void generate_TerrainSpringToSummerMaterial();
	void generate_TerrainSummerToFallMaterial();
};



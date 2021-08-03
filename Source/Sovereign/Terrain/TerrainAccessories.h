// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Materials/Material.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "MaterialEditingLibrary.h"
#include "./../Generic/MaterialFactoryNewExtend.h"
#include "TerrainAccessories.generated.h"

UCLASS()
class SOVEREIGN_API ATerrainAccessories : public AActor
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerrainMaterial")
	UMaterial* TerrainDiffuseConstMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerrainMaterial")
	UMaterial* TerrainDiffuseBlendMaterial;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "TerrainMaterial")
	UMaterialInstanceDynamic* TerrainDiffuseConstMaterialInst;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "TerrainMaterial")
	UMaterialInstanceDynamic* TerrainDiffuseBlendMaterialInst;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerrainMaterial")
	UTexture* WinterTerrainTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerrainMaterial")
	UTexture* SpringTerrainTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerrainMaterial")
	UTexture* SummerTerrainTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerrainMaterial")
	UTexture* FallTerrainTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerrainMaterial")
	UTexture* BaseNormal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TerrainMaterial")
	UTexture* AdditionalNormal;


public:
	ATerrainAccessories();

	UMaterialInterface* GetTerrainSeason_MI(uint8 Season);
	UMaterialInterface* GetTerrainConstantSeason_MI(uint8 Season);
	UMaterialInterface* GetTerrainBlendSeason_MI(uint8 FromSeason, uint8 ToSeason, float Progress);

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

private:

	UTexture* GetTerrainSeasonTexture(uint8 Season);
};



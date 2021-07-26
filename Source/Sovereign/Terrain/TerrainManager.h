// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/StaticMeshActor.h"
#include "Kismet/GameplayStatics.h"
#include "TerrainAccessories.h"
#include "Landscape.h"
#include "TerrainManager.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class SOVEREIGN_API UTerrainManager : public UObject
{
	GENERATED_BODY()

public:

	UTerrainManager();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
	ALandscape* Landscape;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
	TArray<AStaticMeshActor*> TerrainActors;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
	ATerrainAccessories* TerrainAccessories;

public:

	UFUNCTION(BlueprintCallable, Category = "DateTime")
	void SetSeason(uint8 Season);

};

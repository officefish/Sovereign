// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/StaticMeshActor.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "TerrainAccessories.h"
#include "Landscape.h"
#include "Templates/Casts.h"
#include "TerrainManager.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class SOVEREIGN_API UTerrainManager : public UObject
{
	GENERATED_BODY()

public:

	static const int32 ONE_DAY;
	static const uint8 NUM_SEASONS;
	static const uint8 NUM_CHANGE_SEASON_INDICES;
	static const int32 WINTER_REMAINING_DAYS;
	static const int32 NUM_DAYS_IN_SEASON;
	static const int32 HOURS_IN_DAY;
	static const int32 MAX_WINTER_SEASON_PRECISION;
	static const int32 MAX_WINTER_SEASON_CONTINUANCE;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
	ALandscape* Landscape;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
	TArray<AStaticMeshActor*> TerrainActors;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components")
	ATerrainAccessories* TerrainAccessories;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "TerrainMaterial")
	UMaterialInterface* TerrainMaterialInst;

	UPROPERTY()
	TArray<int32> ChangeSeasonSchedule;

	UPROPERTY()
	int32 CurrentChangeSeasonIndex;

	UPROPERTY()
	int32 ChangeSeasonInitialHours;

	UPROPERTY()
	int32 ChangeSeasonTotalHours;

	UPROPERTY()
	bool bChangeSeasonInProgress = false;

public:

	UTerrainManager();

	UFUNCTION(BlueprintCallable, Category = "ChangeSeason")
	void CreateChangeSeasonShedule(uint8 Precision, uint8 Continuance);

	UFUNCTION(BlueprintCallable, Category = "ChangeSeason")
	void ChangeSeasonEntryDateTime(const FDateTime& CurrentDateTime);

	UFUNCTION(BlueprintCallable, Category = "ChangeSeason")
	void ChangeSeasonProgressEveryHour(const FDateTime& CurrentDateTime);

	UFUNCTION(BlueprintCallable, Category = "ChangeSeason")
	void ChangeSeasonProgressEveryDay(const FDateTime& CurrentDateTime);

	UFUNCTION(BlueprintCallable, Category = "ChangeSeason")
	int32 GetCurrentChangeSeasonIndex(int32 NumDays);

	UFUNCTION(BlueprintCallable, Category = "ChangeSeason")
	int32 GetChangeSeasonTotalHours(int32 NumDays);

};

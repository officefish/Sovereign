// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Time/TimeManager.h"
#include "Environment/EnvironmentManager.h"
#include "PostProcess/PostProcessManager.h"
#include "Terrain/TerrainManager.h"
#include "RTSGameStateBase.generated.h"

/**
 *
 */
UCLASS()
class SOVEREIGN_API ARTSGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

	UPROPERTY()
	UTimeManager* TimeMamager;

	UPROPERTY()
	UEnvironmentManager* EnvironmentManager;

	UPROPERTY()
	UPostProcessManager* PostProcessManager;

	UPROPERTY()
	UTerrainManager* TerrainManager;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DateTime")
	FDateTime StoryDateTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DateTime")
	FDateTime PreviousDateTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DateTime")
	FDateTime CurrentDateTime;

public:
	ARTSGameStateBase();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "DateTime")
	void SetDateTimeSpeedMode(int32 DateTimeSpeedMode);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	double GetDateTimeAcceleration(int32 DateTimeSpeedMode);
	void CheckDateTimeCycles();
	void CheckNextDayCycle();
	void CheckNextSeasonCycle();

	uint8 seasonIndex = 0;

};

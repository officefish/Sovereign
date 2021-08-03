// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSGameStateBase.h"

ARTSGameStateBase::ARTSGameStateBase() {
	PrimaryActorTick.bCanEverTick = true;

	TimeManager = CreateDefaultSubobject<UTimeManager>(TEXT("TimeManager"));

	EnvironmentManager = CreateDefaultSubobject<UEnvironmentManager>(TEXT("EnvironmentManager"));

	PostProcessManager = CreateDefaultSubobject<UPostProcessManager>(TEXT("PostProcessManager"));
	PostProcessManager->GlobalDynamicIlluminationSetup();

	TerrainManager = CreateDefaultSubobject<UTerrainManager>(TEXT("TerrainManager"));
}

void ARTSGameStateBase::BeginPlay() {
	Super::BeginPlay();
	StoryDateTime = FDateTime(1400, 1, 7, 6, 0, 0);

	TimeManager->InitializeDateTime(StoryDateTime);

	const uint8 ChangeSeasonPrecision = 7;
	const uint8 ChangeSeasonContinuance = 14;

	TerrainManager->CreateChangeSeasonShedule(ChangeSeasonPrecision, ChangeSeasonContinuance);
	TerrainManager->ChangeSeasonEntryDateTime(StoryDateTime);

	PreviousDateTime = StoryDateTime;

	EnvironmentManager->CurrentDateTime = StoryDateTime;
	EnvironmentManager->UpdateSun();
}

double ARTSGameStateBase::GetDateTimeAcceleration(int32 DateTimeSpeedMode) {
	double Acceleration;
	switch (DateTimeSpeedMode) {
	case 0:
		Acceleration = -1.0;
		break;
	case 1:
		Acceleration = 0;
		break;
	case 2:
		Acceleration = 2.0;
		break;
	case 3:
		Acceleration = 10.0;
		break;
	case 4:
		Acceleration = 20.0;
		break;
	default:
		Acceleration = 100.0;
	}
	return Acceleration;
}

void ARTSGameStateBase::SetDateTimeSpeedMode(int32 DateTimeSpeedMode) {
	double Accelertion = GetDateTimeAcceleration(DateTimeSpeedMode);
	TimeManager->SetAcceleration(Accelertion);
}

void ARTSGameStateBase::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	CurrentDateTime = TimeManager->GetCurrentDateTime();

	EnvironmentManager->CurrentDateTime = CurrentDateTime;
	EnvironmentManager->UpdateSun();

	CheckDateTimeCycles();

	PreviousDateTime = CurrentDateTime;
}

inline void ARTSGameStateBase::CheckDateTimeCycles() {
	CheckNextDayCycle();
	CheckNextHourCycle();
}

inline void ARTSGameStateBase::CheckNextDayCycle() {
	int32 CurrentDay = CurrentDateTime.GetDay();
	int32 PreviousDay = PreviousDateTime.GetDay();
	if (CurrentDay != PreviousDay) {
		TerrainManager->ChangeSeasonProgressEveryDay(CurrentDateTime);
	}
}


inline void ARTSGameStateBase::CheckNextHourCycle() {
	int32 CurrentHour = CurrentDateTime.GetHour();
	int32 PreviousHour = PreviousDateTime.GetHour();
	if (CurrentHour != PreviousHour) {
		TerrainManager->ChangeSeasonProgressEveryHour(CurrentDateTime);
	}
}
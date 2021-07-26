// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSGameStateBase.h"

ARTSGameStateBase::ARTSGameStateBase() {
	PrimaryActorTick.bCanEverTick = true;

	TimeMamager = CreateDefaultSubobject<UTimeManager>(TEXT("TimeManager"));

	EnvironmentManager = CreateDefaultSubobject<UEnvironmentManager>(TEXT("EnvironmentManager"));

	PostProcessManager = CreateDefaultSubobject<UPostProcessManager>(TEXT("PostProcessManager"));
	PostProcessManager->GlobalDynamicIlluminationSetup();

	TerrainManager = CreateDefaultSubobject<UTerrainManager>(TEXT("TerrainManager"));
}

void ARTSGameStateBase::BeginPlay() {
	Super::BeginPlay();
	StoryDateTime = FDateTime(1400, 1, 7, 6, 0, 0);

	TimeMamager->InitializeDateTime(StoryDateTime);

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
	TimeMamager->SetAcceleration(Accelertion);
}

void ARTSGameStateBase::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	CurrentDateTime = TimeMamager->GetCurrentDateTime();

	EnvironmentManager->CurrentDateTime = CurrentDateTime;
	EnvironmentManager->UpdateSun();

	CheckDateTimeCycles();

	PreviousDateTime = CurrentDateTime;
	//if (EnvironmentManager)
	//	EnvironmentManager->SetCurrentDateTime(CurrentDateTime);

	//FString DateTimeString = CurrentDateTime.ToString();
	//if (GEngine)
	//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, DateTimeString);

	//UE_LOG(LogTemp, Warning, TEXT("%s"), *DateTimeString);
}

inline void ARTSGameStateBase::CheckDateTimeCycles() {
	CheckNextDayCycle();
	CheckNextSeasonCycle();
}

inline void ARTSGameStateBase::CheckNextDayCycle() {
	int32 CurrentDay = CurrentDateTime.GetDay();
	int32 PreviousDay = PreviousDateTime.GetDay();
	if (CurrentDay != PreviousDay) {
		// If complex logic here, should use native event instead
		//UE_LOG(LogTemp, Warning, TEXT("CurrentDay: %d, PreviousDay: %d"), CurrentDay, PreviousDay);
		++seasonIndex;
		seasonIndex = (seasonIndex <= 3) ? seasonIndex : 0;
		TerrainManager->SetSeason(seasonIndex);
	}
}

inline void ARTSGameStateBase::CheckNextSeasonCycle() {
	uint8 CurrentSeason = UTimeFunctionLibrary::GetSeason(CurrentDateTime.GetMonth());
	uint8 PreviousSeason = UTimeFunctionLibrary::GetSeason(PreviousDateTime.GetMonth());
	if (CurrentSeason != PreviousSeason) {
		// If complex logic here, should use native event instead
		//UE_LOG(LogTemp, Warning, TEXT("CurrentSeason: %d, PreviousSeason: %d"), CurrentSeason, PreviousSeason);
	}
}
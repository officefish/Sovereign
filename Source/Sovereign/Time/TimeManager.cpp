// Fill out your copyright notice in the Description page of Project Settings.
#include "./TimeManager.h"

void UTimeManager::InitializeDateTime(const FDateTime& StoryDateTime) {
	DeltaSeconds = (FDateTime::Now() - StoryDateTime).GetTotalSeconds();
}

FDateTime UTimeManager::GetDeltaDateTime() {
	return FDateTime{ FTimespan::FromSeconds(DeltaSeconds).GetTicks() };
}

FDateTime UTimeManager::GetCurrentDateTime() {
	double CurrentSeconds = GetCurrentTotalSeconds();
	return FDateTime{ FTimespan::FromSeconds(CurrentSeconds).GetTicks() };
}

double UTimeManager::GetCurrentTotalSeconds() {
	if (bPauseFlag) return PauseSeconds;
	ExtraSeconds += Acceleration;
	double NowSeconds = FTimespan{ FDateTime::Now().GetTicks() }.GetTotalSeconds();
	return (NowSeconds + ExtraSeconds - DeltaSeconds);
}

void UTimeManager::SetAcceleration(double inSeconds) {
	if (inSeconds < 0) {
		PauseSeconds = GetCurrentTotalSeconds();
		bPauseFlag = true;
	}
	else if (bPauseFlag) {
		bPauseFlag = false;
		ExtraSeconds += (GetCurrentTotalSeconds() - PauseSeconds);
		PauseSeconds = 0;
	}
	Acceleration = inSeconds;
}

TEnumAsByte<enum EDateTimeSeason> UTimeFunctionLibrary::GetSeason(int32 Month) {
	TEnumAsByte<enum EDateTimeSeason> Season;
	switch (Month) {
	case 1:
	case 2:
	case 12:
		Season = EDateTimeSeason::Winter;
		break;
	case 3:
	case 4:
	case 5:
		Season = EDateTimeSeason::Spring;
		break;
	case 6:
	case 7:
	case 8:
		Season = EDateTimeSeason::Summer;
		break;
	default:
		Season = EDateTimeSeason::Fall;
	}
	return Season;
}
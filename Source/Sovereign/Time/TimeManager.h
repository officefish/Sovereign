// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Misc/DateTime.h"
#include "TimeManager.generated.h"

UENUM(BlueprintType)
enum EDateTimeSeason
{
	Winter,
	Spring,
	Summer,
	Fall
};

/**
 *
 */
UCLASS(Blueprintable)
class SOVEREIGN_API UTimeManager : public UObject
{
	GENERATED_BODY()

	UPROPERTY()
	double DeltaSeconds;

	UPROPERTY()
	double ExtraSeconds;

	UPROPERTY()
	double PauseSeconds;

	UPROPERTY()
	double Acceleration;

	UPROPERTY()
	bool bPauseFlag = false;

public:

	UFUNCTION(BlueprintCallable, Category = "DateTime")
	void InitializeDateTime(const FDateTime& StoryDateTime);

	UFUNCTION(BlueprintPure, Category = "DateTime")
	FDateTime GetDeltaDateTime();

	UFUNCTION(BlueprintPure, Category = "DateTime")
	FDateTime GetCurrentDateTime();

	double GetCurrentTotalSeconds();
	void SetAcceleration(double inSeconds);
};

UCLASS()
class SOVEREIGN_API UTimeFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	*Add Rule to Genesis alphabet
	*
	*@param		Alphabet		Genesis Alphabet Object Reference
	*@param		Rule			Genesis Rule Object Reference
	*@return	void
	*/
	UFUNCTION(BlueprintCallable, Category = "Season")
	static TEnumAsByte<enum EDateTimeSeason> GetSeason(int32 Month);


};

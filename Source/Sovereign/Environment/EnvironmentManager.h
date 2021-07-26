// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "./AccurateSunSky.h"
#include "Misc/DateTime.h"
#include "Kismet/GameplayStatics.h"
#include "EnvironmentManager.generated.h"

UCLASS(Blueprintable)
class SOVEREIGN_API UEnvironmentManager : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	AAccurateSunSky* SunSky;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DateTime")
	FDateTime CurrentDateTime;


public:
	// Sets default values for this component's properties
	UEnvironmentManager();
	void UpdateSun();
};

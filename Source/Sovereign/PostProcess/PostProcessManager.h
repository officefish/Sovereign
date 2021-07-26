// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/PostProcessVolume.h"
#include "Kismet/GameplayStatics.h"
#include "PostProcessManager.generated.h"

/**
 *
 */
UCLASS()
class SOVEREIGN_API UPostProcessManager : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY()
	APostProcessVolume* PP_Volume;

public:
	UPostProcessManager();
	void GlobalDynamicIlluminationSetup();
};

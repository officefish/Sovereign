// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include <cmath>

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SkyLightComponent.h"
#include "Components/SkyAtmosphereComponent.h"
#include "Components/ChildActorComponent.h"
#include "Components/DirectionalLightComponent.h"
#include "GenericPlatform/GenericPlatformMath.h"
#include "Misc/DateTime.h"
#include "SunPosition.h"
#include "AccurateSunSky.generated.h"

UCLASS()
class SOVEREIGN_API AAccurateSunSky : public AActor
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USceneComponent* Scene;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UDirectionalLightComponent* SunLight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USkyAtmosphereComponent* SkyAtmosphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USkyLightComponent* SkyLight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DateTime")
	FDateTime CurrentDateTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Location")
	float Latitude = 45.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Location")
	float Longitude = -73.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Location")
	float TimeZone = -4.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Location")
	float NorthOffset = 0.0;

public:
	// Sets default values for this actor's properties
	AAccurateSunSky();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void UpdateSun();

};

// Fill out your copyright notice in the Description page of Project Settings.

#include "./AccurateSunSky.h"

// Sets default values
AAccurateSunSky::AAccurateSunSky()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = Scene;

	SunLight = CreateDefaultSubobject<UDirectionalLightComponent>(TEXT("SunLight"));
	SunLight->SetupAttachment(Scene);

	SkyLight = CreateDefaultSubobject<USkyLightComponent>(TEXT("SkyLight"));
	SkyLight->SetupAttachment(Scene);

	SkyAtmosphere = CreateDefaultSubobject<USkyAtmosphereComponent>(TEXT("SkyAthosphere"));
	SkyAtmosphere->SetupAttachment(Scene);

	// SetupComponents
	SunLight->SetAtmosphereSunLight(true);

	// Global Dynamic light issue
	SunLight->SetIntensity(3.14);
	SunLight->LightmassSettings.IndirectLightingSaturation = 1;

	// AccurateSunSky issue
	//SunLight->SetIntensity(103.708992);
	SunLight->LightSourceAngle = 0.5;
	SunLight->DynamicShadowCascades = 5;
	SunLight->SetCascadeDistributionExponent(1.4);

	// AccurateSunSky issue
	SkyLight->bRealTimeCapture = true;
	SkyLight->SamplesPerPixel = 1;

	// Global Dynamic light issue
	SkyLight->OcclusionCombineMode = EOcclusionCombineMode::OCM_Multiply;

	// SetTag
	this->Tags.Add(FName("SunSky"));
}

// Called when the game starts or when spawned
void AAccurateSunSky::BeginPlay() {
	Super::BeginPlay();
}

// Called every frame
void AAccurateSunSky::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void AAccurateSunSky::UpdateSun() {
	if (!SunLight) {
		UE_LOG(LogTemp, Warning, TEXT("SunLight component not awailable in AccurateSunSky"));
		return;
	}

	FSunPositionData SunPositionData {};
	USunPositionFunctionLibrary::GetSunPosition
	(
		Latitude,
		Longitude,
		TimeZone,
		true,
		CurrentDateTime.GetYear(),
		CurrentDateTime.GetMonth(),
		CurrentDateTime.GetDay(),
		CurrentDateTime.GetHour(),
		CurrentDateTime.GetMinute(),
		CurrentDateTime.GetSecond(),
		SunPositionData
	);

	FRotator WorldRotation;
	WorldRotation.Roll = 0.0;
	WorldRotation.Pitch = SunPositionData.CorrectedElevation;
	WorldRotation.Yaw = SunPositionData.Azimuth + NorthOffset;
	//WorldRotation.Normalize();
	//FRotator RinterpRotation = FMath::RInterpTo(SunLight->GetComponentRotation(), WorldRotation, 0.5f, 0.05f);
	//WorldRotation = FMath::Lerp(SunLight->GetComponentTransform().GetRotation().Rotator(), WorldRotation, 0.05f);
	SunLight->SetWorldRotation(WorldRotation);

}


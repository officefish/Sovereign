// Fill out your copyright notice in the Description page of Project Settings.


#include "./PostProcessManager.h"

UPostProcessManager::UPostProcessManager() {
	TArray<AActor*> PPVActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName(TEXT("PP_V")), PPVActors);
	if (PPVActors.Num()) {
		APostProcessVolume* PP_VolumeReference = dynamic_cast<APostProcessVolume*>(PPVActors[0]);
		
		if (PP_VolumeReference) {
			PP_Volume = PP_VolumeReference;
			UE_LOG(LogTemp, Warning, TEXT("PP_Volume ref initialised"));
		}
		else
			UE_LOG(LogTemp, Warning, TEXT("dynamic cast PPVActors[0] to APostProcessVolume failed"));
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("No Actors with tag:'PP_V' found"));
}

void UPostProcessManager::GlobalDynamicIlluminationSetup() {

	if (!PP_Volume) {
		UE_LOG(LogTemp, Warning, TEXT("GlobalDynamicIlluminationSetup process with no effect. PP_Volume not allowed."));
		return;
	}

	PP_Volume->bUnbound = true;

	// PostProcessVolume / Rendering Features

	// Light Propagation Volume(set any comfortable value, by default 1) 0 recommended
	PP_Volume->Settings.LPVIntensity = 0.0;

	// Light Injection Bias(same, by default 0, 64) 2 recommended
	PP_Volume->Settings.LPVVplInjectionBias = 2.0;

	// Indirect Global Illumination(same, by default 0, 64) 0 recommended
	PP_Volume->Settings.IndirectLightingIntensity = 0.0;

	// Deactivate AutoExposure
	PP_Volume->Settings.AutoExposureMinBrightness = 1.0;
	PP_Volume->Settings.AutoExposureMaxBrightness = 1.0;

	// Deactivate SSAO and SSR
	PP_Volume->Settings.AmbientOcclusionIntensity = 0.0;
	PP_Volume->Settings.ScreenSpaceReflectionIntensity = 0.0;

	// Bloom off / vignetting off
	PP_Volume->Settings.BloomIntensity = 0.0;
	PP_Volume->Settings.VignetteIntensity = 0.0;
}
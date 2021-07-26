// Fill out your copyright notice in the Description page of Project Settings.

#include "./EnvironmentManager.h"

// Sets default values for this component's properties
UEnvironmentManager::UEnvironmentManager() {

	// Search AccurateSunSky Actor with tag "SunSky" and save reference if is found
	TArray<AActor*> SunSkyActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName(TEXT("SunSky")), SunSkyActors);
	if (SunSkyActors.Num()) {
		AAccurateSunSky* SunSkyReference = dynamic_cast<AAccurateSunSky*>(SunSkyActors[0]);
		if (SunSkyReference) {
			SunSky = SunSkyReference;
			UE_LOG(LogTemp, Warning, TEXT("SunRef ref initialised"));
		} else
			UE_LOG(LogTemp, Warning, TEXT("dynamic cast SunSkyActors[0] to AccurateSunSky failed"));
	} else
		UE_LOG(LogTemp, Warning, TEXT("No Actors with tag:'SunSky' found"));
}


// Called every frame
void UEnvironmentManager::UpdateSun() {
	if (SunSky) {
		SunSky->CurrentDateTime = this->CurrentDateTime;
		SunSky->UpdateSun();
	}
}



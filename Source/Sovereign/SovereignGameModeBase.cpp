// Copyright Epic Games, Inc. All Rights Reserved.


#include "SovereignGameModeBase.h"

void ASovereignGameModeBase::BeginPlay() {
	Super::BeginPlay();

	if (MainHUDClass != nullptr) {
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), MainHUDClass);
		if (CurrentWidget != nullptr) {
			CurrentWidget->AddToViewport();
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("ASovereignGameModeBase could not create MainHUD widget"));
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("MainHUDClass not available in ASovereignGameModeBase"));
	}
}
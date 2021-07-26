// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "SovereignGameModeBase.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class SOVEREIGN_API ASovereignGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	// ASovereignGameModeBase()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "DateTime", Meta = (BlueprintProtected = true))
	TSubclassOf<UUserWidget> MainHUDClass;

	UPROPERTY()
	UUserWidget* CurrentWidget;
};

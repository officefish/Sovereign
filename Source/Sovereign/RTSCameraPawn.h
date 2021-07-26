// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Components/SphereComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"
#include "RTSCameraPawn.generated.h"

UCLASS(Blueprintable)
class SOVEREIGN_API ARTSCameraPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ARTSCameraPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Viewport")
	USphereComponent* SphereComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Viewport")
	USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Viewport")
	UCameraComponent* Camera;

	UPROPERTY()
	float fCameraDefaultDistance = 1200.0f;

	UPROPERTY()
	float fCameraZoomSpeed = 75.0f;

	UPROPERTY()
	float fCameraMinZoom = 500.0f;

	UPROPERTY()
	float fCameraMaxZoom = 2500.0f;

	UPROPERTY()
	float fCameraSpeed = 5.0f;

	UPROPERTY()
	float fCameraSpeedModifier = 1.0f;

	UPROPERTY()
	float fPanSpeed = 1.5f;

	UPROPERTY()
	float fPanMin = 0.0f;

	UPROPERTY()
	float fPanMax = 70.0f;

	UPROPERTY()
	float fCameraSpeedDefaultModifier = 1.0f;

	UPROPERTY()
	float fCameraSpeedExtraModifier = 2.5f;

	UPROPERTY()
	float fViewportTriggerSizeX = 0.1f;

	UPROPERTY()
	float fViewportTriggerSizeY = 0.1f;

	UPROPERTY()
	float fPrevMouseX = 0.0f;

	UPROPERTY()
	float fPrevMouseY = 0.0f;

	UPROPERTY()
	bool bMoveForwardFlag;

	UPROPERTY()
	bool bMoveBackFlag;

	UPROPERTY()
	bool bMoveLeftFlag;

	UPROPERTY()
	bool bMoveRightFlag;

	UPROPERTY()
	bool bPanWithMouseFlag;

	UPROPERTY()
	bool bPanClockwiseFlag;

	UPROPERTY()
	bool bCounterClockwiseFlag;

private:

	FVector GetCameraPanDirection();
	FVector2D GetGameViewportSize();

	void OnMoveForwardPressed();
	void OnMoveForwardReleased();
	void OnMoveBackPressed();
	void OnMoveBackReleased();
	void OnMoveRightPressed();
	void OnMoveRightReleased();
	void OnMoveLeftPressed();
	void OnMoveLeftReleased();
	void OnCameraSpeedUpPressed();
	void OnCameraSpeedUpReleased();

	void OnPanWithMousePressed();
	void OnPanWithMouseReleased();
	void OnPanClockwisePressed();
	void OnPanClockwiseReleased();
	void OnPanCounterClockwisePressed();
	void OnPanCounterClockwiseReleased();

	void OnCameraResetPressed();
	void OnZoomInPressed();
	void OnZoomOutPressed();

	void MoveAlong_X_Axis(float Axis);
	void MoveAlong_Y_Axis(float Axis);
	void PanWithMouse();
	void Pan(float Axis);
};

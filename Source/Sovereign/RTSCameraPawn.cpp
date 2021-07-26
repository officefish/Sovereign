// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSCameraPawn.h"

// Sets default values
ARTSCameraPawn::ARTSCameraPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create Subobjects
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	RootComponent = SphereComponent;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	SpringArm->SetupAttachment(SphereComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("RTSCamera"));
	Camera->SetupAttachment(SpringArm);

	// SetupSubobjects;

	// Collision Setup
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	SphereComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);
	// ECollisionChannel::ECC_GameTraceChannel1 is first custom object channel: MapBoundaries
	SphereComponent->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);

	SpringArm->bDoCollisionTest = false;
	//SpringArm->ProbeSize = 12;
	//SpringArm->ProbeChannel = ECollisionChannel::ECC_Camera;

	// Camera defaults
	SpringArm->bEnableCameraLag = true;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraLagSpeed = 12.0f;
	SpringArm->CameraRotationLagSpeed = 12.0f;
	SpringArm->TargetArmLength = fCameraDefaultDistance;
	SpringArm->SetRelativeRotation(FRotator(-70.0, 0.0, 0.0));
}

// Called when the game starts or when spawned
void ARTSCameraPawn::BeginPlay() {
	Super::BeginPlay();
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetShowMouseCursor(true);
}

// Called every frame
void ARTSCameraPawn::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	if (bMoveForwardFlag) MoveAlong_X_Axis(1.0f);
	if (bMoveBackFlag) MoveAlong_X_Axis(-1.0f);
	if (bMoveRightFlag)	MoveAlong_Y_Axis(1.0f);
	if (bMoveLeftFlag) MoveAlong_Y_Axis(-1.0f);

	if (bPanWithMouseFlag) PanWithMouse();
	if (bPanClockwiseFlag) Pan(1.0f);
	if (bCounterClockwiseFlag) Pan(-1.0f);

	this->AddActorWorldOffset(GetCameraPanDirection(), true); // sweep true for MapBoundaries Collision works

}

// Called to bind functionality to input
void ARTSCameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("MoveForward", IE_Pressed, this, &ARTSCameraPawn::OnMoveForwardPressed);
	PlayerInputComponent->BindAction("MoveForward", IE_Released, this, &ARTSCameraPawn::OnMoveForwardReleased);
	PlayerInputComponent->BindAction("MoveBack", IE_Pressed, this, &ARTSCameraPawn::OnMoveBackPressed);
	PlayerInputComponent->BindAction("MoveBack", IE_Released, this, &ARTSCameraPawn::OnMoveBackReleased);
	PlayerInputComponent->BindAction("MoveRight", IE_Pressed, this, &ARTSCameraPawn::OnMoveRightPressed);
	PlayerInputComponent->BindAction("MoveRight", IE_Released, this, &ARTSCameraPawn::OnMoveRightReleased);
	PlayerInputComponent->BindAction("MoveLeft", IE_Pressed, this, &ARTSCameraPawn::OnMoveLeftPressed);
	PlayerInputComponent->BindAction("MoveLeft", IE_Released, this, &ARTSCameraPawn::OnMoveLeftReleased);

	PlayerInputComponent->BindAction("CameraSpeedUp", IE_Pressed, this, &ARTSCameraPawn::OnCameraSpeedUpPressed);
	PlayerInputComponent->BindAction("CameraSpeedUp", IE_Released, this, &ARTSCameraPawn::OnCameraSpeedUpReleased);

	PlayerInputComponent->BindAction("PanWithMouse", IE_Pressed, this, &ARTSCameraPawn::OnPanWithMousePressed);
	PlayerInputComponent->BindAction("PanWithMouse", IE_Released, this, &ARTSCameraPawn::OnPanWithMouseReleased);
	PlayerInputComponent->BindAction("PanClockwise", IE_Pressed, this, &ARTSCameraPawn::OnPanClockwisePressed);
	PlayerInputComponent->BindAction("PanClockwise", IE_Released, this, &ARTSCameraPawn::OnPanClockwiseReleased);
	PlayerInputComponent->BindAction("PanCounterClockwise", IE_Pressed, this, &ARTSCameraPawn::OnPanCounterClockwisePressed);
	PlayerInputComponent->BindAction("PanCounterClockwise", IE_Released, this, &ARTSCameraPawn::OnPanCounterClockwiseReleased);

	PlayerInputComponent->BindAction("CameraReset", IE_Pressed, this, &ARTSCameraPawn::OnCameraResetPressed);
	PlayerInputComponent->BindAction("ZoomIn", IE_Pressed, this, &ARTSCameraPawn::OnZoomInPressed);
	PlayerInputComponent->BindAction("ZoomOut", IE_Pressed, this, &ARTSCameraPawn::OnZoomOutPressed);

}

// Tick Constants Listeners:
void ARTSCameraPawn::OnMoveForwardPressed() {
	bMoveForwardFlag = true;
	bMoveBackFlag = false;
}
void ARTSCameraPawn::OnMoveForwardReleased() {
	bMoveForwardFlag = false;
}
void ARTSCameraPawn::OnMoveBackPressed() {
	bMoveBackFlag = true;
	bMoveForwardFlag = false;
}
void ARTSCameraPawn::OnMoveBackReleased() {
	bMoveBackFlag = false;
}
void ARTSCameraPawn::OnMoveRightPressed() {
	bMoveRightFlag = true;
	bMoveLeftFlag = false;
}
void ARTSCameraPawn::OnMoveRightReleased() {
	bMoveRightFlag = false;
}
void ARTSCameraPawn::OnMoveLeftPressed() {
	bMoveLeftFlag = true;
	bMoveRightFlag = false;
}
void ARTSCameraPawn::OnMoveLeftReleased() {
	bMoveLeftFlag = false;
}
void ARTSCameraPawn::OnPanWithMousePressed() {
	bPanWithMouseFlag = true;
}
void ARTSCameraPawn::OnPanWithMouseReleased() {
	bPanWithMouseFlag = false;
}
void ARTSCameraPawn::OnPanClockwisePressed() {
	bPanClockwiseFlag = true;
}
void ARTSCameraPawn::OnPanClockwiseReleased() {
	bPanClockwiseFlag = false;
}
void ARTSCameraPawn::OnPanCounterClockwisePressed() {
	bCounterClockwiseFlag = true;
}
void ARTSCameraPawn::OnPanCounterClockwiseReleased() {
	bCounterClockwiseFlag = false;
}

void ARTSCameraPawn::OnCameraSpeedUpPressed() {
	fCameraSpeedModifier = fCameraSpeedExtraModifier;
}
void ARTSCameraPawn::OnCameraSpeedUpReleased() {
	fCameraSpeedModifier = fCameraSpeedDefaultModifier;
}

// MouseWheel
// CameraZoom
void ARTSCameraPawn::OnZoomInPressed() {
	float ZoomModifier = SpringArm->TargetArmLength - fCameraZoomSpeed;
	ZoomModifier = FMath::Clamp(ZoomModifier, fCameraMinZoom, fCameraMaxZoom);
	SpringArm->TargetArmLength = ZoomModifier;
}
void ARTSCameraPawn::OnZoomOutPressed() {
	float ZoomModifier = SpringArm->TargetArmLength + fCameraZoomSpeed;
	ZoomModifier = FMath::Clamp(ZoomModifier, fCameraMinZoom, fCameraMaxZoom);
	SpringArm->TargetArmLength = ZoomModifier;
}

// Home
// CameraReset
void ARTSCameraPawn::OnCameraResetPressed() {
	FRotator NewRotation{ 0.0f, 0.0f, 0.0f };
	this->SetActorRotation(NewRotation);
	SpringArm->TargetArmLength = fCameraDefaultDistance;
}

// ASWD Controls
// Camera Move
void ARTSCameraPawn::MoveAlong_X_Axis(float Axis) {
	FVector Direction = this->GetActorRotation().Quaternion().GetForwardVector();
	float Modifier = Axis * fCameraSpeed * fCameraSpeedModifier;
	Direction.X *= Modifier;
	Direction.Y *= Modifier;
	Direction.Z = 0;
	this->AddActorWorldOffset(Direction, true); // sweep true for MapBoundaries Collision works
}
void ARTSCameraPawn::MoveAlong_Y_Axis(float Axis) {
	FVector Direction = this->GetActorRotation().Quaternion().GetRightVector();
	FVector Location = this->GetActorLocation();
	float Modifier = Axis * fCameraSpeed * fCameraSpeedModifier;
	Direction.X *= Modifier;
	Direction.Y *= Modifier;
	Location.X += Direction.X;
	Location.Y += Direction.Y;
	this->SetActorLocation(Location, true);
	// do not use AddActorWorldOffset here because it's modify Z axis while camera pitch rotation
}

// MiddleMouse Q/E Controls
// Camera Pan
void ARTSCameraPawn::PanWithMouse() {
	float fMouseX, fMouseY;
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetMousePosition(fMouseX, fMouseY);

	if (!fPrevMouseX) fPrevMouseX = fMouseX;
	float fAxisX = fMouseX - fPrevMouseX;
	fAxisX *= fPanSpeed;
	fPrevMouseX = fMouseX;

	if (!fPrevMouseY) fPrevMouseY = fMouseY;
	float fAxisY = fMouseY - fPrevMouseY;
	fAxisY *= fPanSpeed;
	fPrevMouseY = fMouseY;

	FRotator PrewRotation = this->GetActorRotation();

	float fPitch = PrewRotation.Pitch;
	fPitch += fAxisY;
	fPitch = FMath::Clamp(fPitch, fPanMin, fPanMax);

	float fYaw = PrewRotation.Yaw;
	fYaw += fAxisX;

	FRotator NewRotation{};
	NewRotation.Pitch = fPitch;
	NewRotation.Yaw = fYaw;
	NewRotation.Roll = PrewRotation.Roll;
	this->SetActorRotation(NewRotation);
}
void ARTSCameraPawn::Pan(float Axis) {
	FRotator PrewRotation = this->GetActorRotation();
	float fYaw = PrewRotation.Yaw;
	fYaw += (fPanSpeed * Axis);
	FRotator NewRotation{};
	NewRotation.Yaw = fYaw;
	NewRotation.Pitch = PrewRotation.Pitch;
	NewRotation.Roll = PrewRotation.Roll;
	this->SetActorRotation(NewRotation);
}

// Edge Scrolls
FVector ARTSCameraPawn::GetCameraPanDirection() {

	float fMouseX, fMouseY;
	float CameraDirectionX = 0;
	float CameraDirectionY = 0;
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetMousePosition(fMouseX, fMouseY);

	FVector2D ViewportSize = GetGameViewportSize();
	float MinTriggerX = FMath::Clamp(ViewportSize.X * fViewportTriggerSizeX, ViewportSize.X * 0.01f, ViewportSize.X * 0.5f);
	float MaxTriggerX = FMath::Clamp(ViewportSize.X * (1 - fViewportTriggerSizeX), ViewportSize.X * 0.5f, ViewportSize.X * 0.99f);
	float MinTriggerY = FMath::Clamp(ViewportSize.Y * fViewportTriggerSizeY, ViewportSize.Y * 0.01f, ViewportSize.Y * 0.5f);
	float MaxTriggerY = FMath::Clamp(ViewportSize.Y * (1 - fViewportTriggerSizeY), ViewportSize.Y * 0.5f, ViewportSize.Y * 0.99f);

	if (fMouseX <= MinTriggerX) {
		FVector Direction = this->GetActorRotation().Quaternion().GetRightVector();
		CameraDirectionX += Direction.X * fCameraSpeed * fCameraSpeedModifier *-1;
		CameraDirectionY += Direction.Y * fCameraSpeed * fCameraSpeedModifier *-1;
	}
	else if (fMouseX >= MaxTriggerX) {
		FVector Direction = this->GetActorRotation().Quaternion().GetRightVector();
		CameraDirectionX += Direction.X * fCameraSpeed * fCameraSpeedModifier;
		CameraDirectionY += Direction.Y * fCameraSpeed * fCameraSpeedModifier;
	}
	if (fMouseY <= MinTriggerY) {
		FVector Direction = this->GetActorRotation().Quaternion().GetForwardVector();
		CameraDirectionX += Direction.X * fCameraSpeed * fCameraSpeedModifier;
		CameraDirectionY += Direction.Y * fCameraSpeed * fCameraSpeedModifier;
	}
	else if (fMouseY >= MaxTriggerY) {
		FVector Direction = this->GetActorRotation().Quaternion().GetForwardVector();
		CameraDirectionX += Direction.X * fCameraSpeed * fCameraSpeedModifier *-1;
		CameraDirectionY += Direction.Y * fCameraSpeed * fCameraSpeedModifier *-1;
	}

	return FVector{ CameraDirectionX, CameraDirectionY, 0 };
}

FVector2D ARTSCameraPawn::GetGameViewportSize()
{
	FVector2D Result = FVector2D(1, 1);
	if (GEngine && GEngine->GameViewport) {
		GEngine->GameViewport->GetViewportSize( /*out*/Result);
	}
	return Result;
}


//UE_LOG(LogTemp, Warning, TEXT("new Y: %f"), Pitch);


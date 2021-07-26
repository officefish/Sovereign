// Fill out your copyright notice in the Description page of Project Settings.


#include "./ResourceMaster.h"

// Sets default values
AResourceMaster::AResourceMaster()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = Scene;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Scene);

	// Variant Transforms
	RandomSetup();
}

void AResourceMaster::RandomSetup() {
	if (bRandomLevel)
		Level = UResourceFunctionLibrary::RandomLevel();

	Modifier = UResourceFunctionLibrary::GetLevelModifier(Level);
	CurrentAmount = BaseAmount * Modifier;

	if (bRandomSize) {
		float RandomScale = FMath::RandRange(MinScale, MaxScale);
		FVector MeshScale{1.0f, 1.0f, 1.0f};
		MeshScale *= RandomScale;
		Mesh->SetWorldScale3D(MeshScale);
	}

	if (bRandomRotation) {
		float RandomRotation = FMath::RandRange(0, 360);
		FRotator MeshRotation{};
		MeshRotation.Yaw += RandomRotation;
		Mesh->SetWorldRotation(MeshRotation);
	}
}

// Called when the game starts or when spawned
void AResourceMaster::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AResourceMaster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

TEnumAsByte<enum EResourceLevel> UResourceFunctionLibrary::RandomLevel() {
	int32 Random = FMath::RandRange(0, 6);
	return TEnumAsByte<enum EResourceLevel>(Random);
}

float UResourceFunctionLibrary::GetLevelModifier(const TEnumAsByte<enum EResourceLevel>& Level) {
	uint8 Byte = static_cast<uint8>(Level);
	return (Byte) ? 0.8 + 0.2 * Byte : 0;
}


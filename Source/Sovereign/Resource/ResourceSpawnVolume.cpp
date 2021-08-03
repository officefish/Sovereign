// Fill out your copyright notice in the Description page of Project Settings.


#include "Recource/ResourceSpawnVolume.h"

// Sets default values
AResourceSpawnVolume::AResourceSpawnVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AResourceSpawnVolume::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AResourceSpawnVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


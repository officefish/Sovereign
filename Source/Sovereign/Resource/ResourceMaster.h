// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Math/UnrealMathUtility.h"
#include "ResourceMaster.generated.h"

UENUM(BlueprintType)
enum EResourceLevel
{
	/** */
	Deleted,
	/** */
	Base,
	Level_2,
	Level_3,
	Level_4,
	Level_5,
	Level_6

};

UCLASS()
class SOVEREIGN_API AResourceMaster : public AActor
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USceneComponent* Scene;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Legend")
	TEnumAsByte<enum EResourceLevel> Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Legend")
	FString Nomination;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Legend")
	UObject* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Random")
	bool bRandomLevel = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Random")
	bool bRandomSize = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Random")
	bool bRandomRotation = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Random")
	float MinScale = 0.5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Random")
	float MaxScale = 1.5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Amount")
	float Modifier = 1.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Amount")
	float BaseAmount = 100.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Amount")
	float CurrentAmount;

public:
	// Sets default values for this actor's properties
	AResourceMaster();

	UFUNCTION(BlueprintCallable, Category = "Random")
	void RandomSetup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

UCLASS()
class SOVEREIGN_API UResourceFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	*Add Rule to Genesis alphabet
	*
	*@param		Alphabet		Genesis Alphabet Object Reference
	*@param		Rule			Genesis Rule Object Reference
	*@return	void
	*/
	UFUNCTION(BlueprintCallable, Category = "Random")
	static TEnumAsByte<enum EResourceLevel> RandomLevel();

	UFUNCTION(BlueprintCallable, Category = "Random")
	static float GetLevelModifier(const TEnumAsByte<enum EResourceLevel>& Level);
};
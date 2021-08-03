// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Factories/MaterialFactoryNew.h"
#include "AssetRegistryModule.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialExpressionBlendMaterialAttributes.h"
#include "Materials/MaterialExpressionMakeMaterialAttributes.h"
#include "Materials/MaterialExpressionScalarParameter.h"
#include "Materials/MaterialExpressionAppendVector.h"
#include "Materials/MaterialExpressionConstant.h"
#include "Materials/MaterialExpressionLinearInterpolate.h"
#include "MaterialFactoryNewExtend.generated.h"

USTRUCT(BlueprintType)
struct SOVEREIGN_API FBlendMaterialInputTextures {

	GENERATED_BODY()

	UPROPERTY()
	UTexture* TopDiffuseTexture;

	UPROPERTY()
	UTexture* BottomDiffuseTexture;

	UPROPERTY()
	UTexture* BaseNormalTexture;

	UPROPERTY()
	UTexture* AdditionalNormalTexture;
};

USTRUCT(BlueprintType)
struct SOVEREIGN_API FBlendMaterialInputs {

	GENERATED_BODY()

	UPROPERTY()
	FString MaterialBaseName;

	UPROPERTY()
	FString Path;

	UPROPERTY()
	UMaterial* TopMaterial;

	UPROPERTY()
	UMaterial* BottomMaterial;

	UPROPERTY()
	UObject* Owner;
};

/**
 *
 */
UCLASS()
class SOVEREIGN_API UMaterialFactoryNewExtend : public UObject
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleDefaultsOnly, Category = "Factory")
	UMaterialFactoryNew* MaterialFactory;

public:
	UMaterialFactoryNewExtend();

	UMaterial* FactoryCreateNewBlend(const FBlendMaterialInputs& Inputs);

	UMaterial* FactoryCreateNewBaseColorBlend(const FBlendMaterialInputs& Inputs);

	UMaterialExpressionMakeMaterialAttributes* GetMaterialAttributes(UMaterial* From, UMaterial* To);

private:

	FBlendMaterialInputTextures GetInputsTextures(const FBlendMaterialInputs& Inputs);
};

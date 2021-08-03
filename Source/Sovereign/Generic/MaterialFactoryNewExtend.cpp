// Fill out your copyright notice in the Description page of Project Settings.


#include "./MaterialFactoryNewExtend.h"

UMaterialFactoryNewExtend::UMaterialFactoryNewExtend() {
	MaterialFactory = CreateDefaultSubobject<UMaterialFactoryNew>(TEXT("MaterialFactory"));
}

UMaterial* UMaterialFactoryNewExtend::FactoryCreateNewBaseColorBlend(const FBlendMaterialInputs& Inputs) {

	FString PackageName = Inputs.Path + Inputs.MaterialBaseName;
	UPackage* Package = CreatePackage(NULL, *PackageName);

	// Create an unreal material asset
	UMaterial* BlendMaterial = (UMaterial*)MaterialFactory->FactoryCreateNew(
		UMaterial::StaticClass(), Package, *Inputs.MaterialBaseName, RF_Standalone | RF_Public, NULL, GWarn);

	// Setup output as MaterialAttributes
	BlendMaterial->bUseMaterialAttributes = true;

	// Then, we must let Unreal do some background process about asset creation
	// and load/set dirty the package we just created.
	// Without this code, the asset creation can be not finalized and it can bring some issues later…
	FAssetRegistryModule::AssetCreated(BlendMaterial);
	Package->FullyLoad();
	Package->SetDirtyFlag(true);

	// Make material attributes expression
	auto MaterialAttributes = NewObject<UMaterialExpressionMakeMaterialAttributes>(BlendMaterial);
	BlendMaterial->Expressions.Add(MaterialAttributes);

	// Connect material attribute expression with output
	BlendMaterial->MaterialAttributes.Expression = MaterialAttributes;

	// make lerp expression
	auto BaseColorLerp = NewObject<UMaterialExpressionLinearInterpolate>(BlendMaterial);
	BlendMaterial->Expressions.Add(BaseColorLerp);

	// Connect Lerp node with Material Attributes output
	MaterialAttributes->BaseColor.Expression = BaseColorLerp;

	// Lerp Alpha Node expression
	auto BaseColorLerpAlpha = NewObject<UMaterialExpressionScalarParameter>(BlendMaterial);
	BlendMaterial->Expressions.Add(BaseColorLerpAlpha);
	BaseColorLerpAlpha->ParameterName = "BaseColorLerpAlpha";
	BaseColorLerpAlpha->DefaultValue = 0;

	// Connect BaseColorLerp Node with "BaseColorLerpAlpha" ScalarParameter
	BaseColorLerp->Alpha.Expression = BaseColorLerpAlpha;

	// ... Textures
	FBlendMaterialInputTextures InputsTextures = GetInputsTextures(Inputs);

	// Make TopMaterial Texture Samples
	auto TopTextureExpression = NewObject<UMaterialExpressionTextureSample>(BlendMaterial);
	if (InputsTextures.TopDiffuseTexture) {
		TopTextureExpression->Texture = InputsTextures.TopDiffuseTexture;
		UE_LOG(LogTemp, Warning, TEXT("Try to Setup TopDiffuseTexture"));
	}
	TopTextureExpression->SamplerType = SAMPLERTYPE_Color;
	BlendMaterial->Expressions.Add(TopTextureExpression);
	BaseColorLerp->A.Expression = TopTextureExpression;

	// Make BottomMaterial Texture Samples
	auto BottomTextureExpression = NewObject<UMaterialExpressionTextureSample>(BlendMaterial);
	if (InputsTextures.BottomDiffuseTexture) {
		BottomTextureExpression->Texture = InputsTextures.BottomDiffuseTexture;
		UE_LOG(LogTemp, Warning, TEXT("Try to Setup BottomDiffuseTexture"));
	}
	BottomTextureExpression->SamplerType = SAMPLERTYPE_Color;
	BlendMaterial->Expressions.Add(BottomTextureExpression);
	BaseColorLerp->B.Expression = BottomTextureExpression;

	// Metallic Scalar Node expression
	auto MetallicScalar = NewObject<UMaterialExpressionScalarParameter>(BlendMaterial);
	BlendMaterial->Expressions.Add(MetallicScalar);
	MetallicScalar->ParameterName = "MetallicScalar";
	MetallicScalar->DefaultValue = 0;

	// Connect metallic scalar node with Material Attributes output
	MaterialAttributes->Metallic.Expression = MetallicScalar;

	// Specular Scalar Node expression
	auto SpecularScalar = NewObject<UMaterialExpressionScalarParameter>(BlendMaterial);
	BlendMaterial->Expressions.Add(SpecularScalar);
	SpecularScalar->ParameterName = "SpecularScalar";
	SpecularScalar->DefaultValue = 0.02f;

	// Connect specular scalar node with Material Attributes output
	MaterialAttributes->Specular.Expression = SpecularScalar;

	// Roughness Scalar Node expression
	auto RoughnessScalar = NewObject<UMaterialExpressionScalarParameter>(BlendMaterial);
	BlendMaterial->Expressions.Add(RoughnessScalar);
	RoughnessScalar->ParameterName = "RoughnessScalar";
	RoughnessScalar->DefaultValue = 1.0f;

	// Connect Roughness scalar node with Material Attributes output
	MaterialAttributes->Roughness.Expression = RoughnessScalar;


	// Let the material update itself if necessary
	BlendMaterial->PreEditChange(NULL);
	BlendMaterial->PostEditChange();

	// make sure that any static meshes, etc using this material
	// will stop using the FMaterialResource of the original
	// material, and will use the new FMaterialResource created
	// when we make a new UMaterial in place
	FGlobalComponentReregisterContext RecreateComponents;

	return BlendMaterial;
}

UMaterial* UMaterialFactoryNewExtend::FactoryCreateNewBlend(const FBlendMaterialInputs& Inputs) {
	//UE_LOG(LogTemp, Warning, TEXT("UMaterialFactoryNewExtend::FactoryCreateNewBlend"));

	FString PackageName = Inputs.Path + Inputs.MaterialBaseName;
	UPackage* Package = CreatePackage(NULL, *PackageName);

	// Create an unreal material asset
	UMaterial* BlendMaterial = (UMaterial*)MaterialFactory->FactoryCreateNew(
		UMaterial::StaticClass(), Package, *Inputs.MaterialBaseName, RF_Standalone | RF_Public, NULL, GWarn);

	// Setup output as MaterialAttributes
	BlendMaterial->bUseMaterialAttributes = true;

	//Inputs.TopMaterial->bUseMaterialAttributes = true;
	//Inputs.BottomMaterial->bUseMaterialAttributes = true;

	// Then, we must let Unreal do some background process about asset creation
	// and load/set dirty the package we just created.
	// Without this code, the asset creation can be not finalized and it can bring some issues later…
	FAssetRegistryModule::AssetCreated(BlendMaterial);
	Package->FullyLoad();
	Package->SetDirtyFlag(true);

	// Blend Material Node
	auto BlendExpression = NewObject<UMaterialExpressionBlendMaterialAttributes>(BlendMaterial);
	BlendMaterial->Expressions.Add(BlendExpression);

	// Alpha scalar parameter for blend node
	auto Alpha = NewObject<UMaterialExpressionScalarParameter>(BlendMaterial);
	BlendMaterial->Expressions.Add(BlendExpression);
	Alpha->ParameterName = "Alpha";
	Alpha->DefaultValue = 0;

	// Connect Blend Material Node Alpha with "Alpha" ScalarParameter
	BlendExpression->Alpha.Expression = Alpha;

	auto TopMaterialAttributes = GetMaterialAttributes(Inputs.TopMaterial, BlendMaterial);
	BlendExpression->A.Expression = TopMaterialAttributes;

	// Connect Blend function with output
	BlendMaterial->MaterialAttributes.Expression = BlendExpression;

	//auto ConstantAlpha = NewObject<UMaterialExpressionConstant>(BlendMaterial);
	//BlendMaterial->Expressions.Add(ConstantAlpha);

	//ConstantAlpha-> =

	//BlendExpression->Alpha.Expression = ConstantAlpha;

	// Let the material update itself if necessary
	BlendMaterial->PreEditChange(NULL);
	BlendMaterial->PostEditChange();

	// make sure that any static meshes, etc using this material
	// will stop using the FMaterialResource of the original
	// material, and will use the new FMaterialResource created
	// when we make a new UMaterial in place
	FGlobalComponentReregisterContext RecreateComponents;

	return BlendMaterial;
}

UMaterialExpressionMakeMaterialAttributes* UMaterialFactoryNewExtend::GetMaterialAttributes(UMaterial* From, UMaterial* To) {

	auto MaterialAttributes = NewObject<UMaterialExpressionMakeMaterialAttributes>(To);

	//From->BaseColor.

	//UE_LOG(LogTemp, Warning, TEXT("%s"), *From->BaseColor.ExpressionName.ToString());

	//MaterialAttributes->BaseColor.Expression = From->BaseColor.Expression;
	//MaterialAttributes->Metallic.Expression = From->;
	//MaterialAttributes->Specular.Expression = From->Specular.Expression;
	//MaterialAttributes->Roughness.Expression = From->Roughness.Expression;
	//MaterialAttributes->Anisotropy.Expression = From->Anisotropy.Expression;
	//MaterialAttributes->EmissiveColor.Expression = From->EmissiveColor.Expression;
	//MaterialAttributes->Opacity.Expression = From->Opacity.Expression;
	//MaterialAttributes->OpacityMask.Expression = From->OpacityMask.Expression;
	//MaterialAttributes->Normal.Expression = From->Normal.Expression;
	//MaterialAttributes->Tangent.Expression = From->Tangent.Expression;
	//MaterialAttributes->WorldPositionOffset.Expression = From->WorldPositionOffset.Expression;
	//MaterialAttributes->WorldDisplacement.Expression = From->WorldDisplacement.Expression;
	//MaterialAttributes->TessellationMultiplier.Expression = From->TessellationMultiplier.Expression;
	//MaterialAttributes->SubsurfaceColor.Expression = From->SubsurfaceColor.Expression;
	//MaterialAttributes->AmbientOcclusion.Expression = From->AmbientOcclusion.Expression;

	//MaterialAttributes->Refraction.Expression = From->Refraction.Expression;
	//MaterialAttributes->PixelDepthOffset.Expression = From->PixelDepthOffset.Expression;

	return MaterialAttributes;
}

FBlendMaterialInputTextures UMaterialFactoryNewExtend::GetInputsTextures(const FBlendMaterialInputs& Inputs) {

	FBlendMaterialInputTextures Textures{};

	auto TopMaterialInstance = UMaterialInstanceDynamic::Create(Inputs.TopMaterial, Inputs.Owner);
	auto BottomMaterialInstance = UMaterialInstanceDynamic::Create(Inputs.BottomMaterial, Inputs.Owner);

	TArray<UTexture*> TopOutTextures;
	TopMaterialInstance->GetUsedTextures(TopOutTextures, EMaterialQualityLevel::Num, true, ERHIFeatureLevel::Num, true);

	TArray<UTexture*> BottomOutTextures;
	TopMaterialInstance->GetUsedTextures(BottomOutTextures, EMaterialQualityLevel::Num, true, ERHIFeatureLevel::Num, true);

	//UE_LOG(LogTemp, Warning, TEXT("TopOutTextures.Num() : %d"), TopOutTextures.Num());
	//UE_LOG(LogTemp, Warning, TEXT("BottomOutTextures.Num() : %d"), BottomOutTextures.Num());
	//for (auto& Texture : TopOutTextures) {
	//	Texture->
	//}

	if (TopOutTextures.Num() > 2)
		Textures.TopDiffuseTexture = TopOutTextures[2];

	if (BottomOutTextures.Num() > 2)
		Textures.TopDiffuseTexture = BottomOutTextures[2];

	if (TopOutTextures.Num() > 1)
		Textures.BaseNormalTexture = TopOutTextures[1];

	if (TopOutTextures.Num())
		Textures.AdditionalNormalTexture = TopOutTextures[0];

	return Textures;
}
/**
 * Copyright (c) Facebook, Inc. and its affiliates.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "VRayMaterial.hpp"

std::unique_ptr<FbxVRayMaterialInfo> FbxVRayMaterialResolver::Resolve() const
{
	auto getTexture = [&](const FbxProperty& prop) -> FbxFileTexture*
	{
		if (prop.IsValid())
		{
			FbxFileTexture* tex = prop.GetSrcObject<FbxFileTexture>();
			if (tex != nullptr && textureLocations.find(tex) == textureLocations.end())
				return tex;
		}

		return nullptr;
	};

	const FbxProperty topProp = fbxMaterial->FindProperty("3dsMax", false);
	if (topProp.GetPropertyDataType() != FbxCompoundDT)
		return nullptr;

	const FbxProperty basicProp = topProp.Find("basic", false);
	if (!basicProp.IsValid())
		return nullptr;

	const FbxProperty brdfProp = topProp.Find("BRDF", false);
	if (!brdfProp.IsValid())
		return nullptr;

	const FbxProperty optionsProp = topProp.Find("options", false);
	if (!optionsProp.IsValid())
		return nullptr;

	const FbxProperty mapsProp = topProp.Find("maps", false);
	if (!mapsProp.IsValid())
		return nullptr;

	std::string name = fbxMaterial->GetName();
	std::string lowerCaseName = name;
	std::for_each(lowerCaseName.begin(), lowerCaseName.end(), [](char& c) { c = std::tolower(c); });

	std::unique_ptr<FbxVRayMaterialInfo> mtl(new FbxVRayMaterialInfo(
		fbxMaterial->GetUniqueID(), name.c_str(), FbxVRayMaterialInfo::FBX_SHADER_VRAY));

	//
	// General parameters
	//
	const FbxProperty unlitProp = fbxMaterial->FindProperty("Unlit", false);
	if (unlitProp.IsValid())
		mtl->unlit = unlitProp.Get<FbxBool>();

	const FbxProperty useLightmapProp = fbxMaterial->FindProperty("UseLightmap", false);
	if (useLightmapProp.IsValid())
		mtl->useSelfIlluminationAsLightmap = useLightmapProp.Get<FbxBool>();

	const FbxProperty alphaTestProp = fbxMaterial->FindProperty("AlphaTest", false);
	if (alphaTestProp.IsValid())
		mtl->alphaTest = alphaTestProp.Get<FbxFloat>();

	const FbxProperty doubleSidedProp = optionsProp.Find("option_doubleSided");
	if (doubleSidedProp.IsValid())
		mtl->doubleSided = doubleSidedProp.Get<FbxBool>();

	//
	// Diffuse
	//
	const FbxProperty diffuseProp = basicProp.Find("diffuse");
	if (diffuseProp.IsValid())
		mtl->diffuseColor = diffuseProp.Get<FbxDouble4>();

	const FbxProperty diffuseTextureProp = mapsProp.Find("texmap_diffuse");
	mtl->diffuseTexture = getTexture(diffuseTextureProp);

	//
	// Metalness
	//
	const FbxProperty metalnessProp = basicProp.Find("reflection_metalness");
	if (metalnessProp.IsValid())
		mtl->metalness = metalnessProp.Get<FbxDouble>();

	const FbxProperty metalnessTextureProp = mapsProp.Find("texmap_metalness");
	mtl->metalnessTexture = getTexture(metalnessTextureProp);

	//
	// Reflection
	//
	const FbxProperty reflectionProp = basicProp.Find("reflection");
	if (reflectionProp.IsValid())
		mtl->reflectionColor = reflectionProp.Get<FbxDouble4>();

	bool useRoughnessInsteadOfGlossiness = false;
	const FbxProperty useRoughnessProp = brdfProp.Find("brdf_useRoughness");
	if (useRoughnessProp.IsValid())
		useRoughnessInsteadOfGlossiness = useRoughnessProp.Get<FbxBool>();

	const FbxProperty reflectionGlossinessProp = basicProp.Find("reflection_glossiness");
	if (reflectionGlossinessProp.IsValid())
	{
		if (useRoughnessInsteadOfGlossiness)
			mtl->roughness = reflectionGlossinessProp.Get<FbxDouble>();
		else
			mtl->roughness = 1.0 - reflectionGlossinessProp.Get<FbxDouble>();
	}

	const FbxProperty reflectionGlossinessTextureProp = mapsProp.Find("texmap_reflectionGlossiness");
	mtl->roughnessTexture = getTexture(reflectionGlossinessTextureProp);

	const FbxProperty glossinessMapMin = fbxMaterial->FindProperty("GlossinessMapMin");
	const FbxProperty glossinessMapMax = fbxMaterial->FindProperty("GlossinessMapMax");
	if (glossinessMapMin.IsValid() && glossinessMapMax.IsValid())
	{
		if (useRoughnessInsteadOfGlossiness)
		{
			mtl->roughnessMapMin = glossinessMapMin.Get<FbxDouble>();
			mtl->roughnessMapMax = glossinessMapMax.Get<FbxDouble>();
		}
		else
		{
			mtl->roughnessMapMin = glossinessMapMax.Get<FbxDouble>();
			mtl->roughnessMapMax = glossinessMapMin.Get<FbxDouble>();
		}
	}
	else
	{
		mtl->roughnessMapMin = useRoughnessInsteadOfGlossiness ? 0 : 1;
		mtl->roughnessMapMax = useRoughnessInsteadOfGlossiness ? 1 : 0;
	}

	//
	// Bump
	//
	const FbxProperty bumpTextureProp = mapsProp.Find("texmap_bump");
	mtl->bumpTexture = getTexture(bumpTextureProp);

	const FbxProperty bumpTextureMultiplierProp = mapsProp.Find("texmap_bump_multiplier");
	if (bumpTextureMultiplierProp.IsValid())
		mtl->bumpMultiplier = bumpTextureMultiplierProp.Get<FbxDouble>();

	mtl->useBumpAsNormal = lowerCaseName.find("normal") != std::string::npos;
	mtl->invertNormalMapY = lowerCaseName.find("inverty") != std::string::npos;

	//
	// Self-illumination
	//
	const FbxProperty selfIllumColorProp = basicProp.Find("selfIllumination");
	if (selfIllumColorProp.IsValid())
		mtl->selfIlluminationColor = selfIllumColorProp.Get<FbxDouble4>();

	const FbxProperty selfIllumTextureProp = mapsProp.Find("texmap_self_illumination");
	mtl->selfIlluminationTexture = getTexture(selfIllumTextureProp);

	const FbxProperty selfIllumMultiplierProp = basicProp.Find("selfIllumination_multiplier");
	if (selfIllumMultiplierProp.IsValid())
		mtl->selfIlluminationMultiplier = selfIllumMultiplierProp.Get<FbxDouble>();

	//
	// Refraction and Opacity
	//
	const FbxProperty refractionProp = basicProp.Find("refraction");
	if (refractionProp.IsValid())
		mtl->refractionColor = refractionProp.Get<FbxDouble4>();

	const FbxProperty opacityTextureProp = mapsProp.Find("texmap_opacity");
	mtl->opacityTexture = getTexture(opacityTextureProp);


	return mtl;
}

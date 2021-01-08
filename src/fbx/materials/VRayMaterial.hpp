/**
 * Copyright (c) Facebook, Inc. and its affiliates.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "FbxMaterials.hpp"

struct FbxVRayMaterialInfo : FbxMaterialInfo
{
	static constexpr const char* FBX_SHADER_VRAY = "VRay";

	FbxVRayMaterialInfo(const FbxUInt64 id, const FbxString& name, const FbxString& shadingModel)
		: FbxMaterialInfo(id, name, shadingModel),
		unlit(false),
		transparent(false),
		doubleSided(false),
		alphaTest(0),
		diffuseColor(1.0, 1.0, 1.0),
		diffuseTexture(nullptr),
		bumpTexture(nullptr),
		bumpMultiplier(1.0f),
		useBumpAsNormal(false),
		invertNormalMapY(false),
		roughness(1.0f),
		roughnessTexture(nullptr),
		roughnessMapMin(0),
		roughnessMapMax(1.0f),
		metalness(0),
		metalnessTexture(nullptr),
		refractionColor(0,0,0),
		opacityTexture(nullptr),
		selfIlluminationTexture(nullptr),
		selfIlluminationMultiplier(0),
		useSelfIlluminationAsLightmap(false)
	{
	}

	bool unlit;
	bool transparent;
	bool doubleSided;
	float alphaTest; // 0 if alpha test is disabled.

	FbxVector4 diffuseColor;
	FbxFileTexture* diffuseTexture;

	FbxFileTexture* bumpTexture;
	FbxDouble bumpMultiplier;
	bool useBumpAsNormal;
	bool invertNormalMapY;

	FbxVector4 reflectionColor;

	FbxDouble roughness;
	FbxFileTexture* roughnessTexture;
	FbxDouble roughnessMapMin, roughnessMapMax;

	FbxDouble metalness;
	FbxFileTexture* metalnessTexture;

	FbxVector4 refractionColor;
	FbxFileTexture* opacityTexture;

	FbxVector4 selfIlluminationColor;
	FbxFileTexture* selfIlluminationTexture;
	FbxDouble selfIlluminationMultiplier;
	bool useSelfIlluminationAsLightmap;
};

class FbxVRayMaterialResolver : FbxMaterialResolver<FbxVRayMaterialInfo>
{
public:
	FbxVRayMaterialResolver(FbxSurfaceMaterial* fbxMaterial)
		: FbxMaterialResolver(fbxMaterial)
	{
	}

	virtual std::unique_ptr<FbxVRayMaterialInfo> Resolve() const;
};

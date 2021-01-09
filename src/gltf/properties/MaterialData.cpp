/**
 * Copyright (c) Facebook, Inc. and its affiliates.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "MaterialData.hpp"
#include "TextureData.hpp"

// TODO: retrieve & pass in correct UV set from FBX
std::unique_ptr<Tex> Tex::ref(const TextureData* tex, uint32_t texCoord)
{
	return std::unique_ptr<Tex>{(tex != nullptr) ? new Tex(tex->ix, texCoord) : nullptr};
}

Tex::Tex(uint32_t texRef, uint32_t texCoord) : texRef(texRef), texCoord(texCoord)
{
}

void to_json(json& j, const Tex& data)
{
	j = json{{"index", data.texRef}, {"texCoord", data.texCoord}};
}

inline float clamp(float d, float bottom = 0, float top = 1)
{
	return std::max(bottom, std::min(top, d));
}

inline Vec3f
clamp(const Vec3f& vec, const Vec3f& bottom = VEC3F_ZERO, const Vec3f& top = VEC3F_ONE)
{
	return Vec3f::Max(bottom, Vec3f::Min(top, vec));
}

inline Vec4f
clamp(const Vec4f& vec, const Vec4f& bottom = VEC4F_ZERO, const Vec4f& top = VEC4F_ONE)
{
	return Vec4f::Max(bottom, Vec4f::Min(top, vec));
}

MaterialData::MaterialData(
	std::string name,
	RawShadingModel shadingModel,
	float alphaTest,
	bool isDoubleSided,
	const TextureData* diffuseTexture,
	const Vec4f diffuseColor,
	const Vec3f specularColor,
	const TextureData* normalTexture,
	bool invertNormalMapY,
	const TextureData* metallicTexture,
	const float metallic,
	const TextureData* roughnessTexture,
	const float roughness,
	const float roughnessMapMin,
	const float roughnessMapMax,
	const TextureData* occlusionTexture,
	const TextureData* emissiveTexture,
	const Vec3f& emissiveColor,
	const TextureData* bumpTexture,
	float bumpFactor,
	const TextureData* opacityTexture,
	const TextureData* lightmapTexture) :
	name(std::move(name)),
	shadingModel(shadingModel),
	alphaTest(clamp(alphaTest)),
	isDoubleSided(isDoubleSided),
	diffuseTexture(Tex::ref(diffuseTexture)),
	diffuseColor(diffuseColor),
	specularColor(specularColor),
	normalTexture(Tex::ref(normalTexture)),
	invertNormalMapY(invertNormalMapY),
	metallicTexture(Tex::ref(metallicTexture)),
	metallic(clamp(metallic)),
	roughnessTexture(Tex::ref(roughnessTexture)),
	roughness(clamp(roughness)),
	roughnessMapMin(clamp(roughnessMapMin)),
	roughnessMapMax(clamp(roughnessMapMax)),
	occlusionTexture(Tex::ref(occlusionTexture)),
	emissiveTexture(Tex::ref(emissiveTexture)),
	emissiveColor(clamp(emissiveColor)),
	bumpTexture(Tex::ref(bumpTexture)),
	bumpFactor(bumpFactor),
	opacityTexture(Tex::ref(opacityTexture)),
	lightmapTexture(Tex::ref(lightmapTexture))
{
}

json MaterialData::serialize() const
{
	json result = {
		{"name", name},
		{"shadingModel", Describe(shadingModel)}
/*		{
			"extras",
			{
				"fromFBX",
				{
					"userProperties",
					{
					}
				}
			}
		}*/
	};

	if (alphaTest > 0)
		result["alphaTest"] = alphaTest;

	if (isDoubleSided)
		result["doubleSided"] = isDoubleSided;

	if (diffuseTexture != nullptr)
		result["diffuseTexture"] = *diffuseTexture;

	result["diffuseColor"] = toStdVec(diffuseColor);
	result["specularColor"] = toStdVec(specularColor);

	if (diffuseTexture != nullptr)
		result["diffuseTexture"] = *diffuseTexture;

	if (emissiveTexture != nullptr)
		result["emissiveTexture"] = *emissiveTexture;
	result["emissiveColor"] = toStdVec(emissiveColor);

	if (opacityTexture != nullptr)
		result["opacityTexture"] = *opacityTexture;

	if (shadingModel != RAW_SHADING_MODEL_UNLIT)
	{
		if (normalTexture != nullptr)
			result["normalTexture"] = *normalTexture;

		if (invertNormalMapY)
			result["invertNormalMapY"] = invertNormalMapY;

		if (metallicTexture != nullptr)
			result["metallicTexture"] = *metallicTexture;
		if (metallic > 0)
			result["metallic"] = metallic;

		if (roughnessTexture != nullptr)
			result["roughnessTexture"] = *roughnessTexture;

		if (roughness < 1.0f)
			result["roughness"] = roughness;
		if (roughnessMapMin > 0 || roughnessMapMax < 1)
		{
			result["roughnessMapMin"] = roughnessMapMin;
			result["roughnessMapMax"] = roughnessMapMax;
		}

		if (occlusionTexture != nullptr)
			result["occlusionTexture"] = *occlusionTexture;

		if (bumpTexture != nullptr)
			result["bumpTexture"] = *bumpTexture;
		if (bumpFactor != 1.0f)
			result["bumpFactor"] = bumpFactor;

		if (lightmapTexture != nullptr)
			result["lightmapTexture"] = *lightmapTexture;
	}

/*
	for (const auto& i : userProperties)
	{
		auto& prop_map = result["extras"]["fromFBX"]["userProperties"];

		json j = json::parse(i);
		for (const auto& k : json::iterator_wrapper(j))
		{
			prop_map[k.key()] = k.value();
		}
	}
*/
	return result;
}

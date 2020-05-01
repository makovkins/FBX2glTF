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
	bool isTransparent,
	const TextureData* diffuseTexture,
	const Vec4f diffuseColor,
	const TextureData* normalTexture,
	const TextureData* metallicTexture,
	const float metallic,
	const TextureData* roughnessTexture,
	const float roughness,	
	const TextureData* occlusionTexture,
	const TextureData* emissiveTexture,
	const Vec3f& emissiveColor,
	const TextureData* bumpTexture,
	float bumpFactor,
	const TextureData* opacityTexture) :
		name(std::move(name)),
		shadingModel(shadingModel),
		isTransparent(isTransparent),
		diffuseTexture(Tex::ref(diffuseTexture)),
		diffuseColor(diffuseColor),
		normalTexture(Tex::ref(normalTexture)),
		metallicTexture(Tex::ref(metallicTexture)),
		metallic(clamp(metallic)),
		roughnessTexture(Tex::ref(roughnessTexture)),
		roughness(clamp(roughness)),
		occlusionTexture(Tex::ref(occlusionTexture)),
		emissiveTexture(Tex::ref(emissiveTexture)),
		emissiveColor(clamp(emissiveColor)),
		bumpTexture(Tex::ref(bumpTexture)),
		bumpFactor(bumpFactor),
		opacityTexture(Tex::ref(opacityTexture))
{
}

json MaterialData::serialize() const
{
	json result = {
		{"name", name},
		{
			"extras",
			{
				{
					"fromFBX",
					{
						{"shadingModel", Describe(shadingModel)},
						{"isTruePBR", shadingModel == RAW_SHADING_MODEL_PBR_MET_ROUGH}
					}
				}
			}
		}
	};

	std::string lowerCaseName = name;
	std::for_each(lowerCaseName.begin(), lowerCaseName.end(), [](char& c) { c = std::tolower(c); });

	if (lowerCaseName.find("alphatest") != std::string::npos)
		result["alphaMode"] = "MASK";
	else
		result["alphaMode"] = isTransparent ? "BLEND" : "OPAQUE";

	if (diffuseTexture != nullptr)
		result["diffuseTexture"] = *diffuseTexture;
	result["diffuseColor"] = toStdVec(diffuseColor);

	if (diffuseTexture != nullptr)
		result["diffuseTexture"] = *diffuseTexture;
	
	if (normalTexture != nullptr)
		result["normalTexture"] = *normalTexture;

	if (metallicTexture != nullptr)
		result["metallicTexture"] = *metallicTexture;
	if (metallic != 1.0f)
		result["metallic"] = metallic;	

	if (roughnessTexture != nullptr)
		result["roughnessTexture"] = *roughnessTexture;
	if (roughness != 1.0f)
		result["roughness"] = roughness;
	
	if (occlusionTexture != nullptr)
		result["occlusionTexture"] = *occlusionTexture;

	if (emissiveTexture != nullptr)
		result["emissiveTexture"] = *emissiveTexture;	
	result["emissiveColor"] = toStdVec(emissiveColor);

	if (bumpTexture != nullptr)
		result["bumpTexture"] = *bumpTexture;
	if (bumpFactor != 1.0f)
		result["bumpFactor"] = bumpFactor;

	if (opacityTexture != nullptr)
		result["opacityTexture"] = *opacityTexture;

	for (const auto& i : userProperties)
	{
		auto& prop_map = result["extras"]["fromFBX"]["userProperties"];

		json j = json::parse(i);
		for (const auto& k : json::iterator_wrapper(j))
		{
			prop_map[k.key()] = k.value();
		}
	}

	return result;
}

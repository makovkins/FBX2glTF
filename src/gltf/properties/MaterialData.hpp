/**
 * Copyright (c) Facebook, Inc. and its affiliates.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <string>

#include "gltf/Raw2Gltf.hpp"

struct Tex
{
	static std::unique_ptr<Tex> ref(const TextureData* tex, uint32_t texCoord = 0);
	explicit Tex(uint32_t texRef, uint32_t texCoord);

	const uint32_t texRef;
	const uint32_t texCoord;
};

struct MaterialData : Holdable
{
	MaterialData(
		std::string name,
		RawShadingModel shadingModel,
		float alphaTest,
		bool isDoubleSided,
		const Vec2f& uvTranslation,
		const Vec2f& uvScale,
		float uvRotation,
		const TextureData* diffuseTexture,
		Vec4f diffuseColor,
		Vec3f specularColor,
		const TextureData* normalTexture,
		bool invertNormalMapY,
		const TextureData* metallicTexture,
		float metallic,
		const TextureData* roughnessTexture,
		float roughness,
		float roughnessMapMin,
		float roughnessMapMax,
		const TextureData* occlusionTexture,
		const TextureData* emissiveTexture,
		const Vec3f& emissiveColor,
		const TextureData* bumpTexture,
		float bumpFactor,
		const TextureData* opacityTexture,
		const TextureData* lightmapTexture);

	json serialize() const override;

	const std::string name;
	const RawShadingModel shadingModel;
	const float alphaTest;
	const bool isDoubleSided;
	const Vec2f uvTranslation;
	const Vec2f uvScale;
	const float uvRotation;
	const std::unique_ptr<const Tex> diffuseTexture;
	const Vec4f diffuseColor;
	const Vec3f specularColor;
	const std::unique_ptr<const Tex> normalTexture;
	const bool invertNormalMapY;
	const std::unique_ptr<const Tex> metallicTexture;
	const float metallic;
	const std::unique_ptr<const Tex> roughnessTexture;
	const float roughness;
	const float roughnessMapMin;
	const float roughnessMapMax;
	const std::unique_ptr<const Tex> occlusionTexture;
	const std::unique_ptr<const Tex> emissiveTexture;
	const Vec3f emissiveColor;
	const std::unique_ptr<const Tex> bumpTexture;
	const float bumpFactor;
	const std::unique_ptr<const Tex> opacityTexture;
	const std::unique_ptr<const Tex> lightmapTexture;

	std::vector<std::string> userProperties;
};

void to_json(json& j, const Tex& data);

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
		const TextureData* opacityTexture);

	json serialize() const override;

	const std::string name;
	const RawShadingModel shadingModel;
	const bool isTransparent;	
	const std::unique_ptr<const Tex> diffuseTexture;
	const Vec4f diffuseColor;
	const std::unique_ptr<const Tex> normalTexture;
	const std::unique_ptr<const Tex> metallicTexture;
	const float metallic;	
	const std::unique_ptr<const Tex> roughnessTexture;
	const float roughness;
	const std::unique_ptr<const Tex> occlusionTexture;
	const std::unique_ptr<const Tex> emissiveTexture;
	const Vec3f emissiveColor;
	const std::unique_ptr<const Tex> bumpTexture;
	const float bumpFactor;
	const std::unique_ptr<const Tex> opacityTexture;

	std::vector<std::string> userProperties;
};

void to_json(json& j, const Tex& data);

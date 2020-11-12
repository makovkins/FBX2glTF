/**
 * Copyright (c) Facebook, Inc. and its affiliates.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include "gltf/Raw2Gltf.hpp"

struct TextureData : Holdable {
  TextureData(std::string name, const SamplerData& sampler, const ImageData& source);

  json serialize() const override;

  const std::string name;
  const uint32_t sampler;
  const uint32_t source;
};

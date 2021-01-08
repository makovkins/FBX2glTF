/**
 * Copyright (c) Facebook, Inc. and its affiliates.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <map>
#include <vector>

#include "FBX2glTF.h"

class FbxMaterialInfo
{
public:
	FbxMaterialInfo(const FbxUInt64 id, const FbxString& name, const FbxString& shadingModel)
		: id(id), name(name), shadingModel(shadingModel)
	{
	}

	const FbxUInt64 id;
	const FbxString name;
	const FbxString shadingModel;
};

template <class T>
class FbxMaterialResolver
{
public:
	FbxMaterialResolver(FbxSurfaceMaterial* fbxMaterial)
		: fbxMaterial(fbxMaterial)
	{
	}

	virtual ~FbxMaterialResolver() = default;

	virtual std::unique_ptr<T> Resolve() const = 0;

protected:
	const FbxSurfaceMaterial* fbxMaterial;
};

class FbxMaterialsAccess
{
public:
	FbxMaterialsAccess(const FbxMesh* pMesh);

	const std::shared_ptr<FbxMaterialInfo> GetMaterial(const int polygonIndex) const;

	const std::vector<std::string> GetUserProperties(const int polygonIndex) const;

	static std::unique_ptr<FbxMaterialInfo> GetMaterialInfo(FbxSurfaceMaterial* material);

private:
	FbxGeometryElement::EMappingMode mappingMode;
	std::vector<std::shared_ptr<FbxMaterialInfo>> summaries{};
	std::vector<std::vector<std::string>> userProperties;
	const FbxMesh* mesh;
	const FbxLayerElementArrayTemplate<int>* indices;
};

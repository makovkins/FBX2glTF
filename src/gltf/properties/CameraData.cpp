/**
 * Copyright (c) Facebook, Inc. and its affiliates.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "CameraData.hpp"

CameraData::CameraData()
	: Holdable(), aspectRatio(0.0f), yfov(0.0f), xmag(0.0f), ymag(0.0f), znear(0.0f), zfar(0.0f)
{
}

json CameraData::serialize() const
{
	json result{
		{"name", name},
		{"type", type},
	};
	json subResult{{"znear", znear}, {"zfar", zfar}};
	if (type == "perspective")
	{
		subResult["aspectRatio"] = aspectRatio;
		subResult["yfov"] = yfov;
	}
	else
	{
		subResult["xmag"] = xmag;
		subResult["ymag"] = ymag;
	}
	result[type] = subResult;
	return result;
}

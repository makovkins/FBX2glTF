/**
 * Copyright (c) Facebook, Inc. and its affiliates.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "ImageData.hpp"

#include <utility>

#include "BufferViewData.hpp"

ImageData::ImageData(std::string name, std::string uri)
	: Holdable(), name(std::move(name)), uri(std::move(uri)), bufferView(-1)
{
}

ImageData::ImageData(std::string name, const BufferViewData& bufferView, std::string mimeType)
	: Holdable(), name(std::move(name)), bufferView(bufferView.ix), mimeType(std::move(mimeType))
{
}

json ImageData::serialize() const
{
	if (bufferView < 0)
	{
		return {{"name", name}, {"uri", uri}};
	}
	return {{"name", name}, {"bufferView", bufferView}, {"mimeType", mimeType}};
}

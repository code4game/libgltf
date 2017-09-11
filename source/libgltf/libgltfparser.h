#pragma once

#include "libgltfpch.h"

#include <memory>
#include <vector>

namespace libgltf
{
    bool operator<<(std::shared_ptr<struct SGlTFProperty>& _pData, const WCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SGlTFProperty>>& _pDatas, const WCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<struct SMaterial>& _pData, const WCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SMaterial>>& _pDatas, const WCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<struct SAsset>& _pData, const WCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SAsset>>& _pDatas, const WCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<struct SSampler>& _pData, const WCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SSampler>>& _pDatas, const WCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<struct SGlTFChildofRootProperty>& _pData, const WCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SGlTFChildofRootProperty>>& _pDatas, const WCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<struct SAnimationSampler>& _pData, const WCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SAnimationSampler>>& _pDatas, const WCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<struct SExtras>& _pData, const WCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SExtras>>& _pDatas, const WCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<struct SScene>& _pData, const WCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SScene>>& _pDatas, const WCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<struct SCameraPerspective>& _pData, const WCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SCameraPerspective>>& _pDatas, const WCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<struct SBufferView>& _pData, const WCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SBufferView>>& _pDatas, const WCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<struct SMaterialNormalTextureInfo>& _pData, const WCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SMaterialNormalTextureInfo>>& _pDatas, const WCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<struct SMaterialOcclusionTextureInfo>& _pData, const WCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SMaterialOcclusionTextureInfo>>& _pDatas, const WCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<struct SAccessorSparseValues>& _pData, const WCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SAccessorSparseValues>>& _pDatas, const WCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<struct SAnimationChannelTarget>& _pData, const WCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SAnimationChannelTarget>>& _pDatas, const WCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<struct SMesh>& _pData, const WCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SMesh>>& _pDatas, const WCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<struct SAccessorSparse>& _pData, const WCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SAccessorSparse>>& _pDatas, const WCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<struct SMeshPrimitive>& _pData, const WCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SMeshPrimitive>>& _pDatas, const WCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<struct SExtension>& _pData, const WCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SExtension>>& _pDatas, const WCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<struct SAnimationChannel>& _pData, const WCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SAnimationChannel>>& _pDatas, const WCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<struct SAccessorSparseIndices>& _pData, const WCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SAccessorSparseIndices>>& _pDatas, const WCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<struct SNode>& _pData, const WCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SNode>>& _pDatas, const WCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<struct SAnimation>& _pData, const WCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SAnimation>>& _pDatas, const WCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<struct SSkin>& _pData, const WCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SSkin>>& _pDatas, const WCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<struct SMaterialPBRMetallicRoughness>& _pData, const WCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SMaterialPBRMetallicRoughness>>& _pDatas, const WCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<struct SCamera>& _pData, const WCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SCamera>>& _pDatas, const WCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<struct SImage>& _pData, const WCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SImage>>& _pDatas, const WCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<struct STexture>& _pData, const WCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct STexture>>& _pDatas, const WCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<struct SCameraOrthographic>& _pData, const WCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SCameraOrthographic>>& _pDatas, const WCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<struct SBuffer>& _pData, const WCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SBuffer>>& _pDatas, const WCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<struct SAccessor>& _pData, const WCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SAccessor>>& _pDatas, const WCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<struct SGlTF>& _pData, const WCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SGlTF>>& _pDatas, const WCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<struct STextureInfo>& _pData, const WCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct STextureInfo>>& _pDatas, const WCharValue& _JsonValue);

}

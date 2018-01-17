#pragma once

#include "libgltfpch.h"

#include <memory>
#include <vector>

namespace libgltf
{
    bool operator<<(std::shared_ptr<struct SGlTFProperty>& _pData, const GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SGlTFProperty>>& _pDatas, const GLTFCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<struct SMaterial>& _pData, const GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SMaterial>>& _pDatas, const GLTFCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<struct SAsset>& _pData, const GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SAsset>>& _pDatas, const GLTFCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<struct SSampler>& _pData, const GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SSampler>>& _pDatas, const GLTFCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<struct SGlTFChildofRootProperty>& _pData, const GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SGlTFChildofRootProperty>>& _pDatas, const GLTFCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<struct SAnimationSampler>& _pData, const GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SAnimationSampler>>& _pDatas, const GLTFCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<struct SExtras>& _pData, const GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SExtras>>& _pDatas, const GLTFCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<struct SScene>& _pData, const GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SScene>>& _pDatas, const GLTFCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<struct SCameraPerspective>& _pData, const GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SCameraPerspective>>& _pDatas, const GLTFCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<struct SBufferView>& _pData, const GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SBufferView>>& _pDatas, const GLTFCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<struct SMaterialNormalTextureInfo>& _pData, const GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SMaterialNormalTextureInfo>>& _pDatas, const GLTFCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<struct SMaterialOcclusionTextureInfo>& _pData, const GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SMaterialOcclusionTextureInfo>>& _pDatas, const GLTFCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<struct SAccessorSparseValues>& _pData, const GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SAccessorSparseValues>>& _pDatas, const GLTFCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<struct SAnimationChannelTarget>& _pData, const GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SAnimationChannelTarget>>& _pDatas, const GLTFCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<struct SMesh>& _pData, const GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SMesh>>& _pDatas, const GLTFCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<struct SAccessorSparse>& _pData, const GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SAccessorSparse>>& _pDatas, const GLTFCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<struct SMeshPrimitive>& _pData, const GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SMeshPrimitive>>& _pDatas, const GLTFCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<struct SExtension>& _pData, const GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SExtension>>& _pDatas, const GLTFCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<struct SAnimationChannel>& _pData, const GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SAnimationChannel>>& _pDatas, const GLTFCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<struct SGlTFId>& _pData, const GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SGlTFId>>& _pDatas, const GLTFCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<struct SAccessorSparseIndices>& _pData, const GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SAccessorSparseIndices>>& _pDatas, const GLTFCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<struct SNode>& _pData, const GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SNode>>& _pDatas, const GLTFCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<struct SAnimation>& _pData, const GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SAnimation>>& _pDatas, const GLTFCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<struct SSkin>& _pData, const GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SSkin>>& _pDatas, const GLTFCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<struct SMaterialPBRMetallicRoughness>& _pData, const GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SMaterialPBRMetallicRoughness>>& _pDatas, const GLTFCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<struct SCamera>& _pData, const GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SCamera>>& _pDatas, const GLTFCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<struct SImage>& _pData, const GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SImage>>& _pDatas, const GLTFCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<struct STexture>& _pData, const GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct STexture>>& _pDatas, const GLTFCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<struct SCameraOrthographic>& _pData, const GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SCameraOrthographic>>& _pDatas, const GLTFCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<struct SBuffer>& _pData, const GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SBuffer>>& _pDatas, const GLTFCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<struct SAccessor>& _pData, const GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SAccessor>>& _pDatas, const GLTFCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<struct SGlTF>& _pData, const GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SGlTF>>& _pDatas, const GLTFCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<struct STextureInfo>& _pData, const GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct STextureInfo>>& _pDatas, const GLTFCharValue& _JsonValue);

}

#pragma once

#include "libgltfpch.h"
#include "libgltf.h"

namespace libgltf
{
    bool operator<<(std::shared_ptr<SGlTFProperty> _pData, const WCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SGlTFProperty>> _pDatas, const WCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<SMaterial> _pData, const WCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SMaterial>> _pDatas, const WCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<SAsset> _pData, const WCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SAsset>> _pDatas, const WCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<SSampler> _pData, const WCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SSampler>> _pDatas, const WCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<SAnimationSampler> _pData, const WCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SAnimationSampler>> _pDatas, const WCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<SExtras> _pData, const WCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SExtras>> _pDatas, const WCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<SScene> _pData, const WCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SScene>> _pDatas, const WCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<SCameraPerspective> _pData, const WCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SCameraPerspective>> _pDatas, const WCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<SBufferView> _pData, const WCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SBufferView>> _pDatas, const WCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<SMaterialNormalTextureInfo> _pData, const WCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SMaterialNormalTextureInfo>> _pDatas, const WCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<SMaterialOcclusionTextureInfo> _pData, const WCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SMaterialOcclusionTextureInfo>> _pDatas, const WCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<SAccessorSparseValues> _pData, const WCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SAccessorSparseValues>> _pDatas, const WCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<SAnimationChannelTarget> _pData, const WCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SAnimationChannelTarget>> _pDatas, const WCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<SMesh> _pData, const WCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SMesh>> _pDatas, const WCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<SAccessorSparse> _pData, const WCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SAccessorSparse>> _pDatas, const WCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<SMeshPrimitive> _pData, const WCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SMeshPrimitive>> _pDatas, const WCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<SExtension> _pData, const WCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SExtension>> _pDatas, const WCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<SAnimationChannel> _pData, const WCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SAnimationChannel>> _pDatas, const WCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<SAccessorSparseIndices> _pData, const WCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SAccessorSparseIndices>> _pDatas, const WCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<SNode> _pData, const WCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SNode>> _pDatas, const WCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<SAnimation> _pData, const WCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SAnimation>> _pDatas, const WCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<SSkin> _pData, const WCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SSkin>> _pDatas, const WCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<SMaterialPBRMetallicRoughness> _pData, const WCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SMaterialPBRMetallicRoughness>> _pDatas, const WCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<SCamera> _pData, const WCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SCamera>> _pDatas, const WCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<SImage> _pData, const WCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SImage>> _pDatas, const WCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<STexture> _pData, const WCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<STexture>> _pDatas, const WCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<SCameraOrthographic> _pData, const WCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SCameraOrthographic>> _pDatas, const WCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<SBuffer> _pData, const WCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SBuffer>> _pDatas, const WCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<SAccessor> _pData, const WCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SAccessor>> _pDatas, const WCharValue& _JsonValue);

    bool operator<<(std::shared_ptr<SGlTF> _pData, const WCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SGlTF>> _pDatas, const WCharValue& _JsonValue);

}

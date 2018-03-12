#pragma once

#include "libgltfpch.h"

#include <memory>
#include <vector>

namespace libgltf
{
    bool operator<<(struct SGlTFProperty& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const struct SGlTFProperty& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<struct SGlTFProperty>& _pData, const GLTFCharValue& _JsonValue);
    bool operator>>(const std::shared_ptr<struct SGlTFProperty>& _pData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SGlTFProperty>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const std::vector<std::shared_ptr<struct SGlTFProperty>>& _vDatas, GLTFCharValue& _JsonValue);

    bool operator<<(struct SMaterial& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const struct SMaterial& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<struct SMaterial>& _pData, const GLTFCharValue& _JsonValue);
    bool operator>>(const std::shared_ptr<struct SMaterial>& _pData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SMaterial>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const std::vector<std::shared_ptr<struct SMaterial>>& _vDatas, GLTFCharValue& _JsonValue);

    bool operator<<(struct SAsset& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const struct SAsset& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<struct SAsset>& _pData, const GLTFCharValue& _JsonValue);
    bool operator>>(const std::shared_ptr<struct SAsset>& _pData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SAsset>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const std::vector<std::shared_ptr<struct SAsset>>& _vDatas, GLTFCharValue& _JsonValue);

    bool operator<<(struct SSampler& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const struct SSampler& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<struct SSampler>& _pData, const GLTFCharValue& _JsonValue);
    bool operator>>(const std::shared_ptr<struct SSampler>& _pData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SSampler>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const std::vector<std::shared_ptr<struct SSampler>>& _vDatas, GLTFCharValue& _JsonValue);

    bool operator<<(struct SGlTFChildofRootProperty& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const struct SGlTFChildofRootProperty& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<struct SGlTFChildofRootProperty>& _pData, const GLTFCharValue& _JsonValue);
    bool operator>>(const std::shared_ptr<struct SGlTFChildofRootProperty>& _pData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SGlTFChildofRootProperty>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const std::vector<std::shared_ptr<struct SGlTFChildofRootProperty>>& _vDatas, GLTFCharValue& _JsonValue);

    bool operator<<(struct SAnimationSampler& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const struct SAnimationSampler& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<struct SAnimationSampler>& _pData, const GLTFCharValue& _JsonValue);
    bool operator>>(const std::shared_ptr<struct SAnimationSampler>& _pData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SAnimationSampler>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const std::vector<std::shared_ptr<struct SAnimationSampler>>& _vDatas, GLTFCharValue& _JsonValue);

    bool operator<<(struct SExtras& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const struct SExtras& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<struct SExtras>& _pData, const GLTFCharValue& _JsonValue);
    bool operator>>(const std::shared_ptr<struct SExtras>& _pData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SExtras>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const std::vector<std::shared_ptr<struct SExtras>>& _vDatas, GLTFCharValue& _JsonValue);

    bool operator<<(struct SScene& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const struct SScene& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<struct SScene>& _pData, const GLTFCharValue& _JsonValue);
    bool operator>>(const std::shared_ptr<struct SScene>& _pData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SScene>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const std::vector<std::shared_ptr<struct SScene>>& _vDatas, GLTFCharValue& _JsonValue);

    bool operator<<(struct SCameraPerspective& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const struct SCameraPerspective& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<struct SCameraPerspective>& _pData, const GLTFCharValue& _JsonValue);
    bool operator>>(const std::shared_ptr<struct SCameraPerspective>& _pData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SCameraPerspective>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const std::vector<std::shared_ptr<struct SCameraPerspective>>& _vDatas, GLTFCharValue& _JsonValue);

    bool operator<<(struct SBufferView& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const struct SBufferView& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<struct SBufferView>& _pData, const GLTFCharValue& _JsonValue);
    bool operator>>(const std::shared_ptr<struct SBufferView>& _pData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SBufferView>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const std::vector<std::shared_ptr<struct SBufferView>>& _vDatas, GLTFCharValue& _JsonValue);

    bool operator<<(struct SMaterialNormalTextureInfo& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const struct SMaterialNormalTextureInfo& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<struct SMaterialNormalTextureInfo>& _pData, const GLTFCharValue& _JsonValue);
    bool operator>>(const std::shared_ptr<struct SMaterialNormalTextureInfo>& _pData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SMaterialNormalTextureInfo>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const std::vector<std::shared_ptr<struct SMaterialNormalTextureInfo>>& _vDatas, GLTFCharValue& _JsonValue);

    bool operator<<(struct SMaterialOcclusionTextureInfo& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const struct SMaterialOcclusionTextureInfo& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<struct SMaterialOcclusionTextureInfo>& _pData, const GLTFCharValue& _JsonValue);
    bool operator>>(const std::shared_ptr<struct SMaterialOcclusionTextureInfo>& _pData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SMaterialOcclusionTextureInfo>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const std::vector<std::shared_ptr<struct SMaterialOcclusionTextureInfo>>& _vDatas, GLTFCharValue& _JsonValue);

    bool operator<<(struct SAccessorSparseValues& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const struct SAccessorSparseValues& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<struct SAccessorSparseValues>& _pData, const GLTFCharValue& _JsonValue);
    bool operator>>(const std::shared_ptr<struct SAccessorSparseValues>& _pData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SAccessorSparseValues>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const std::vector<std::shared_ptr<struct SAccessorSparseValues>>& _vDatas, GLTFCharValue& _JsonValue);

    bool operator<<(struct SAnimationChannelTarget& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const struct SAnimationChannelTarget& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<struct SAnimationChannelTarget>& _pData, const GLTFCharValue& _JsonValue);
    bool operator>>(const std::shared_ptr<struct SAnimationChannelTarget>& _pData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SAnimationChannelTarget>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const std::vector<std::shared_ptr<struct SAnimationChannelTarget>>& _vDatas, GLTFCharValue& _JsonValue);

    bool operator<<(struct SMesh& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const struct SMesh& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<struct SMesh>& _pData, const GLTFCharValue& _JsonValue);
    bool operator>>(const std::shared_ptr<struct SMesh>& _pData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SMesh>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const std::vector<std::shared_ptr<struct SMesh>>& _vDatas, GLTFCharValue& _JsonValue);

    bool operator<<(struct SAccessorSparse& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const struct SAccessorSparse& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<struct SAccessorSparse>& _pData, const GLTFCharValue& _JsonValue);
    bool operator>>(const std::shared_ptr<struct SAccessorSparse>& _pData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SAccessorSparse>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const std::vector<std::shared_ptr<struct SAccessorSparse>>& _vDatas, GLTFCharValue& _JsonValue);

    bool operator<<(struct SMeshPrimitive& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const struct SMeshPrimitive& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<struct SMeshPrimitive>& _pData, const GLTFCharValue& _JsonValue);
    bool operator>>(const std::shared_ptr<struct SMeshPrimitive>& _pData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SMeshPrimitive>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const std::vector<std::shared_ptr<struct SMeshPrimitive>>& _vDatas, GLTFCharValue& _JsonValue);

    bool operator<<(struct SKHR_materials_pbrSpecularGlossinessglTFextension& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const struct SKHR_materials_pbrSpecularGlossinessglTFextension& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<struct SKHR_materials_pbrSpecularGlossinessglTFextension>& _pData, const GLTFCharValue& _JsonValue);
    bool operator>>(const std::shared_ptr<struct SKHR_materials_pbrSpecularGlossinessglTFextension>& _pData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SKHR_materials_pbrSpecularGlossinessglTFextension>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const std::vector<std::shared_ptr<struct SKHR_materials_pbrSpecularGlossinessglTFextension>>& _vDatas, GLTFCharValue& _JsonValue);

    bool operator<<(struct SExtension& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const struct SExtension& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<struct SExtension>& _pData, const GLTFCharValue& _JsonValue);
    bool operator>>(const std::shared_ptr<struct SExtension>& _pData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SExtension>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const std::vector<std::shared_ptr<struct SExtension>>& _vDatas, GLTFCharValue& _JsonValue);

    bool operator<<(struct SAnimationChannel& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const struct SAnimationChannel& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<struct SAnimationChannel>& _pData, const GLTFCharValue& _JsonValue);
    bool operator>>(const std::shared_ptr<struct SAnimationChannel>& _pData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SAnimationChannel>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const std::vector<std::shared_ptr<struct SAnimationChannel>>& _vDatas, GLTFCharValue& _JsonValue);

    bool operator<<(struct SGlTFId& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const struct SGlTFId& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<struct SGlTFId>& _pData, const GLTFCharValue& _JsonValue);
    bool operator>>(const std::shared_ptr<struct SGlTFId>& _pData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SGlTFId>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const std::vector<std::shared_ptr<struct SGlTFId>>& _vDatas, GLTFCharValue& _JsonValue);

    bool operator<<(struct SAccessorSparseIndices& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const struct SAccessorSparseIndices& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<struct SAccessorSparseIndices>& _pData, const GLTFCharValue& _JsonValue);
    bool operator>>(const std::shared_ptr<struct SAccessorSparseIndices>& _pData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SAccessorSparseIndices>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const std::vector<std::shared_ptr<struct SAccessorSparseIndices>>& _vDatas, GLTFCharValue& _JsonValue);

    bool operator<<(struct SNode& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const struct SNode& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<struct SNode>& _pData, const GLTFCharValue& _JsonValue);
    bool operator>>(const std::shared_ptr<struct SNode>& _pData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SNode>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const std::vector<std::shared_ptr<struct SNode>>& _vDatas, GLTFCharValue& _JsonValue);

    bool operator<<(struct SAnimation& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const struct SAnimation& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<struct SAnimation>& _pData, const GLTFCharValue& _JsonValue);
    bool operator>>(const std::shared_ptr<struct SAnimation>& _pData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SAnimation>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const std::vector<std::shared_ptr<struct SAnimation>>& _vDatas, GLTFCharValue& _JsonValue);

    bool operator<<(struct SSkin& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const struct SSkin& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<struct SSkin>& _pData, const GLTFCharValue& _JsonValue);
    bool operator>>(const std::shared_ptr<struct SSkin>& _pData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SSkin>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const std::vector<std::shared_ptr<struct SSkin>>& _vDatas, GLTFCharValue& _JsonValue);

    bool operator<<(struct SMaterialPBRMetallicRoughness& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const struct SMaterialPBRMetallicRoughness& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<struct SMaterialPBRMetallicRoughness>& _pData, const GLTFCharValue& _JsonValue);
    bool operator>>(const std::shared_ptr<struct SMaterialPBRMetallicRoughness>& _pData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SMaterialPBRMetallicRoughness>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const std::vector<std::shared_ptr<struct SMaterialPBRMetallicRoughness>>& _vDatas, GLTFCharValue& _JsonValue);

    bool operator<<(struct SKHR_draco_mesh_compressionextension& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const struct SKHR_draco_mesh_compressionextension& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<struct SKHR_draco_mesh_compressionextension>& _pData, const GLTFCharValue& _JsonValue);
    bool operator>>(const std::shared_ptr<struct SKHR_draco_mesh_compressionextension>& _pData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SKHR_draco_mesh_compressionextension>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const std::vector<std::shared_ptr<struct SKHR_draco_mesh_compressionextension>>& _vDatas, GLTFCharValue& _JsonValue);

    bool operator<<(struct SCamera& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const struct SCamera& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<struct SCamera>& _pData, const GLTFCharValue& _JsonValue);
    bool operator>>(const std::shared_ptr<struct SCamera>& _pData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SCamera>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const std::vector<std::shared_ptr<struct SCamera>>& _vDatas, GLTFCharValue& _JsonValue);

    bool operator<<(struct SImage& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const struct SImage& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<struct SImage>& _pData, const GLTFCharValue& _JsonValue);
    bool operator>>(const std::shared_ptr<struct SImage>& _pData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SImage>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const std::vector<std::shared_ptr<struct SImage>>& _vDatas, GLTFCharValue& _JsonValue);

    bool operator<<(struct STexture& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const struct STexture& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<struct STexture>& _pData, const GLTFCharValue& _JsonValue);
    bool operator>>(const std::shared_ptr<struct STexture>& _pData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct STexture>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const std::vector<std::shared_ptr<struct STexture>>& _vDatas, GLTFCharValue& _JsonValue);

    bool operator<<(struct SCameraOrthographic& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const struct SCameraOrthographic& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<struct SCameraOrthographic>& _pData, const GLTFCharValue& _JsonValue);
    bool operator>>(const std::shared_ptr<struct SCameraOrthographic>& _pData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SCameraOrthographic>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const std::vector<std::shared_ptr<struct SCameraOrthographic>>& _vDatas, GLTFCharValue& _JsonValue);

    bool operator<<(struct SBuffer& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const struct SBuffer& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<struct SBuffer>& _pData, const GLTFCharValue& _JsonValue);
    bool operator>>(const std::shared_ptr<struct SBuffer>& _pData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SBuffer>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const std::vector<std::shared_ptr<struct SBuffer>>& _vDatas, GLTFCharValue& _JsonValue);

    bool operator<<(struct SAccessor& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const struct SAccessor& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<struct SAccessor>& _pData, const GLTFCharValue& _JsonValue);
    bool operator>>(const std::shared_ptr<struct SAccessor>& _pData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SAccessor>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const std::vector<std::shared_ptr<struct SAccessor>>& _vDatas, GLTFCharValue& _JsonValue);

    bool operator<<(struct SGlTF& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const struct SGlTF& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<struct SGlTF>& _pData, const GLTFCharValue& _JsonValue);
    bool operator>>(const std::shared_ptr<struct SGlTF>& _pData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SGlTF>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const std::vector<std::shared_ptr<struct SGlTF>>& _vDatas, GLTFCharValue& _JsonValue);

    bool operator<<(struct STextureInfo& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const struct STextureInfo& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<struct STextureInfo>& _pData, const GLTFCharValue& _JsonValue);
    bool operator>>(const std::shared_ptr<struct STextureInfo>& _pData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct STextureInfo>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const std::vector<std::shared_ptr<struct STextureInfo>>& _vDatas, GLTFCharValue& _JsonValue);

}

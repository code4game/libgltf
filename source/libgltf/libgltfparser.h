/*
 * This software is released under the MIT license.
 *
 * Copyright (c) 2017-2020 Alex Chi, The Code 4 Game Organization
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is furnished to do
 * so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once

#include "libgltfpch.h"
#include "libgltf/libgltf.h"

#include <memory>
#include <vector>

namespace libgltf
{
    template<typename TData>
    class TDataDoc
    {
    public:
        explicit TDataDoc(const TData& _rData, GLTFCharDocument* _pDoc)
            : data(_rData)
            , doc(_pDoc)
        {
            //
        }
        const TData& data;
        GLTFCharDocument* doc;
    };

    bool operator<<(SAccessor& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SAccessor>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SAccessor>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SAccessor>>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SAccessor>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SAccessor>>>& _rData, GLTFCharValue& _JsonValue);

    bool operator<<(SAccessorSparseIndices& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SAccessorSparseIndices>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SAccessorSparseIndices>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SAccessorSparseIndices>>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SAccessorSparseIndices>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SAccessorSparseIndices>>>& _rData, GLTFCharValue& _JsonValue);

    bool operator<<(SAccessorSparse& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SAccessorSparse>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SAccessorSparse>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SAccessorSparse>>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SAccessorSparse>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SAccessorSparse>>>& _rData, GLTFCharValue& _JsonValue);

    bool operator<<(SAccessorSparseValues& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SAccessorSparseValues>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SAccessorSparseValues>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SAccessorSparseValues>>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SAccessorSparseValues>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SAccessorSparseValues>>>& _rData, GLTFCharValue& _JsonValue);

    bool operator<<(SAnimationChannel& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SAnimationChannel>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SAnimationChannel>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SAnimationChannel>>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SAnimationChannel>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SAnimationChannel>>>& _rData, GLTFCharValue& _JsonValue);

    bool operator<<(SAnimationChannelTarget& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SAnimationChannelTarget>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SAnimationChannelTarget>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SAnimationChannelTarget>>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SAnimationChannelTarget>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SAnimationChannelTarget>>>& _rData, GLTFCharValue& _JsonValue);

    bool operator<<(SAnimationSampler& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SAnimationSampler>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SAnimationSampler>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SAnimationSampler>>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SAnimationSampler>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SAnimationSampler>>>& _rData, GLTFCharValue& _JsonValue);

    bool operator<<(SAnimation& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SAnimation>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SAnimation>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SAnimation>>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SAnimation>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SAnimation>>>& _rData, GLTFCharValue& _JsonValue);

    bool operator<<(SAsset& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SAsset>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SAsset>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SAsset>>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SAsset>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SAsset>>>& _rData, GLTFCharValue& _JsonValue);

    bool operator<<(SBuffer& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SBuffer>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SBuffer>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SBuffer>>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SBuffer>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SBuffer>>>& _rData, GLTFCharValue& _JsonValue);

    bool operator<<(SBufferView& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SBufferView>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SBufferView>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SBufferView>>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SBufferView>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SBufferView>>>& _rData, GLTFCharValue& _JsonValue);

    bool operator<<(SCameraOrthographic& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SCameraOrthographic>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SCameraOrthographic>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SCameraOrthographic>>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SCameraOrthographic>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SCameraOrthographic>>>& _rData, GLTFCharValue& _JsonValue);

    bool operator<<(SCameraPerspective& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SCameraPerspective>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SCameraPerspective>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SCameraPerspective>>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SCameraPerspective>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SCameraPerspective>>>& _rData, GLTFCharValue& _JsonValue);

    bool operator<<(SCamera& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SCamera>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SCamera>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SCamera>>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SCamera>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SCamera>>>& _rData, GLTFCharValue& _JsonValue);

    bool operator<<(SExtension& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SExtension>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SExtension>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SExtension>>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SExtension>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SExtension>>>& _rData, GLTFCharValue& _JsonValue);

    bool operator<<(SExtras& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SExtras>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SExtras>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SExtras>>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SExtras>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SExtras>>>& _rData, GLTFCharValue& _JsonValue);

    bool operator<<(SGlTF& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SGlTF>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SGlTF>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SGlTF>>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SGlTF>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SGlTF>>>& _rData, GLTFCharValue& _JsonValue);

    bool operator<<(SGlTFChildofRootProperty& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SGlTFChildofRootProperty>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SGlTFChildofRootProperty>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SGlTFChildofRootProperty>>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SGlTFChildofRootProperty>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SGlTFChildofRootProperty>>>& _rData, GLTFCharValue& _JsonValue);

    bool operator<<(SGlTFId& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SGlTFId>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SGlTFId>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SGlTFId>>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SGlTFId>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SGlTFId>>>& _rData, GLTFCharValue& _JsonValue);

    bool operator<<(SGlTFProperty& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SGlTFProperty>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SGlTFProperty>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SGlTFProperty>>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SGlTFProperty>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SGlTFProperty>>>& _rData, GLTFCharValue& _JsonValue);

    bool operator<<(SImage& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SImage>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SImage>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SImage>>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SImage>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SImage>>>& _rData, GLTFCharValue& _JsonValue);

    bool operator<<(SMaterialNormalTextureInfo& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SMaterialNormalTextureInfo>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SMaterialNormalTextureInfo>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SMaterialNormalTextureInfo>>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SMaterialNormalTextureInfo>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SMaterialNormalTextureInfo>>>& _rData, GLTFCharValue& _JsonValue);

    bool operator<<(SMaterialOcclusionTextureInfo& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SMaterialOcclusionTextureInfo>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SMaterialOcclusionTextureInfo>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SMaterialOcclusionTextureInfo>>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SMaterialOcclusionTextureInfo>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SMaterialOcclusionTextureInfo>>>& _rData, GLTFCharValue& _JsonValue);

    bool operator<<(SMaterialPBRMetallicRoughness& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SMaterialPBRMetallicRoughness>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SMaterialPBRMetallicRoughness>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SMaterialPBRMetallicRoughness>>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SMaterialPBRMetallicRoughness>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SMaterialPBRMetallicRoughness>>>& _rData, GLTFCharValue& _JsonValue);

    bool operator<<(SMaterial& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SMaterial>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SMaterial>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SMaterial>>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SMaterial>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SMaterial>>>& _rData, GLTFCharValue& _JsonValue);

    bool operator<<(SMeshPrimitive& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SMeshPrimitive>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SMeshPrimitive>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SMeshPrimitive>>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SMeshPrimitive>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SMeshPrimitive>>>& _rData, GLTFCharValue& _JsonValue);

    bool operator<<(SMesh& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SMesh>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SMesh>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SMesh>>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SMesh>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SMesh>>>& _rData, GLTFCharValue& _JsonValue);

    bool operator<<(SNode& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SNode>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SNode>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SNode>>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SNode>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SNode>>>& _rData, GLTFCharValue& _JsonValue);

    bool operator<<(SSampler& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SSampler>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SSampler>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SSampler>>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SSampler>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SSampler>>>& _rData, GLTFCharValue& _JsonValue);

    bool operator<<(SScene& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SScene>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SScene>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SScene>>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SScene>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SScene>>>& _rData, GLTFCharValue& _JsonValue);

    bool operator<<(SSkin& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SSkin>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SSkin>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SSkin>>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SSkin>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SSkin>>>& _rData, GLTFCharValue& _JsonValue);

    bool operator<<(STexture& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<STexture>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<STexture>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<STexture>>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<STexture>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<STexture>>>& _rData, GLTFCharValue& _JsonValue);

    bool operator<<(STextureInfo& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<STextureInfo>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<STextureInfo>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<STextureInfo>>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<STextureInfo>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<STextureInfo>>>& _rData, GLTFCharValue& _JsonValue);

    bool operator<<(SKHR_draco_mesh_compressionextension& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SKHR_draco_mesh_compressionextension>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SKHR_draco_mesh_compressionextension>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SKHR_draco_mesh_compressionextension>>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SKHR_draco_mesh_compressionextension>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SKHR_draco_mesh_compressionextension>>>& _rData, GLTFCharValue& _JsonValue);

    bool operator<<(SKHR_lights_punctualglTFextension& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SKHR_lights_punctualglTFextension>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SKHR_lights_punctualglTFextension>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SKHR_lights_punctualglTFextension>>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SKHR_lights_punctualglTFextension>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SKHR_lights_punctualglTFextension>>>& _rData, GLTFCharValue& _JsonValue);

    bool operator<<(SLight& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SLight>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SLight>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SLight>>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SLight>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SLight>>>& _rData, GLTFCharValue& _JsonValue);

    bool operator<<(SLightspot& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SLightspot>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SLightspot>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SLightspot>>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SLightspot>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SLightspot>>>& _rData, GLTFCharValue& _JsonValue);

    bool operator<<(SKHR_lights_punctualnodeextension& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SKHR_lights_punctualnodeextension>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SKHR_lights_punctualnodeextension>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SKHR_lights_punctualnodeextension>>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SKHR_lights_punctualnodeextension>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SKHR_lights_punctualnodeextension>>>& _rData, GLTFCharValue& _JsonValue);

    bool operator<<(SKHR_materials_clearcoatglTFextension& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SKHR_materials_clearcoatglTFextension>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SKHR_materials_clearcoatglTFextension>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SKHR_materials_clearcoatglTFextension>>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SKHR_materials_clearcoatglTFextension>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SKHR_materials_clearcoatglTFextension>>>& _rData, GLTFCharValue& _JsonValue);

    bool operator<<(SKHR_materials_pbrSpecularGlossinessglTFextension& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SKHR_materials_pbrSpecularGlossinessglTFextension>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SKHR_materials_pbrSpecularGlossinessglTFextension>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SKHR_materials_pbrSpecularGlossinessglTFextension>>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SKHR_materials_pbrSpecularGlossinessglTFextension>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SKHR_materials_pbrSpecularGlossinessglTFextension>>>& _rData, GLTFCharValue& _JsonValue);

    bool operator<<(SKHR_materials_unlitglTFextension& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SKHR_materials_unlitglTFextension>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SKHR_materials_unlitglTFextension>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SKHR_materials_unlitglTFextension>>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SKHR_materials_unlitglTFextension>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SKHR_materials_unlitglTFextension>>>& _rData, GLTFCharValue& _JsonValue);

    bool operator<<(SKHR_techniques_webglglTFextension& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SKHR_techniques_webglglTFextension>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SKHR_techniques_webglglTFextension>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SKHR_techniques_webglglTFextension>>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SKHR_techniques_webglglTFextension>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SKHR_techniques_webglglTFextension>>>& _rData, GLTFCharValue& _JsonValue);

    bool operator<<(SKHR_techniques_webglmaterialextension& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SKHR_techniques_webglmaterialextension>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SKHR_techniques_webglmaterialextension>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SKHR_techniques_webglmaterialextension>>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SKHR_techniques_webglmaterialextension>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SKHR_techniques_webglmaterialextension>>>& _rData, GLTFCharValue& _JsonValue);

    bool operator<<(SProgram& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SProgram>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SProgram>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SProgram>>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SProgram>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SProgram>>>& _rData, GLTFCharValue& _JsonValue);

    bool operator<<(SShader& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SShader>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SShader>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SShader>>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SShader>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SShader>>>& _rData, GLTFCharValue& _JsonValue);

    bool operator<<(SAttribute& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SAttribute>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SAttribute>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SAttribute>>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SAttribute>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SAttribute>>>& _rData, GLTFCharValue& _JsonValue);

    bool operator<<(STechnique& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<STechnique>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<STechnique>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<STechnique>>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<STechnique>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<STechnique>>>& _rData, GLTFCharValue& _JsonValue);

    bool operator<<(SUniform& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SUniform>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SUniform>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SUniform>>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SUniform>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SUniform>>>& _rData, GLTFCharValue& _JsonValue);

    bool operator<<(SUniformValue& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SUniformValue>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SUniformValue>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SUniformValue>>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SUniformValue>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SUniformValue>>>& _rData, GLTFCharValue& _JsonValue);

    bool operator<<(SKHR_texture_transformtextureInfoextension& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SKHR_texture_transformtextureInfoextension>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SKHR_texture_transformtextureInfoextension>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SKHR_texture_transformtextureInfoextension>>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SKHR_texture_transformtextureInfoextension>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SKHR_texture_transformtextureInfoextension>>>& _rData, GLTFCharValue& _JsonValue);

    bool operator<<(SADOBE_materials_thin_transparencyglTFextension& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SADOBE_materials_thin_transparencyglTFextension>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SADOBE_materials_thin_transparencyglTFextension>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SADOBE_materials_thin_transparencyglTFextension>>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SADOBE_materials_thin_transparencyglTFextension>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SADOBE_materials_thin_transparencyglTFextension>>>& _rData, GLTFCharValue& _JsonValue);

    bool operator<<(SArticulation& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SArticulation>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SArticulation>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SArticulation>>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SArticulation>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SArticulation>>>& _rData, GLTFCharValue& _JsonValue);

    bool operator<<(SArticulationStage& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SArticulationStage>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SArticulationStage>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SArticulationStage>>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SArticulationStage>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SArticulationStage>>>& _rData, GLTFCharValue& _JsonValue);

    bool operator<<(SAGI_articulationsglTFextension& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SAGI_articulationsglTFextension>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SAGI_articulationsglTFextension>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SAGI_articulationsglTFextension>>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SAGI_articulationsglTFextension>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SAGI_articulationsglTFextension>>>& _rData, GLTFCharValue& _JsonValue);

    bool operator<<(SAGI_articulationsglTFNodeextension& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SAGI_articulationsglTFNodeextension>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SAGI_articulationsglTFNodeextension>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SAGI_articulationsglTFNodeextension>>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SAGI_articulationsglTFNodeextension>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SAGI_articulationsglTFNodeextension>>>& _rData, GLTFCharValue& _JsonValue);

    bool operator<<(SAGI_stk_metadataglTFextension& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SAGI_stk_metadataglTFextension>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SAGI_stk_metadataglTFextension>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SAGI_stk_metadataglTFextension>>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SAGI_stk_metadataglTFextension>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SAGI_stk_metadataglTFextension>>>& _rData, GLTFCharValue& _JsonValue);

    bool operator<<(SAGI_stk_metadataglTFNodeextension& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SAGI_stk_metadataglTFNodeextension>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SAGI_stk_metadataglTFNodeextension>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SAGI_stk_metadataglTFNodeextension>>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SAGI_stk_metadataglTFNodeextension>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SAGI_stk_metadataglTFNodeextension>>>& _rData, GLTFCharValue& _JsonValue);

    bool operator<<(SSolarPanelGroup& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SSolarPanelGroup>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SSolarPanelGroup>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SSolarPanelGroup>>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SSolarPanelGroup>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SSolarPanelGroup>>>& _rData, GLTFCharValue& _JsonValue);

    bool operator<<(SCESIUM_primitive_outlineglTFprimitiveextension& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SCESIUM_primitive_outlineglTFprimitiveextension>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SCESIUM_primitive_outlineglTFprimitiveextension>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SCESIUM_primitive_outlineglTFprimitiveextension>>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SCESIUM_primitive_outlineglTFprimitiveextension>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SCESIUM_primitive_outlineglTFprimitiveextension>>>& _rData, GLTFCharValue& _JsonValue);

    bool operator<<(SEXT_mesh_gpu_instancingglTFextension& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SEXT_mesh_gpu_instancingglTFextension>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SEXT_mesh_gpu_instancingglTFextension>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SEXT_mesh_gpu_instancingglTFextension>>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SEXT_mesh_gpu_instancingglTFextension>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SEXT_mesh_gpu_instancingglTFextension>>>& _rData, GLTFCharValue& _JsonValue);

    bool operator<<(SEXT_texture_webpglTFextension& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SEXT_texture_webpglTFextension>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SEXT_texture_webpglTFextension>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SEXT_texture_webpglTFextension>>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SEXT_texture_webpglTFextension>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SEXT_texture_webpglTFextension>>>& _rData, GLTFCharValue& _JsonValue);

    bool operator<<(SMSFT_lodglTFextension& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SMSFT_lodglTFextension>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SMSFT_lodglTFextension>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SMSFT_lodglTFextension>>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SMSFT_lodglTFextension>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SMSFT_lodglTFextension>>>& _rData, GLTFCharValue& _JsonValue);

    bool operator<<(SMSFT_texture_ddsextension& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SMSFT_texture_ddsextension>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SMSFT_texture_ddsextension>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SMSFT_texture_ddsextension>>& _rData, GLTFCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SMSFT_texture_ddsextension>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SMSFT_texture_ddsextension>>>& _rData, GLTFCharValue& _JsonValue);

}

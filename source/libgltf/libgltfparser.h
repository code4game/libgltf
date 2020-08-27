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

    bool operator<<(struct SKHR_techniques_webglmaterialextension& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<struct SKHR_techniques_webglmaterialextension>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::shared_ptr<struct SKHR_techniques_webglmaterialextension>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<struct SKHR_techniques_webglmaterialextension>>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SKHR_techniques_webglmaterialextension>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<struct SKHR_techniques_webglmaterialextension>>>& _rData, GLTFCharValue& const _JsonValue);

    bool operator<<(struct SGlTFProperty& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<struct SGlTFProperty>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::shared_ptr<struct SGlTFProperty>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<struct SGlTFProperty>>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SGlTFProperty>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<struct SGlTFProperty>>>& _rData, GLTFCharValue& const _JsonValue);

    bool operator<<(struct SMaterial& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<struct SMaterial>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::shared_ptr<struct SMaterial>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<struct SMaterial>>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SMaterial>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<struct SMaterial>>>& _rData, GLTFCharValue& const _JsonValue);

    bool operator<<(struct SKHR_texture_transformtextureInfoextension& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<struct SKHR_texture_transformtextureInfoextension>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::shared_ptr<struct SKHR_texture_transformtextureInfoextension>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<struct SKHR_texture_transformtextureInfoextension>>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SKHR_texture_transformtextureInfoextension>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<struct SKHR_texture_transformtextureInfoextension>>>& _rData, GLTFCharValue& const _JsonValue);

    bool operator<<(struct SAsset& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<struct SAsset>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::shared_ptr<struct SAsset>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<struct SAsset>>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SAsset>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<struct SAsset>>>& _rData, GLTFCharValue& const _JsonValue);

    bool operator<<(struct SSampler& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<struct SSampler>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::shared_ptr<struct SSampler>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<struct SSampler>>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SSampler>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<struct SSampler>>>& _rData, GLTFCharValue& const _JsonValue);

    bool operator<<(struct SGlTFChildofRootProperty& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<struct SGlTFChildofRootProperty>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::shared_ptr<struct SGlTFChildofRootProperty>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<struct SGlTFChildofRootProperty>>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SGlTFChildofRootProperty>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<struct SGlTFChildofRootProperty>>>& _rData, GLTFCharValue& const _JsonValue);

    bool operator<<(struct SAnimationSampler& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<struct SAnimationSampler>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::shared_ptr<struct SAnimationSampler>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<struct SAnimationSampler>>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SAnimationSampler>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<struct SAnimationSampler>>>& _rData, GLTFCharValue& const _JsonValue);

    bool operator<<(struct SExtras& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<struct SExtras>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::shared_ptr<struct SExtras>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<struct SExtras>>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SExtras>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<struct SExtras>>>& _rData, GLTFCharValue& const _JsonValue);

    bool operator<<(struct SADOBE_materials_thin_transparencyglTFextension& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<struct SADOBE_materials_thin_transparencyglTFextension>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::shared_ptr<struct SADOBE_materials_thin_transparencyglTFextension>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<struct SADOBE_materials_thin_transparencyglTFextension>>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SADOBE_materials_thin_transparencyglTFextension>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<struct SADOBE_materials_thin_transparencyglTFextension>>>& _rData, GLTFCharValue& const _JsonValue);

    bool operator<<(struct SScene& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<struct SScene>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::shared_ptr<struct SScene>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<struct SScene>>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SScene>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<struct SScene>>>& _rData, GLTFCharValue& const _JsonValue);

    bool operator<<(struct STechnique& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<struct STechnique>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::shared_ptr<struct STechnique>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<struct STechnique>>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct STechnique>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<struct STechnique>>>& _rData, GLTFCharValue& const _JsonValue);

    bool operator<<(struct SCameraPerspective& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<struct SCameraPerspective>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::shared_ptr<struct SCameraPerspective>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<struct SCameraPerspective>>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SCameraPerspective>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<struct SCameraPerspective>>>& _rData, GLTFCharValue& const _JsonValue);

    bool operator<<(struct SLight& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<struct SLight>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::shared_ptr<struct SLight>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<struct SLight>>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SLight>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<struct SLight>>>& _rData, GLTFCharValue& const _JsonValue);

    bool operator<<(struct SBufferView& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<struct SBufferView>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::shared_ptr<struct SBufferView>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<struct SBufferView>>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SBufferView>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<struct SBufferView>>>& _rData, GLTFCharValue& const _JsonValue);

    bool operator<<(struct SMaterialNormalTextureInfo& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<struct SMaterialNormalTextureInfo>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::shared_ptr<struct SMaterialNormalTextureInfo>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<struct SMaterialNormalTextureInfo>>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SMaterialNormalTextureInfo>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<struct SMaterialNormalTextureInfo>>>& _rData, GLTFCharValue& const _JsonValue);

    bool operator<<(struct SSolarPanelGroup& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<struct SSolarPanelGroup>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::shared_ptr<struct SSolarPanelGroup>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<struct SSolarPanelGroup>>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SSolarPanelGroup>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<struct SSolarPanelGroup>>>& _rData, GLTFCharValue& const _JsonValue);

    bool operator<<(struct SKHR_materials_clearcoatglTFextension& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<struct SKHR_materials_clearcoatglTFextension>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::shared_ptr<struct SKHR_materials_clearcoatglTFextension>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<struct SKHR_materials_clearcoatglTFextension>>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SKHR_materials_clearcoatglTFextension>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<struct SKHR_materials_clearcoatglTFextension>>>& _rData, GLTFCharValue& const _JsonValue);

    bool operator<<(struct SUniform& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<struct SUniform>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::shared_ptr<struct SUniform>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<struct SUniform>>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SUniform>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<struct SUniform>>>& _rData, GLTFCharValue& const _JsonValue);

    bool operator<<(struct SMaterialOcclusionTextureInfo& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<struct SMaterialOcclusionTextureInfo>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::shared_ptr<struct SMaterialOcclusionTextureInfo>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<struct SMaterialOcclusionTextureInfo>>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SMaterialOcclusionTextureInfo>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<struct SMaterialOcclusionTextureInfo>>>& _rData, GLTFCharValue& const _JsonValue);

    bool operator<<(struct SAccessorSparseValues& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<struct SAccessorSparseValues>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::shared_ptr<struct SAccessorSparseValues>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<struct SAccessorSparseValues>>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SAccessorSparseValues>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<struct SAccessorSparseValues>>>& _rData, GLTFCharValue& const _JsonValue);

    bool operator<<(struct SMSFT_lodglTFextension& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<struct SMSFT_lodglTFextension>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::shared_ptr<struct SMSFT_lodglTFextension>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<struct SMSFT_lodglTFextension>>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SMSFT_lodglTFextension>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<struct SMSFT_lodglTFextension>>>& _rData, GLTFCharValue& const _JsonValue);

    bool operator<<(struct SAnimationChannelTarget& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<struct SAnimationChannelTarget>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::shared_ptr<struct SAnimationChannelTarget>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<struct SAnimationChannelTarget>>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SAnimationChannelTarget>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<struct SAnimationChannelTarget>>>& _rData, GLTFCharValue& const _JsonValue);

    bool operator<<(struct SMesh& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<struct SMesh>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::shared_ptr<struct SMesh>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<struct SMesh>>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SMesh>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<struct SMesh>>>& _rData, GLTFCharValue& const _JsonValue);

    bool operator<<(struct SAccessorSparse& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<struct SAccessorSparse>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::shared_ptr<struct SAccessorSparse>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<struct SAccessorSparse>>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SAccessorSparse>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<struct SAccessorSparse>>>& _rData, GLTFCharValue& const _JsonValue);

    bool operator<<(struct SShader& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<struct SShader>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::shared_ptr<struct SShader>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<struct SShader>>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SShader>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<struct SShader>>>& _rData, GLTFCharValue& const _JsonValue);

    bool operator<<(struct SMeshPrimitive& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<struct SMeshPrimitive>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::shared_ptr<struct SMeshPrimitive>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<struct SMeshPrimitive>>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SMeshPrimitive>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<struct SMeshPrimitive>>>& _rData, GLTFCharValue& const _JsonValue);

    bool operator<<(struct SArticulationStage& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<struct SArticulationStage>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::shared_ptr<struct SArticulationStage>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<struct SArticulationStage>>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SArticulationStage>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<struct SArticulationStage>>>& _rData, GLTFCharValue& const _JsonValue);

    bool operator<<(struct SKHR_materials_pbrSpecularGlossinessglTFextension& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<struct SKHR_materials_pbrSpecularGlossinessglTFextension>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::shared_ptr<struct SKHR_materials_pbrSpecularGlossinessglTFextension>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<struct SKHR_materials_pbrSpecularGlossinessglTFextension>>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SKHR_materials_pbrSpecularGlossinessglTFextension>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<struct SKHR_materials_pbrSpecularGlossinessglTFextension>>>& _rData, GLTFCharValue& const _JsonValue);

    bool operator<<(struct SExtension& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<struct SExtension>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::shared_ptr<struct SExtension>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<struct SExtension>>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SExtension>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<struct SExtension>>>& _rData, GLTFCharValue& const _JsonValue);

    bool operator<<(struct SAnimationChannel& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<struct SAnimationChannel>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::shared_ptr<struct SAnimationChannel>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<struct SAnimationChannel>>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SAnimationChannel>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<struct SAnimationChannel>>>& _rData, GLTFCharValue& const _JsonValue);

    bool operator<<(struct SGlTFId& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<struct SGlTFId>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::shared_ptr<struct SGlTFId>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<struct SGlTFId>>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SGlTFId>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<struct SGlTFId>>>& _rData, GLTFCharValue& const _JsonValue);

    bool operator<<(struct SAccessorSparseIndices& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<struct SAccessorSparseIndices>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::shared_ptr<struct SAccessorSparseIndices>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<struct SAccessorSparseIndices>>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SAccessorSparseIndices>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<struct SAccessorSparseIndices>>>& _rData, GLTFCharValue& const _JsonValue);

    bool operator<<(struct SKHR_lights_punctualnodeextension& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<struct SKHR_lights_punctualnodeextension>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::shared_ptr<struct SKHR_lights_punctualnodeextension>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<struct SKHR_lights_punctualnodeextension>>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SKHR_lights_punctualnodeextension>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<struct SKHR_lights_punctualnodeextension>>>& _rData, GLTFCharValue& const _JsonValue);

    bool operator<<(struct SNode& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<struct SNode>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::shared_ptr<struct SNode>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<struct SNode>>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SNode>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<struct SNode>>>& _rData, GLTFCharValue& const _JsonValue);

    bool operator<<(struct SAnimation& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<struct SAnimation>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::shared_ptr<struct SAnimation>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<struct SAnimation>>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SAnimation>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<struct SAnimation>>>& _rData, GLTFCharValue& const _JsonValue);

    bool operator<<(struct SSkin& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<struct SSkin>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::shared_ptr<struct SSkin>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<struct SSkin>>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SSkin>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<struct SSkin>>>& _rData, GLTFCharValue& const _JsonValue);

    bool operator<<(struct SMSFT_texture_ddsextension& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<struct SMSFT_texture_ddsextension>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::shared_ptr<struct SMSFT_texture_ddsextension>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<struct SMSFT_texture_ddsextension>>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SMSFT_texture_ddsextension>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<struct SMSFT_texture_ddsextension>>>& _rData, GLTFCharValue& const _JsonValue);

    bool operator<<(struct SAGI_articulationsglTFextension& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<struct SAGI_articulationsglTFextension>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::shared_ptr<struct SAGI_articulationsglTFextension>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<struct SAGI_articulationsglTFextension>>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SAGI_articulationsglTFextension>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<struct SAGI_articulationsglTFextension>>>& _rData, GLTFCharValue& const _JsonValue);

    bool operator<<(struct SAttribute& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<struct SAttribute>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::shared_ptr<struct SAttribute>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<struct SAttribute>>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SAttribute>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<struct SAttribute>>>& _rData, GLTFCharValue& const _JsonValue);

    bool operator<<(struct SMaterialPBRMetallicRoughness& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<struct SMaterialPBRMetallicRoughness>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::shared_ptr<struct SMaterialPBRMetallicRoughness>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<struct SMaterialPBRMetallicRoughness>>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SMaterialPBRMetallicRoughness>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<struct SMaterialPBRMetallicRoughness>>>& _rData, GLTFCharValue& const _JsonValue);

    bool operator<<(struct SKHR_draco_mesh_compressionextension& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<struct SKHR_draco_mesh_compressionextension>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::shared_ptr<struct SKHR_draco_mesh_compressionextension>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<struct SKHR_draco_mesh_compressionextension>>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SKHR_draco_mesh_compressionextension>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<struct SKHR_draco_mesh_compressionextension>>>& _rData, GLTFCharValue& const _JsonValue);

    bool operator<<(struct SCamera& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<struct SCamera>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::shared_ptr<struct SCamera>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<struct SCamera>>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SCamera>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<struct SCamera>>>& _rData, GLTFCharValue& const _JsonValue);

    bool operator<<(struct SImage& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<struct SImage>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::shared_ptr<struct SImage>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<struct SImage>>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SImage>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<struct SImage>>>& _rData, GLTFCharValue& const _JsonValue);

    bool operator<<(struct SAGI_stk_metadataglTFNodeextension& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<struct SAGI_stk_metadataglTFNodeextension>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::shared_ptr<struct SAGI_stk_metadataglTFNodeextension>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<struct SAGI_stk_metadataglTFNodeextension>>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SAGI_stk_metadataglTFNodeextension>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<struct SAGI_stk_metadataglTFNodeextension>>>& _rData, GLTFCharValue& const _JsonValue);

    bool operator<<(struct STexture& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<struct STexture>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::shared_ptr<struct STexture>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<struct STexture>>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct STexture>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<struct STexture>>>& _rData, GLTFCharValue& const _JsonValue);

    bool operator<<(struct SAGI_articulationsglTFNodeextension& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<struct SAGI_articulationsglTFNodeextension>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::shared_ptr<struct SAGI_articulationsglTFNodeextension>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<struct SAGI_articulationsglTFNodeextension>>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SAGI_articulationsglTFNodeextension>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<struct SAGI_articulationsglTFNodeextension>>>& _rData, GLTFCharValue& const _JsonValue);

    bool operator<<(struct SKHR_materials_unlitglTFextension& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<struct SKHR_materials_unlitglTFextension>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::shared_ptr<struct SKHR_materials_unlitglTFextension>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<struct SKHR_materials_unlitglTFextension>>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SKHR_materials_unlitglTFextension>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<struct SKHR_materials_unlitglTFextension>>>& _rData, GLTFCharValue& const _JsonValue);

    bool operator<<(struct SCameraOrthographic& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<struct SCameraOrthographic>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::shared_ptr<struct SCameraOrthographic>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<struct SCameraOrthographic>>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SCameraOrthographic>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<struct SCameraOrthographic>>>& _rData, GLTFCharValue& const _JsonValue);

    bool operator<<(struct SAGI_stk_metadataglTFextension& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<struct SAGI_stk_metadataglTFextension>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::shared_ptr<struct SAGI_stk_metadataglTFextension>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<struct SAGI_stk_metadataglTFextension>>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SAGI_stk_metadataglTFextension>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<struct SAGI_stk_metadataglTFextension>>>& _rData, GLTFCharValue& const _JsonValue);

    bool operator<<(struct SBuffer& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<struct SBuffer>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::shared_ptr<struct SBuffer>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<struct SBuffer>>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SBuffer>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<struct SBuffer>>>& _rData, GLTFCharValue& const _JsonValue);

    bool operator<<(struct SEXT_mesh_gpu_instancingglTFextension& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<struct SEXT_mesh_gpu_instancingglTFextension>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::shared_ptr<struct SEXT_mesh_gpu_instancingglTFextension>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<struct SEXT_mesh_gpu_instancingglTFextension>>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SEXT_mesh_gpu_instancingglTFextension>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<struct SEXT_mesh_gpu_instancingglTFextension>>>& _rData, GLTFCharValue& const _JsonValue);

    bool operator<<(struct SUniformValue& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<struct SUniformValue>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::shared_ptr<struct SUniformValue>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<struct SUniformValue>>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SUniformValue>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<struct SUniformValue>>>& _rData, GLTFCharValue& const _JsonValue);

    bool operator<<(struct SKHR_lights_punctualglTFextension& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<struct SKHR_lights_punctualglTFextension>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::shared_ptr<struct SKHR_lights_punctualglTFextension>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<struct SKHR_lights_punctualglTFextension>>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SKHR_lights_punctualglTFextension>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<struct SKHR_lights_punctualglTFextension>>>& _rData, GLTFCharValue& const _JsonValue);

    bool operator<<(struct SProgram& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<struct SProgram>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::shared_ptr<struct SProgram>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<struct SProgram>>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SProgram>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<struct SProgram>>>& _rData, GLTFCharValue& const _JsonValue);

    bool operator<<(struct SArticulation& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<struct SArticulation>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::shared_ptr<struct SArticulation>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<struct SArticulation>>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SArticulation>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<struct SArticulation>>>& _rData, GLTFCharValue& const _JsonValue);

    bool operator<<(struct SCESIUM_primitive_outlineglTFprimitiveextension& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<struct SCESIUM_primitive_outlineglTFprimitiveextension>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::shared_ptr<struct SCESIUM_primitive_outlineglTFprimitiveextension>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<struct SCESIUM_primitive_outlineglTFprimitiveextension>>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SCESIUM_primitive_outlineglTFprimitiveextension>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<struct SCESIUM_primitive_outlineglTFprimitiveextension>>>& _rData, GLTFCharValue& const _JsonValue);

    bool operator<<(struct SAccessor& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<struct SAccessor>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::shared_ptr<struct SAccessor>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<struct SAccessor>>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SAccessor>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<struct SAccessor>>>& _rData, GLTFCharValue& const _JsonValue);

    bool operator<<(struct SEXT_texture_webpglTFextension& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<struct SEXT_texture_webpglTFextension>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::shared_ptr<struct SEXT_texture_webpglTFextension>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<struct SEXT_texture_webpglTFextension>>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SEXT_texture_webpglTFextension>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<struct SEXT_texture_webpglTFextension>>>& _rData, GLTFCharValue& const _JsonValue);

    bool operator<<(struct SKHR_techniques_webglglTFextension& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<struct SKHR_techniques_webglglTFextension>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::shared_ptr<struct SKHR_techniques_webglglTFextension>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<struct SKHR_techniques_webglglTFextension>>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SKHR_techniques_webglglTFextension>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<struct SKHR_techniques_webglglTFextension>>>& _rData, GLTFCharValue& const _JsonValue);

    bool operator<<(struct SGlTF& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<struct SGlTF>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::shared_ptr<struct SGlTF>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<struct SGlTF>>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SGlTF>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<struct SGlTF>>>& _rData, GLTFCharValue& const _JsonValue);

    bool operator<<(struct SLightspot& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<struct SLightspot>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::shared_ptr<struct SLightspot>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<struct SLightspot>>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct SLightspot>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<struct SLightspot>>>& _rData, GLTFCharValue& const _JsonValue);

    bool operator<<(struct STextureInfo& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<struct STextureInfo>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::shared_ptr<struct STextureInfo>& _rData, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<struct STextureInfo>>& _rData, GLTFCharValue& const _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<struct STextureInfo>>& _vDatas, const GLTFCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<struct STextureInfo>>>& _rData, GLTFCharValue& const _JsonValue);

}

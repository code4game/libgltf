/*
 * This software is released under the MIT license.
 * 
 * Copyright (c) 2017-2023 Code 4 Game, Org. All Rights Reserved.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
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

#include "libgltf/libgltf.h"

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

namespace libgltf
{
    typedef rapidjson::UTF8<char>                          JSONChar;
    typedef rapidjson::SizeType                            JSONSizeType;
    typedef rapidjson::GenericDocument<JSONChar>           JSONCharDocument;
    typedef rapidjson::GenericValue<JSONChar>              JSONCharValue;
    typedef rapidjson::GenericStringRef<char>              JSONStringRef;
    typedef rapidjson::GenericStringBuffer<JSONChar>       JSONStringBuffer;
    typedef rapidjson::Writer<JSONStringBuffer, JSONChar>  JSONWriter;
    typedef rapidjson::GenericArray<true, JSONCharValue>   JSONCharConstArray;
    typedef rapidjson::GenericArray<false, JSONCharValue>  JSONCharArray;
    typedef rapidjson::GenericObject<true, JSONCharValue>  JSONCharConstObject;
    typedef rapidjson::GenericObject<false, JSONCharValue> JSONCharObject;
}

namespace libgltf
{
    template<typename TData>
    class TDataDoc
    {
    public:
        explicit TDataDoc(const TData& _rData, JSONCharDocument* _pDoc)
            : data(_rData)
            , doc(_pDoc)
        {
            //
        }
        const TData& data;
        JSONCharDocument* doc;
    };

    bool operator<<(SAccessor& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SAccessor>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SAccessor>& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SAccessor>>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SAccessor>>& _vDatas, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SAccessor>>>& _rData, JSONCharValue& _JsonValue);

    bool operator<<(SAccessorSparseIndices& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SAccessorSparseIndices>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SAccessorSparseIndices>& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SAccessorSparseIndices>>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SAccessorSparseIndices>>& _vDatas, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SAccessorSparseIndices>>>& _rData, JSONCharValue& _JsonValue);

    bool operator<<(SAccessorSparse& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SAccessorSparse>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SAccessorSparse>& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SAccessorSparse>>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SAccessorSparse>>& _vDatas, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SAccessorSparse>>>& _rData, JSONCharValue& _JsonValue);

    bool operator<<(SAccessorSparseValues& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SAccessorSparseValues>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SAccessorSparseValues>& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SAccessorSparseValues>>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SAccessorSparseValues>>& _vDatas, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SAccessorSparseValues>>>& _rData, JSONCharValue& _JsonValue);

    bool operator<<(SAnimationChannel& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SAnimationChannel>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SAnimationChannel>& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SAnimationChannel>>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SAnimationChannel>>& _vDatas, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SAnimationChannel>>>& _rData, JSONCharValue& _JsonValue);

    bool operator<<(SAnimationChannelTarget& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SAnimationChannelTarget>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SAnimationChannelTarget>& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SAnimationChannelTarget>>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SAnimationChannelTarget>>& _vDatas, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SAnimationChannelTarget>>>& _rData, JSONCharValue& _JsonValue);

    bool operator<<(SAnimationSampler& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SAnimationSampler>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SAnimationSampler>& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SAnimationSampler>>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SAnimationSampler>>& _vDatas, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SAnimationSampler>>>& _rData, JSONCharValue& _JsonValue);

    bool operator<<(SAnimation& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SAnimation>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SAnimation>& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SAnimation>>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SAnimation>>& _vDatas, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SAnimation>>>& _rData, JSONCharValue& _JsonValue);

    bool operator<<(SAsset& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SAsset>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SAsset>& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SAsset>>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SAsset>>& _vDatas, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SAsset>>>& _rData, JSONCharValue& _JsonValue);

    bool operator<<(SBuffer& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SBuffer>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SBuffer>& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SBuffer>>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SBuffer>>& _vDatas, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SBuffer>>>& _rData, JSONCharValue& _JsonValue);

    bool operator<<(SBufferView& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SBufferView>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SBufferView>& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SBufferView>>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SBufferView>>& _vDatas, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SBufferView>>>& _rData, JSONCharValue& _JsonValue);

    bool operator<<(SCameraOrthographic& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SCameraOrthographic>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SCameraOrthographic>& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SCameraOrthographic>>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SCameraOrthographic>>& _vDatas, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SCameraOrthographic>>>& _rData, JSONCharValue& _JsonValue);

    bool operator<<(SCameraPerspective& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SCameraPerspective>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SCameraPerspective>& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SCameraPerspective>>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SCameraPerspective>>& _vDatas, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SCameraPerspective>>>& _rData, JSONCharValue& _JsonValue);

    bool operator<<(SCamera& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SCamera>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SCamera>& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SCamera>>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SCamera>>& _vDatas, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SCamera>>>& _rData, JSONCharValue& _JsonValue);

    bool operator<<(SExtension& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SExtension>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SExtension>& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SExtension>>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SExtension>>& _vDatas, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SExtension>>>& _rData, JSONCharValue& _JsonValue);

    bool operator<<(SExtras& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SExtras>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SExtras>& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SExtras>>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SExtras>>& _vDatas, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SExtras>>>& _rData, JSONCharValue& _JsonValue);

    bool operator<<(SGlTF& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SGlTF>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SGlTF>& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SGlTF>>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SGlTF>>& _vDatas, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SGlTF>>>& _rData, JSONCharValue& _JsonValue);

    bool operator<<(SGlTFChildofRootProperty& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SGlTFChildofRootProperty>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SGlTFChildofRootProperty>& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SGlTFChildofRootProperty>>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SGlTFChildofRootProperty>>& _vDatas, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SGlTFChildofRootProperty>>>& _rData, JSONCharValue& _JsonValue);

    bool operator<<(SGlTFId& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SGlTFId>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SGlTFId>& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SGlTFId>>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SGlTFId>>& _vDatas, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SGlTFId>>>& _rData, JSONCharValue& _JsonValue);

    bool operator<<(SGlTFProperty& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SGlTFProperty>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SGlTFProperty>& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SGlTFProperty>>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SGlTFProperty>>& _vDatas, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SGlTFProperty>>>& _rData, JSONCharValue& _JsonValue);

    bool operator<<(SImage& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SImage>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SImage>& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SImage>>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SImage>>& _vDatas, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SImage>>>& _rData, JSONCharValue& _JsonValue);

    bool operator<<(SMaterialNormalTextureInfo& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SMaterialNormalTextureInfo>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SMaterialNormalTextureInfo>& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SMaterialNormalTextureInfo>>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SMaterialNormalTextureInfo>>& _vDatas, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SMaterialNormalTextureInfo>>>& _rData, JSONCharValue& _JsonValue);

    bool operator<<(SMaterialOcclusionTextureInfo& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SMaterialOcclusionTextureInfo>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SMaterialOcclusionTextureInfo>& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SMaterialOcclusionTextureInfo>>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SMaterialOcclusionTextureInfo>>& _vDatas, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SMaterialOcclusionTextureInfo>>>& _rData, JSONCharValue& _JsonValue);

    bool operator<<(SMaterialPBRMetallicRoughness& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SMaterialPBRMetallicRoughness>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SMaterialPBRMetallicRoughness>& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SMaterialPBRMetallicRoughness>>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SMaterialPBRMetallicRoughness>>& _vDatas, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SMaterialPBRMetallicRoughness>>>& _rData, JSONCharValue& _JsonValue);

    bool operator<<(SMaterial& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SMaterial>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SMaterial>& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SMaterial>>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SMaterial>>& _vDatas, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SMaterial>>>& _rData, JSONCharValue& _JsonValue);

    bool operator<<(SMeshPrimitive& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SMeshPrimitive>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SMeshPrimitive>& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SMeshPrimitive>>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SMeshPrimitive>>& _vDatas, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SMeshPrimitive>>>& _rData, JSONCharValue& _JsonValue);

    bool operator<<(SMesh& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SMesh>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SMesh>& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SMesh>>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SMesh>>& _vDatas, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SMesh>>>& _rData, JSONCharValue& _JsonValue);

    bool operator<<(SNode& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SNode>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SNode>& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SNode>>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SNode>>& _vDatas, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SNode>>>& _rData, JSONCharValue& _JsonValue);

    bool operator<<(SSampler& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SSampler>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SSampler>& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SSampler>>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SSampler>>& _vDatas, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SSampler>>>& _rData, JSONCharValue& _JsonValue);

    bool operator<<(SScene& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SScene>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SScene>& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SScene>>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SScene>>& _vDatas, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SScene>>>& _rData, JSONCharValue& _JsonValue);

    bool operator<<(SSkin& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SSkin>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SSkin>& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SSkin>>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SSkin>>& _vDatas, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SSkin>>>& _rData, JSONCharValue& _JsonValue);

    bool operator<<(STexture& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<STexture>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<STexture>& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<STexture>>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<STexture>>& _vDatas, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<STexture>>>& _rData, JSONCharValue& _JsonValue);

    bool operator<<(STextureInfo& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<STextureInfo>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<STextureInfo>& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<STextureInfo>>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<STextureInfo>>& _vDatas, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<STextureInfo>>>& _rData, JSONCharValue& _JsonValue);

    bool operator<<(SKHR_draco_mesh_compressionextension& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SKHR_draco_mesh_compressionextension>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SKHR_draco_mesh_compressionextension>& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SKHR_draco_mesh_compressionextension>>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SKHR_draco_mesh_compressionextension>>& _vDatas, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SKHR_draco_mesh_compressionextension>>>& _rData, JSONCharValue& _JsonValue);

    bool operator<<(SKHR_lights_punctualglTFextension& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SKHR_lights_punctualglTFextension>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SKHR_lights_punctualglTFextension>& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SKHR_lights_punctualglTFextension>>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SKHR_lights_punctualglTFextension>>& _vDatas, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SKHR_lights_punctualglTFextension>>>& _rData, JSONCharValue& _JsonValue);

    bool operator<<(SLight& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SLight>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SLight>& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SLight>>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SLight>>& _vDatas, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SLight>>>& _rData, JSONCharValue& _JsonValue);

    bool operator<<(SLightspot& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SLightspot>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SLightspot>& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SLightspot>>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SLightspot>>& _vDatas, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SLightspot>>>& _rData, JSONCharValue& _JsonValue);

    bool operator<<(SKHR_lights_punctualnodeextension& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SKHR_lights_punctualnodeextension>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SKHR_lights_punctualnodeextension>& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SKHR_lights_punctualnodeextension>>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SKHR_lights_punctualnodeextension>>& _vDatas, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SKHR_lights_punctualnodeextension>>>& _rData, JSONCharValue& _JsonValue);

    bool operator<<(SKHR_materials_clearcoatglTFextension& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SKHR_materials_clearcoatglTFextension>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SKHR_materials_clearcoatglTFextension>& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SKHR_materials_clearcoatglTFextension>>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SKHR_materials_clearcoatglTFextension>>& _vDatas, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SKHR_materials_clearcoatglTFextension>>>& _rData, JSONCharValue& _JsonValue);

    bool operator<<(SKHR_materials_emissive_strengthglTFextension& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SKHR_materials_emissive_strengthglTFextension>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SKHR_materials_emissive_strengthglTFextension>& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SKHR_materials_emissive_strengthglTFextension>>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SKHR_materials_emissive_strengthglTFextension>>& _vDatas, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SKHR_materials_emissive_strengthglTFextension>>>& _rData, JSONCharValue& _JsonValue);

    bool operator<<(SKHR_materials_iorglTFextension& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SKHR_materials_iorglTFextension>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SKHR_materials_iorglTFextension>& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SKHR_materials_iorglTFextension>>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SKHR_materials_iorglTFextension>>& _vDatas, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SKHR_materials_iorglTFextension>>>& _rData, JSONCharValue& _JsonValue);

    bool operator<<(SKHR_materials_iridescenceglTFextension& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SKHR_materials_iridescenceglTFextension>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SKHR_materials_iridescenceglTFextension>& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SKHR_materials_iridescenceglTFextension>>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SKHR_materials_iridescenceglTFextension>>& _vDatas, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SKHR_materials_iridescenceglTFextension>>>& _rData, JSONCharValue& _JsonValue);

    bool operator<<(SKHR_materials_sheenglTFextension& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SKHR_materials_sheenglTFextension>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SKHR_materials_sheenglTFextension>& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SKHR_materials_sheenglTFextension>>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SKHR_materials_sheenglTFextension>>& _vDatas, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SKHR_materials_sheenglTFextension>>>& _rData, JSONCharValue& _JsonValue);

    bool operator<<(SKHR_materials_specularglTFextension& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SKHR_materials_specularglTFextension>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SKHR_materials_specularglTFextension>& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SKHR_materials_specularglTFextension>>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SKHR_materials_specularglTFextension>>& _vDatas, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SKHR_materials_specularglTFextension>>>& _rData, JSONCharValue& _JsonValue);

    bool operator<<(SKHR_materials_transmissionglTFextension& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SKHR_materials_transmissionglTFextension>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SKHR_materials_transmissionglTFextension>& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SKHR_materials_transmissionglTFextension>>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SKHR_materials_transmissionglTFextension>>& _vDatas, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SKHR_materials_transmissionglTFextension>>>& _rData, JSONCharValue& _JsonValue);

    bool operator<<(SKHR_materials_unlitglTFextension& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SKHR_materials_unlitglTFextension>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SKHR_materials_unlitglTFextension>& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SKHR_materials_unlitglTFextension>>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SKHR_materials_unlitglTFextension>>& _vDatas, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SKHR_materials_unlitglTFextension>>>& _rData, JSONCharValue& _JsonValue);

    bool operator<<(SKHR_materials_variantsglTFextension& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SKHR_materials_variantsglTFextension>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SKHR_materials_variantsglTFextension>& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SKHR_materials_variantsglTFextension>>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SKHR_materials_variantsglTFextension>>& _vDatas, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SKHR_materials_variantsglTFextension>>>& _rData, JSONCharValue& _JsonValue);

    bool operator<<(SKHR_materials_variantsmeshprimitiveextension& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SKHR_materials_variantsmeshprimitiveextension>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SKHR_materials_variantsmeshprimitiveextension>& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SKHR_materials_variantsmeshprimitiveextension>>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SKHR_materials_variantsmeshprimitiveextension>>& _vDatas, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SKHR_materials_variantsmeshprimitiveextension>>>& _rData, JSONCharValue& _JsonValue);

    bool operator<<(SKHR_materials_volumeglTFextension& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SKHR_materials_volumeglTFextension>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SKHR_materials_volumeglTFextension>& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SKHR_materials_volumeglTFextension>>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SKHR_materials_volumeglTFextension>>& _vDatas, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SKHR_materials_volumeglTFextension>>>& _rData, JSONCharValue& _JsonValue);

    bool operator<<(SKHR_texture_transformtextureInfoextension& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SKHR_texture_transformtextureInfoextension>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SKHR_texture_transformtextureInfoextension>& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SKHR_texture_transformtextureInfoextension>>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SKHR_texture_transformtextureInfoextension>>& _vDatas, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SKHR_texture_transformtextureInfoextension>>>& _rData, JSONCharValue& _JsonValue);

    bool operator<<(SADOBE_materials_thin_transparencyglTFextension& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SADOBE_materials_thin_transparencyglTFextension>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SADOBE_materials_thin_transparencyglTFextension>& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SADOBE_materials_thin_transparencyglTFextension>>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SADOBE_materials_thin_transparencyglTFextension>>& _vDatas, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SADOBE_materials_thin_transparencyglTFextension>>>& _rData, JSONCharValue& _JsonValue);

    bool operator<<(SArticulation& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SArticulation>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SArticulation>& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SArticulation>>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SArticulation>>& _vDatas, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SArticulation>>>& _rData, JSONCharValue& _JsonValue);

    bool operator<<(SArticulationStage& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SArticulationStage>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SArticulationStage>& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SArticulationStage>>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SArticulationStage>>& _vDatas, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SArticulationStage>>>& _rData, JSONCharValue& _JsonValue);

    bool operator<<(SAGI_articulationsglTFextension& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SAGI_articulationsglTFextension>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SAGI_articulationsglTFextension>& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SAGI_articulationsglTFextension>>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SAGI_articulationsglTFextension>>& _vDatas, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SAGI_articulationsglTFextension>>>& _rData, JSONCharValue& _JsonValue);

    bool operator<<(SAGI_articulationsglTFNodeextension& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SAGI_articulationsglTFNodeextension>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SAGI_articulationsglTFNodeextension>& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SAGI_articulationsglTFNodeextension>>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SAGI_articulationsglTFNodeextension>>& _vDatas, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SAGI_articulationsglTFNodeextension>>>& _rData, JSONCharValue& _JsonValue);

    bool operator<<(SAGI_stk_metadataglTFextension& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SAGI_stk_metadataglTFextension>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SAGI_stk_metadataglTFextension>& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SAGI_stk_metadataglTFextension>>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SAGI_stk_metadataglTFextension>>& _vDatas, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SAGI_stk_metadataglTFextension>>>& _rData, JSONCharValue& _JsonValue);

    bool operator<<(SAGI_stk_metadataglTFNodeextension& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SAGI_stk_metadataglTFNodeextension>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SAGI_stk_metadataglTFNodeextension>& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SAGI_stk_metadataglTFNodeextension>>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SAGI_stk_metadataglTFNodeextension>>& _vDatas, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SAGI_stk_metadataglTFNodeextension>>>& _rData, JSONCharValue& _JsonValue);

    bool operator<<(SSolarPanelGroup& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SSolarPanelGroup>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SSolarPanelGroup>& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SSolarPanelGroup>>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SSolarPanelGroup>>& _vDatas, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SSolarPanelGroup>>>& _rData, JSONCharValue& _JsonValue);

    bool operator<<(SCESIUM_primitive_outlineglTFprimitiveextension& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SCESIUM_primitive_outlineglTFprimitiveextension>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SCESIUM_primitive_outlineglTFprimitiveextension>& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SCESIUM_primitive_outlineglTFprimitiveextension>>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SCESIUM_primitive_outlineglTFprimitiveextension>>& _vDatas, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SCESIUM_primitive_outlineglTFprimitiveextension>>>& _rData, JSONCharValue& _JsonValue);

    bool operator<<(SEXT_lights_iesglTFextension& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SEXT_lights_iesglTFextension>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SEXT_lights_iesglTFextension>& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SEXT_lights_iesglTFextension>>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SEXT_lights_iesglTFextension>>& _vDatas, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SEXT_lights_iesglTFextension>>>& _rData, JSONCharValue& _JsonValue);

    bool operator<<(SLightProfile& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SLightProfile>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SLightProfile>& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SLightProfile>>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SLightProfile>>& _vDatas, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SLightProfile>>>& _rData, JSONCharValue& _JsonValue);

    bool operator<<(SEXT_lights_iesnodeextension& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SEXT_lights_iesnodeextension>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SEXT_lights_iesnodeextension>& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SEXT_lights_iesnodeextension>>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SEXT_lights_iesnodeextension>>& _vDatas, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SEXT_lights_iesnodeextension>>>& _rData, JSONCharValue& _JsonValue);

    bool operator<<(SEXT_mesh_gpu_instancingglTFextension& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SEXT_mesh_gpu_instancingglTFextension>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SEXT_mesh_gpu_instancingglTFextension>& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SEXT_mesh_gpu_instancingglTFextension>>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SEXT_mesh_gpu_instancingglTFextension>>& _vDatas, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SEXT_mesh_gpu_instancingglTFextension>>>& _rData, JSONCharValue& _JsonValue);

    bool operator<<(SEXT_texture_webpglTFextension& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SEXT_texture_webpglTFextension>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SEXT_texture_webpglTFextension>& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SEXT_texture_webpglTFextension>>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SEXT_texture_webpglTFextension>>& _vDatas, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SEXT_texture_webpglTFextension>>>& _rData, JSONCharValue& _JsonValue);

    bool operator<<(SFB_geometry_metadatasceneextension& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SFB_geometry_metadatasceneextension>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SFB_geometry_metadatasceneextension>& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SFB_geometry_metadatasceneextension>>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SFB_geometry_metadatasceneextension>>& _vDatas, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SFB_geometry_metadatasceneextension>>>& _rData, JSONCharValue& _JsonValue);

    bool operator<<(SMinimumandmaximumboundingboxextent& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SMinimumandmaximumboundingboxextent>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SMinimumandmaximumboundingboxextent>& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SMinimumandmaximumboundingboxextent>>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SMinimumandmaximumboundingboxextent>>& _vDatas, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SMinimumandmaximumboundingboxextent>>>& _rData, JSONCharValue& _JsonValue);

    bool operator<<(SMSFT_lodglTFextension& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SMSFT_lodglTFextension>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SMSFT_lodglTFextension>& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SMSFT_lodglTFextension>>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SMSFT_lodglTFextension>>& _vDatas, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SMSFT_lodglTFextension>>>& _rData, JSONCharValue& _JsonValue);

    bool operator<<(SMSFT_texture_ddsextension& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<SMSFT_texture_ddsextension>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SMSFT_texture_ddsextension>& _rData, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::shared_ptr<SMSFT_texture_ddsextension>>& _rData, JSONCharValue& _JsonValue);
    bool operator<<(std::vector<std::shared_ptr<SMSFT_texture_ddsextension>>& _vDatas, const JSONCharValue& _JsonValue);
    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SMSFT_texture_ddsextension>>>& _rData, JSONCharValue& _JsonValue);

}

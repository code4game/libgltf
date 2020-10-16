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

#include "libgltfpch.h"
#include "libgltf/libgltf.h"

namespace libgltf
{
    SObject::SObject()
        : schemaType(GLTFTEXT(""))
    {
        //
    }

    SGlTFProperty::SGlTFProperty()
        : SObject()
        , extras(nullptr)
        , extensions(nullptr)
    {
        //
    }

    SGlTFProperty::operator bool() const
    {
        //TODO:
        return true;
    }

    SKHR_techniques_webglmaterialextension::SKHR_techniques_webglmaterialextension()
        : SGlTFProperty()
        , technique(nullptr)
        , values()
    {
        //
    }

    SKHR_techniques_webglmaterialextension::operator bool() const
    {
        //TODO:
        return true;
    }

    SGlTFChildofRootProperty::SGlTFChildofRootProperty()
        : SGlTFProperty()
        , name(GLTFTEXT(""))
    {
        //
    }

    SGlTFChildofRootProperty::operator bool() const
    {
        //TODO:
        return true;
    }

    SMaterial::SMaterial()
        : SGlTFChildofRootProperty()
        , alphaCutoff(0.500000f)
        , emissiveTexture(nullptr)
        , pbrMetallicRoughness(nullptr)
        , occlusionTexture(nullptr)
        , alphaMode(GLTFTEXT("OPAQUE"))
        , doubleSided(false)
        , normalTexture(nullptr)
        , emissiveFactor({ 0.000000f, 0.000000f, 0.000000f })
    {
        //
    }

    SMaterial::operator bool() const
    {
        //TODO:
        return true;
    }

    SKHR_texture_transformtextureInfoextension::SKHR_texture_transformtextureInfoextension()
        : SGlTFProperty()
        , rotation(0.000000f)
        , scale({ 1.000000f, 1.000000f })
        , texCoord(0)
        , offset({ 0.000000f, 0.000000f })
    {
        //
    }

    SKHR_texture_transformtextureInfoextension::operator bool() const
    {
        //TODO:
        return true;
    }

    SAsset::SAsset()
        : SGlTFProperty()
        , minVersion(GLTFTEXT(""))
        , version(GLTFTEXT(""))
        , generator(GLTFTEXT(""))
        , copyright(GLTFTEXT(""))
    {
        //
    }

    SAsset::operator bool() const
    {
        //TODO:
        return true;
    }

    SSampler::SSampler()
        : SGlTFChildofRootProperty()
        , wrapS(10497)
        , minFilter(0)
        , magFilter(0)
        , wrapT(10497)
    {
        //
    }

    SSampler::operator bool() const
    {
        //TODO:
        return true;
    }

    SAnimationSampler::SAnimationSampler()
        : SGlTFProperty()
        , input(nullptr)
        , output(nullptr)
        , interpolation(GLTFTEXT("LINEAR"))
    {
        //
    }

    SAnimationSampler::operator bool() const
    {
        //TODO:
        return true;
    }

    SExtras::SExtras()
        : SObject()
    {
        //
    }

    SExtras::operator bool() const
    {
        //TODO:
        return true;
    }

    SADOBE_materials_thin_transparencyglTFextension::SADOBE_materials_thin_transparencyglTFextension()
        : SGlTFProperty()
        , ior(1.330000f)
        , transmissionTexture(nullptr)
        , transmissionFactor(1.000000f)
    {
        //
    }

    SADOBE_materials_thin_transparencyglTFextension::operator bool() const
    {
        //TODO:
        return true;
    }

    SScene::SScene()
        : SGlTFChildofRootProperty()
        , nodes()
    {
        //
    }

    SScene::operator bool() const
    {
        //TODO:
        return true;
    }

    STechnique::STechnique()
        : SGlTFChildofRootProperty()
        , program(nullptr)
        , uniforms()
        , attributes()
    {
        //
    }

    STechnique::operator bool() const
    {
        //TODO:
        return true;
    }

    SCameraPerspective::SCameraPerspective()
        : SGlTFProperty()
        , aspectRatio(0.0f)
        , yfov(0.0f)
        , znear(0.0f)
        , zfar(0.0f)
    {
        //
    }

    SCameraPerspective::operator bool() const
    {
        //TODO:
        return true;
    }

    SLight::SLight()
        : SGlTFChildofRootProperty()
        , color({ 1.000000f, 1.000000f, 1.000000f })
        , type(GLTFTEXT(""))
        , intensity(1.000000f)
        , spot(nullptr)
        , range(0.0f)
    {
        //
    }

    SLight::operator bool() const
    {
        //TODO:
        return true;
    }

    SBufferView::SBufferView()
        : SGlTFChildofRootProperty()
        , byteLength(0)
        , buffer(nullptr)
        , byteOffset(0)
        , target(0)
        , byteStride(0)
    {
        //
    }

    SBufferView::operator bool() const
    {
        //TODO:
        return true;
    }

    STextureInfo::STextureInfo()
        : SGlTFProperty()
        , index(nullptr)
        , texCoord(0)
    {
        //
    }

    STextureInfo::operator bool() const
    {
        //TODO:
        return true;
    }

    SMaterialNormalTextureInfo::SMaterialNormalTextureInfo()
        : STextureInfo()
        , scale(1.000000f)
    {
        //
    }

    SMaterialNormalTextureInfo::operator bool() const
    {
        //TODO:
        return true;
    }

    SSolarPanelGroup::SSolarPanelGroup()
        : SGlTFProperty()
        , efficiency(0.0f)
        , name(GLTFTEXT(""))
    {
        //
    }

    SSolarPanelGroup::operator bool() const
    {
        //TODO:
        return true;
    }

    SKHR_materials_clearcoatglTFextension::SKHR_materials_clearcoatglTFextension()
        : SGlTFProperty()
        , clearcoatRoughnessTexture(nullptr)
        , clearcoatFactor(0.000000f)
        , clearcoatTexture(nullptr)
        , clearcoatNormalTexture(nullptr)
        , clearcoatRoughnessFactor(0.000000f)
    {
        //
    }

    SKHR_materials_clearcoatglTFextension::operator bool() const
    {
        //TODO:
        return true;
    }

    SUniform::SUniform()
        : SGlTFProperty()
        , count(0)
        , node(nullptr)
        , semantic(GLTFTEXT(""))
        , value(nullptr)
        , type(0)
    {
        //
    }

    SUniform::operator bool() const
    {
        //TODO:
        return true;
    }

    SMaterialOcclusionTextureInfo::SMaterialOcclusionTextureInfo()
        : STextureInfo()
        , strength(1.000000f)
    {
        //
    }

    SMaterialOcclusionTextureInfo::operator bool() const
    {
        //TODO:
        return true;
    }

    SAccessorSparseValues::SAccessorSparseValues()
        : SGlTFProperty()
        , bufferView(nullptr)
        , byteOffset(0)
    {
        //
    }

    SAccessorSparseValues::operator bool() const
    {
        //TODO:
        return true;
    }

    SMSFT_lodglTFextension::SMSFT_lodglTFextension()
        : SGlTFProperty()
        , ids()
    {
        //
    }

    SMSFT_lodglTFextension::operator bool() const
    {
        //TODO:
        return true;
    }

    SAnimationChannelTarget::SAnimationChannelTarget()
        : SGlTFProperty()
        , node(nullptr)
        , path(GLTFTEXT(""))
    {
        //
    }

    SAnimationChannelTarget::operator bool() const
    {
        //TODO:
        return true;
    }

    SMesh::SMesh()
        : SGlTFChildofRootProperty()
        , primitives()
        , weights()
    {
        //
    }

    SMesh::operator bool() const
    {
        //TODO:
        return true;
    }

    SAccessorSparse::SAccessorSparse()
        : SGlTFProperty()
        , count(0)
        , indices(nullptr)
        , values(nullptr)
    {
        //
    }

    SAccessorSparse::operator bool() const
    {
        //TODO:
        return true;
    }

    SShader::SShader()
        : SGlTFChildofRootProperty()
        , bufferView(nullptr)
        , type(0)
        , uri(GLTFTEXT(""))
    {
        //
    }

    SShader::operator bool() const
    {
        //TODO:
        return true;
    }

    SMeshPrimitive::SMeshPrimitive()
        : SGlTFProperty()
        , indices(nullptr)
        , attributes()
        , material(nullptr)
        , mode(4)
        , targets()
    {
        //
    }

    SMeshPrimitive::operator bool() const
    {
        //TODO:
        return true;
    }

    SArticulationStage::SArticulationStage()
        : SGlTFProperty()
        , maximumValue(0.0f)
        , initialValue(0.0f)
        , minimumValue(0.0f)
        , name(GLTFTEXT(""))
        , type(GLTFTEXT(""))
    {
        //
    }

    SArticulationStage::operator bool() const
    {
        //TODO:
        return true;
    }

    SKHR_materials_pbrSpecularGlossinessglTFextension::SKHR_materials_pbrSpecularGlossinessglTFextension()
        : SGlTFProperty()
        , specularFactor({ 1.000000f, 1.000000f, 1.000000f })
        , glossinessFactor(1.000000f)
        , diffuseTexture(nullptr)
        , specularGlossinessTexture(nullptr)
        , diffuseFactor({ 1.000000f, 1.000000f, 1.000000f, 1.000000f })
    {
        //
    }

    SKHR_materials_pbrSpecularGlossinessglTFextension::operator bool() const
    {
        //TODO:
        return true;
    }

    SExtension::SExtension()
        : SObject()
        // Manual code lines
        , properties()
    {
        //
    }

    SExtension::operator bool() const
    {
        //TODO:
        return true;
    }

    SAnimationChannel::SAnimationChannel()
        : SGlTFProperty()
        , target(nullptr)
        , sampler(nullptr)
    {
        //
    }

    SAnimationChannel::operator bool() const
    {
        //TODO:
        return true;
    }

    SGlTFId::SGlTFId()
        : SObject()
        , int32_tValue(0)
    {
        //
    }

    SGlTFId::operator bool() const
    {
        //TODO:
        return true;
    }

    SGlTFId::operator int32_t() const
    {
        return int32_tValue;
    }

    SAccessorSparseIndices::SAccessorSparseIndices()
        : SGlTFProperty()
        , componentType(0)
        , bufferView(nullptr)
        , byteOffset(0)
    {
        //
    }

    SAccessorSparseIndices::operator bool() const
    {
        //TODO:
        return true;
    }

    SKHR_lights_punctualnodeextension::SKHR_lights_punctualnodeextension()
        : SGlTFProperty()
        , light(nullptr)
    {
        //
    }

    SKHR_lights_punctualnodeextension::operator bool() const
    {
        //TODO:
        return true;
    }

    SNode::SNode()
        : SGlTFChildofRootProperty()
        , scale({ 1.000000f, 1.000000f, 1.000000f })
        , rotation({ 0.000000f, 0.000000f, 0.000000f, 1.000000f })
        , matrix({ 1.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, 1.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, 1.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, 1.000000f })
        , mesh(nullptr)
        , camera(nullptr)
        , weights()
        , skin(nullptr)
        , translation({ 0.000000f, 0.000000f, 0.000000f })
        , children()
    {
        //
    }

    SNode::operator bool() const
    {
        //TODO:
        return true;
    }

    SAnimation::SAnimation()
        : SGlTFChildofRootProperty()
        , channels()
        , samplers()
    {
        //
    }

    SAnimation::operator bool() const
    {
        //TODO:
        return true;
    }

    SSkin::SSkin()
        : SGlTFChildofRootProperty()
        , joints()
        , inverseBindMatrices(nullptr)
        , skeleton(nullptr)
    {
        //
    }

    SSkin::operator bool() const
    {
        //TODO:
        return true;
    }

    SMSFT_texture_ddsextension::SMSFT_texture_ddsextension()
        : SGlTFProperty()
        , source(nullptr)
    {
        //
    }

    SMSFT_texture_ddsextension::operator bool() const
    {
        //TODO:
        return true;
    }

    SAGI_articulationsglTFextension::SAGI_articulationsglTFextension()
        : SGlTFProperty()
        , articulations()
    {
        //
    }

    SAGI_articulationsglTFextension::operator bool() const
    {
        //TODO:
        return true;
    }

    SAttribute::SAttribute()
        : SGlTFProperty()
        , semantic(GLTFTEXT(""))
    {
        //
    }

    SAttribute::operator bool() const
    {
        //TODO:
        return true;
    }

    SMaterialPBRMetallicRoughness::SMaterialPBRMetallicRoughness()
        : SGlTFProperty()
        , roughnessFactor(1.000000f)
        , baseColorTexture(nullptr)
        , metallicFactor(1.000000f)
        , baseColorFactor({ 1.000000f, 1.000000f, 1.000000f, 1.000000f })
        , metallicRoughnessTexture(nullptr)
    {
        //
    }

    SMaterialPBRMetallicRoughness::operator bool() const
    {
        //TODO:
        return true;
    }

    SKHR_draco_mesh_compressionextension::SKHR_draco_mesh_compressionextension()
        : SGlTFProperty()
        , attributes()
        , bufferView(nullptr)
    {
        //
    }

    SKHR_draco_mesh_compressionextension::operator bool() const
    {
        //TODO:
        return true;
    }

    SCamera::SCamera()
        : SGlTFChildofRootProperty()
        , type(GLTFTEXT(""))
        , perspective(nullptr)
        , orthographic(nullptr)
    {
        //
    }

    SCamera::operator bool() const
    {
        //TODO:
        return true;
    }

    SImage::SImage()
        : SGlTFChildofRootProperty()
        , mimeType(GLTFTEXT(""))
        , bufferView(nullptr)
        , uri(GLTFTEXT(""))
    {
        //
    }

    SImage::operator bool() const
    {
        //TODO:
        return true;
    }

    SAGI_stk_metadataglTFNodeextension::SAGI_stk_metadataglTFNodeextension()
        : SGlTFProperty()
        , solarPanelGroupName(GLTFTEXT(""))
        , noObscuration(false)
    {
        //
    }

    SAGI_stk_metadataglTFNodeextension::operator bool() const
    {
        //TODO:
        return true;
    }

    STexture::STexture()
        : SGlTFChildofRootProperty()
        , source(nullptr)
        , sampler(nullptr)
    {
        //
    }

    STexture::operator bool() const
    {
        //TODO:
        return true;
    }

    SAGI_articulationsglTFNodeextension::SAGI_articulationsglTFNodeextension()
        : SGlTFProperty()
        , articulationName(GLTFTEXT(""))
        , isAttachPoint(false)
    {
        //
    }

    SAGI_articulationsglTFNodeextension::operator bool() const
    {
        //TODO:
        return true;
    }

    SKHR_materials_unlitglTFextension::SKHR_materials_unlitglTFextension()
        : SGlTFProperty()
    {
        //
    }

    SKHR_materials_unlitglTFextension::operator bool() const
    {
        //TODO:
        return true;
    }

    SCameraOrthographic::SCameraOrthographic()
        : SGlTFProperty()
        , xmag(0.0f)
        , ymag(0.0f)
        , zfar(0.0f)
        , znear(0.0f)
    {
        //
    }

    SCameraOrthographic::operator bool() const
    {
        //TODO:
        return true;
    }

    SAGI_stk_metadataglTFextension::SAGI_stk_metadataglTFextension()
        : SGlTFProperty()
        , solarPanelGroups()
    {
        //
    }

    SAGI_stk_metadataglTFextension::operator bool() const
    {
        //TODO:
        return true;
    }

    SBuffer::SBuffer()
        : SGlTFChildofRootProperty()
        , byteLength(0)
        , uri(GLTFTEXT(""))
    {
        //
    }

    SBuffer::operator bool() const
    {
        //TODO:
        return true;
    }

    SEXT_mesh_gpu_instancingglTFextension::SEXT_mesh_gpu_instancingglTFextension()
        : SGlTFProperty()
        , attributes()
    {
        //
    }

    SEXT_mesh_gpu_instancingglTFextension::operator bool() const
    {
        //TODO:
        return true;
    }

    SUniformValue::SUniformValue()
        : SObject()
    {
        //
    }

    SUniformValue::operator bool() const
    {
        //TODO:
        return true;
    }

    SKHR_lights_punctualglTFextension::SKHR_lights_punctualglTFextension()
        : SGlTFProperty()
        , lights()
    {
        //
    }

    SKHR_lights_punctualglTFextension::operator bool() const
    {
        //TODO:
        return true;
    }

    SProgram::SProgram()
        : SGlTFChildofRootProperty()
        , vertexShader(nullptr)
        , glExtensions()
        , fragmentShader(nullptr)
    {
        //
    }

    SProgram::operator bool() const
    {
        //TODO:
        return true;
    }

    SArticulation::SArticulation()
        : SGlTFProperty()
        , stages()
        , name(GLTFTEXT(""))
        , pointingVector()
    {
        //
    }

    SArticulation::operator bool() const
    {
        //TODO:
        return true;
    }

    SCESIUM_primitive_outlineglTFprimitiveextension::SCESIUM_primitive_outlineglTFprimitiveextension()
        : SGlTFProperty()
        , indices(0)
    {
        //
    }

    SCESIUM_primitive_outlineglTFprimitiveextension::operator bool() const
    {
        //TODO:
        return true;
    }

    SAccessor::SAccessor()
        : SGlTFChildofRootProperty()
        , count(0)
        , min()
        , max()
        , bufferView(nullptr)
        , componentType(0)
        , byteOffset(0)
        , sparse(nullptr)
        , type(GLTFTEXT(""))
        , normalized(false)
    {
        //
    }

    SAccessor::operator bool() const
    {
        //TODO:
        return true;
    }

    SEXT_texture_webpglTFextension::SEXT_texture_webpglTFextension()
        : SGlTFProperty()
        , source(nullptr)
    {
        //
    }

    SEXT_texture_webpglTFextension::operator bool() const
    {
        //TODO:
        return true;
    }

    SKHR_techniques_webglglTFextension::SKHR_techniques_webglglTFextension()
        : SGlTFProperty()
        , techniques()
        , shaders()
        , programs()
    {
        //
    }

    SKHR_techniques_webglglTFextension::operator bool() const
    {
        //TODO:
        return true;
    }

    SGlTF::SGlTF()
        : SGlTFProperty()
        , textures()
        , cameras()
        , accessors()
        , extensionsUsed()
        , samplers()
        , scenes()
        , scene(nullptr)
        , extensionsRequired()
        , meshes()
        , animations()
        , images()
        , nodes()
        , bufferViews()
        , skins()
        , materials()
        , buffers()
        , asset(nullptr)
    {
        //
    }

    SGlTF::operator bool() const
    {
        //TODO:
        return true;
    }

    SLightspot::SLightspot()
        : SGlTFProperty()
        , innerConeAngle(0.000000f)
        , outerConeAngle(0.785398f)
    {
        //
    }

    SLightspot::operator bool() const
    {
        //TODO:
        return true;
    }

}

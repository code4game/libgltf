#include "libgltf.h"

namespace libgltf
{
    SGlTFProperty::SGlTFProperty()
        : extras(nullptr)
        , extensions(nullptr)
    {
        //
    }

    SGlTFProperty::operator bool() const
    {
        //TODO:
        return true;
    }

    SGlTFChildofRootProperty::SGlTFChildofRootProperty()
        : SGlTFProperty()
        , name(L"")
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
        , alphaMode(L"OPAQUE")
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

    SAsset::SAsset()
        : SGlTFProperty()
        , minVersion(L"")
        , version(L"")
        , generator(L"")
        , copyright(L"")
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
        , input(0)
        , output(0)
        , interpolation(L"LINEAR")
    {
        //
    }

    SAnimationSampler::operator bool() const
    {
        //TODO:
        return true;
    }

    SExtras::SExtras()
    {
        //
    }

    SExtras::operator bool() const
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

    SBufferView::SBufferView()
        : SGlTFChildofRootProperty()
        , byteLength(0)
        , buffer(0)
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
        , index(0)
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
        , bufferView(0)
        , byteOffset(0)
    {
        //
    }

    SAccessorSparseValues::operator bool() const
    {
        //TODO:
        return true;
    }

    SAnimationChannelTarget::SAnimationChannelTarget()
        : SGlTFProperty()
        , node(0)
        , path(L"")
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

    SMeshPrimitive::SMeshPrimitive()
        : SGlTFProperty()
        , indices(0)
        , attributes()
        , material(0)
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

    SExtension::SExtension()
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
        , sampler(0)
    {
        //
    }

    SAnimationChannel::operator bool() const
    {
        //TODO:
        return true;
    }



    SAccessorSparseIndices::SAccessorSparseIndices()
        : SGlTFProperty()
        , componentType(0)
        , bufferView(0)
        , byteOffset(0)
    {
        //
    }

    SAccessorSparseIndices::operator bool() const
    {
        //TODO:
        return true;
    }

    SNode::SNode()
        : SGlTFChildofRootProperty()
        , scale({ 1.000000f, 1.000000f, 1.000000f })
        , rotation({ 0.000000f, 0.000000f, 0.000000f, 1.000000f })
        , matrix({ 1.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, 1.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, 1.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, 1.000000f })
        , mesh(0)
        , camera(0)
        , weights()
        , skin(0)
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
        , inverseBindMatrices(0)
        , skeleton(0)
    {
        //
    }

    SSkin::operator bool() const
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

    SCamera::SCamera()
        : SGlTFChildofRootProperty()
        , type(L"")
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
        , mimeType(L"")
        , bufferView(0)
        , uri(L"")
    {
        //
    }

    SImage::operator bool() const
    {
        //TODO:
        return true;
    }

    STexture::STexture()
        : SGlTFChildofRootProperty()
        , source(0)
        , sampler(0)
    {
        //
    }

    STexture::operator bool() const
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

    SBuffer::SBuffer()
        : SGlTFChildofRootProperty()
        , byteLength(0)
        , uri(L"")
    {
        //
    }

    SBuffer::operator bool() const
    {
        //TODO:
        return true;
    }

    SAccessor::SAccessor()
        : SGlTFChildofRootProperty()
        , count(0)
        , min()
        , max()
        , bufferView(0)
        , componentType(0)
        , byteOffset(0)
        , sparse(nullptr)
        , type(L"")
        , normalized(false)
    {
        //
    }

    SAccessor::operator bool() const
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
        , scene(0)
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

}

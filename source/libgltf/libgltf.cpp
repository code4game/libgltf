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
        //
        return false;
    }

    SGlTFChildofRootProperty::SGlTFChildofRootProperty()
        : SGlTFProperty()
        , name(L"")
    {
        //
    }

    SGlTFChildofRootProperty::operator bool() const
    {
        //
        return false;
    }

    SMaterial::SMaterial()
        : SGlTFChildofRootProperty()
        , alphaCutoff(0.0f)
        , emissiveTexture(nullptr)
        , pbrMetallicRoughness(nullptr)
        , occlusionTexture(nullptr)
        , alphaMode(L"")
        , doubleSided(false)
        , normalTexture(nullptr)
        , emissiveFactor()
    {
        //
    }

    SMaterial::operator bool() const
    {
        //
        return false;
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
        //
        return false;
    }

    SSampler::SSampler()
        : SGlTFChildofRootProperty()
        , wrapS(0)
        , minFilter(0)
        , magFilter(0)
        , wrapT(0)
    {
        //
    }

    SSampler::operator bool() const
    {
        //
        return false;
    }

    SAnimationSampler::SAnimationSampler()
        : SGlTFProperty()
        , input(0)
        , output(0)
        , interpolation(L"")
    {
        //
    }

    SAnimationSampler::operator bool() const
    {
        //
        return false;
    }

    SExtras::SExtras()
    {
        //
    }

    SExtras::operator bool() const
    {
        //
        return false;
    }

    SScene::SScene()
        : SGlTFChildofRootProperty()
        , nodes()
    {
        //
    }

    SScene::operator bool() const
    {
        //
        return false;
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
        //
        return false;
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
        //
        return false;
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
        //
        return false;
    }

    SMaterialNormalTextureInfo::SMaterialNormalTextureInfo()
        : STextureInfo()
        , scale(0.0f)
    {
        //
    }

    SMaterialNormalTextureInfo::operator bool() const
    {
        //
        return false;
    }

    SMaterialOcclusionTextureInfo::SMaterialOcclusionTextureInfo()
        : STextureInfo()
        , strength(0.0f)
    {
        //
    }

    SMaterialOcclusionTextureInfo::operator bool() const
    {
        //
        return false;
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
        //
        return false;
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
        //
        return false;
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
        //
        return false;
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
        //
        return false;
    }

    SMeshPrimitive::SMeshPrimitive()
        : SGlTFProperty()
        , indices(0)
        , attributes(0)
        , material(0)
        , mode(0)
        , targets()
    {
        //
    }

    SMeshPrimitive::operator bool() const
    {
        //
        return false;
    }

    SExtension::SExtension()
    {
        //
    }

    SExtension::operator bool() const
    {
        //
        return false;
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
        //
        return false;
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
        //
        return false;
    }

    SNode::SNode()
        : SGlTFChildofRootProperty()
        , scale()
        , rotation()
        , matrix()
        , mesh(0)
        , camera(0)
        , weights()
        , skin(0)
        , translation()
        , children()
    {
        //
    }

    SNode::operator bool() const
    {
        //
        return false;
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
        //
        return false;
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
        //
        return false;
    }

    SMaterialPBRMetallicRoughness::SMaterialPBRMetallicRoughness()
        : SGlTFProperty()
        , roughnessFactor(0.0f)
        , baseColorTexture(nullptr)
        , metallicFactor(0.0f)
        , baseColorFactor()
        , metallicRoughnessTexture(nullptr)
    {
        //
    }

    SMaterialPBRMetallicRoughness::operator bool() const
    {
        //
        return false;
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
        //
        return false;
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
        //
        return false;
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
        //
        return false;
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
        //
        return false;
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
        //
        return false;
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
        //
        return false;
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
        //
        return false;
    }

}

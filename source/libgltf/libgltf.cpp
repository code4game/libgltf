#include "libgltf.h"

#include <rapidjson/rapidjson.h>

namespace libgltf
{
    bool ParseByString(const std::wstring& _sContent, std::shared_ptr<SGLTF>& _pGLTF)
    {
        //
        return false;
    }

    SGLTF::SGLTF()
        : SProperty()
        , extensionsUsed()
        , extensionsRequired()
        , accessors()
        , animations()
        , asset(nullptr)
    {
        //
    }

    SProperty::SProperty()
        : extensions(nullptr)
        , extras(nullptr)
    {
        //
    }

    SAsset::SAsset()
        : SProperty()
        , version(L"")
        , minVersion(nullptr)
        , generator(nullptr)
        , copyright(nullptr)
    {
        //
    }

    SChildOfRootProperty::SChildOfRootProperty()
        : SProperty()
        , name(nullptr)
    {
        //
    }

    SScene::SScene()
        : SChildOfRootProperty()
        , nodes()
    {
        //
    }

    SNode::SNode()
        : SChildOfRootProperty()
    {
        //
    }

    SMaterial::SMaterial()
        : SChildOfRootProperty()
        , pbrMetallicRoughness(nullptr)
        , normalTexture(nullptr)
        , occlusionTexture(nullptr)
        , emissiveTexture(nullptr)
        , emissiveFactor(3)
        , alphaMode(L"OPAQUE")
        , alphaCutoff(0.5f)
        , doubleSided(false)
    {
        emissiveFactor[0] = 0.0f;
        emissiveFactor[1] = 0.0f;
        emissiveFactor[2] = 0.0f;
    }

    SMaterialTextureInfo::SMaterialTextureInfo()
        : SProperty()
        , index(nullptr)
        , texCoord(0)
    {
        //
    }

    SImage::SImage()
        : SChildOfRootProperty()
        , uri(L"")
        , mimeType(L"")
        , bufferView(nullptr)
    {
        //
    }

    SSampler::SSampler()
        : SChildOfRootProperty()
        , magFilter(0)
        , minFilter(0)
        , wrapS(0)
        , wrapT(0)
    {
        //
    }
}

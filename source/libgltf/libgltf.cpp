#include "libgltf.h"

#include <rapidjson/rapidjson.h>

namespace libgltf
{
    bool ParseByString(const std::wstring& _sContent, std::shared_ptr<SGLTF>& _pGLTF)
    {
        //
        return false;
    }

    bool ParseByFile(const std::wstring& _sFilePath, std::shared_ptr<SGLTF>& _pGLTF)
    {
        //
        return false;
    }

    SGLTF::SGLTF()
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

    void SAsset::Reset(SAsset& _stAsset)
    {
        new(&_stAsset)SAsset;
    }

    SChildOfRootProperty::SChildOfRootProperty()
        : SProperty()
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

}

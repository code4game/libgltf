#include "libgltf.h"

#include <rapidjson/rapidjson.h>

namespace libgltf
{
    SAsset::SAsset()
        : Generator(L"")
        , Version(L"")
    {
        //
    }

    void SAsset::Reset(SAsset& _stAsset)
    {
        new(&_stAsset)SAsset;
    }

    SScene::SScene()
    {
        //
    }

    SNode::SNode()
    {
        //
    }

    SGLTF::SGLTF()
    {
        //
    }

    bool ParseByString(const std::wstring& _sContent, SGLTF& _stGLTF)
    {
        //
        return false;
    }

    bool ParseByFile(const std::wstring& _sFilePath, SGLTF& _stGLTF)
    {
        //
        return false;
    }
}

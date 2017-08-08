#pragma once

#include <cstdint>
#include <memory>
#include <vector>
#include <string>

namespace libgltf
{
    struct SGLTF
    {
        SGLTF();

        std::shared_ptr<struct SAsset> Asset;
        std::vector<std::shared_ptr<struct SScene>> Scenes;
        std::vector<std::shared_ptr<struct SNode>> Nodes;
    };

    bool ParseByString(const std::wstring& _sContent, SGLTF& _stGLTF);
    bool ParseByFile(const std::wstring& _sFilePath, SGLTF& _stGLTF);

    struct SAsset
    {
        SAsset();

        std::wstring Generator;
        std::wstring Version;

        static void Reset(SAsset& _stAsset);
    };

    struct SScene
    {
        SScene();

        std::vector<std::shared_ptr<struct SNode>> Nodes;
    };

    struct SNode
    {
        SNode();

        std::shared_ptr<struct SCamera> Camera;
        std::shared_ptr<struct SSkin> Skin;
        std::vector<std::shared_ptr<struct SMesh>> Meshes;

        std::vector<std::shared_ptr<struct SNode>> Children;
    };

    struct SMesh
    {
        //
        std::vector<std::shared_ptr<struct SAccessor>> Accessors;
        std::vector<std::shared_ptr<struct SMaterial>> Materials;
    };

    struct SCamera
    {
        //
    };

    struct SSkin
    {
        //
        std::vector<std::shared_ptr<struct SAccessor>> Accessors;
    };

    struct SAnimation
    {
        //
        std::vector<std::shared_ptr<struct SAccessor>> Accessors;
    };

    struct SAccessor
    {
        //
        std::shared_ptr<struct SBufferView> BufferView;
    };

    struct SBufferView
    {
        //
        std::shared_ptr<struct SBuffer> Buffer;
    };

    struct SBuffer
    {
        //
    };

    struct SMaterial
    {
        //
        std::vector<std::shared_ptr<struct STexture>> Textures;
    };

    struct STexture
    {
        //
        std::shared_ptr<struct SImage> Image;
        std::shared_ptr<struct SSampler> Sampler;
    };

    struct SImage
    {
        //
    };

    struct SSampler
    {
        //
    };
}

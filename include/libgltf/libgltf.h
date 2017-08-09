#pragma once

#include <cstdint>
#include <memory>
#include <vector>
#include <string>

namespace libgltf
{
    struct SGLTF;

    bool ParseByString(const std::wstring& _sContent, std::shared_ptr<SGLTF>& _pGLTF);
    bool ParseByFile(const std::wstring& _sFilePath, std::shared_ptr<SGLTF>& _pGLTF);

    struct SGLTF
    {
        SGLTF();

        std::shared_ptr<struct SAsset> asset;
        std::shared_ptr<struct SScene> scene;
        std::vector<std::shared_ptr<struct SScene>> scenes;
        std::vector<std::shared_ptr<struct SNode>> nodes;
        std::vector<std::shared_ptr<struct SMesh>> meshes;
        std::vector<std::shared_ptr<struct SCamera>> cameras;
        std::vector<std::shared_ptr<struct SSkin>> skins;
        std::vector<std::shared_ptr<struct SAnimation>> animations;
        std::vector<std::shared_ptr<struct SAccessor>> accessors;
        std::vector<std::shared_ptr<struct SBufferView>> bufferViews;
        std::vector<std::shared_ptr<struct SBuffer>> buffers;
        std::vector<std::shared_ptr<struct SMaterial>> materials;
        std::vector<std::shared_ptr<struct STexture>> textures;
        std::vector<std::shared_ptr<struct SImage>> images;
        std::vector<std::shared_ptr<struct SSampler>> samplers;
    };

    struct SProperty
    {
        SProperty();

        std::shared_ptr<std::vector<std::shared_ptr<struct SExtension>>> extensions;
        std::shared_ptr<std::vector<std::shared_ptr<struct SExtra>>> extras;
    };

    struct SAsset : SProperty
    {
        SAsset();

        std::wstring version;
        std::shared_ptr<std::wstring> minVersion;
        std::shared_ptr<std::wstring> generator;
        std::shared_ptr<std::wstring> copyright;

        static void Reset(SAsset& _stAsset);
    };

    struct SChildOfRootProperty : SProperty
    {
        SChildOfRootProperty();

        std::shared_ptr<std::wstring> name;
    };

    struct SScene : SChildOfRootProperty
    {
        SScene();

        std::vector<std::shared_ptr<struct SNode>> nodes;
    };

    struct SNode : SChildOfRootProperty
    {
        SNode();

        std::shared_ptr<struct SCamera> camera;
        std::shared_ptr<struct SSkin> skin;
        std::vector<std::shared_ptr<struct SMesh>> meshes;

        std::vector<std::shared_ptr<struct SNode>> children;
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

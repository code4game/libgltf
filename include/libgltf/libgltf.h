#pragma once

#include <cstdint>
#include <memory>
#include <vector>
#include <string>

namespace libgltf
{
    struct SGLTF;

    bool ParseByString(const std::wstring& _sContent, std::shared_ptr<SGLTF>& _pGLTF);

    struct SProperty
    {
        SProperty();

        std::shared_ptr<std::vector<std::shared_ptr<struct SExtension>>> extensions;
        std::shared_ptr<std::vector<std::shared_ptr<struct SExtra>>> extras;
    };

    struct SGLTF : SProperty
    {
        SGLTF();

        std::vector<std::wstring> extensionsUsed;
        std::vector<std::wstring> extensionsRequired;

        std::vector<std::shared_ptr<struct SAccessor>> accessors;
        std::vector<std::shared_ptr<struct SAnimation>> animations;
        std::shared_ptr<struct SAsset> asset;
        std::vector<std::shared_ptr<struct SBuffer>> buffers;
        std::vector<std::shared_ptr<struct SBufferView>> bufferViews;
        std::vector<std::shared_ptr<struct SCamera>> cameras;
        std::vector<std::shared_ptr<struct SImage>> images;
        std::vector<std::shared_ptr<struct SMaterial>> materials;
        std::vector<std::shared_ptr<struct SMesh>> meshes;
        std::vector<std::shared_ptr<struct SNode>> nodes;
        std::vector<std::shared_ptr<struct SSampler>> samplers;
        std::shared_ptr<struct SId> scene;
        std::vector<std::shared_ptr<struct SScene>> scenes;
        std::vector<std::shared_ptr<struct SSkin>> skins;
        std::vector<std::shared_ptr<struct STexture>> textures;
    };

    struct SAsset : SProperty
    {
        SAsset();

        std::wstring version;
        std::shared_ptr<std::wstring> minVersion;
        std::shared_ptr<std::wstring> generator;
        std::shared_ptr<std::wstring> copyright;
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

    struct SMaterial : SChildOfRootProperty
    {
        SMaterial();

        std::shared_ptr<struct SMaterialPBRMetallicRoughness> pbrMetallicRoughness;
        std::shared_ptr<struct SMaterialNormalTextureInfo> normalTexture;
        std::shared_ptr<struct SMaterialOcclusionTextureInfo> occlusionTexture;
        std::shared_ptr<struct SMaterialTextureInfo> emissiveTexture;
        std::vector<float> emissiveFactor;
        std::wstring alphaMode;
        float alphaCutoff;
        bool doubleSided;
    };

    struct SMaterialPBRMetallicRoughness
    {
        //
    };

    struct SMaterialNormalTextureInfo
    {
        //
    };

    struct SMaterialOcclusionTextureInfo
    {
        //
    };

    struct SMaterialTextureInfo : SProperty
    {
        SMaterialTextureInfo();

        std::shared_ptr<struct SId> index;
        int32_t texCoord;
    };

    struct STexture
    {
        //
        std::shared_ptr<struct SImage> Image;
        std::shared_ptr<struct SSampler> Sampler;
    };

    struct SImage : SChildOfRootProperty
    {
        SImage();

        std::wstring uri;
        std::wstring mimeType;
        std::shared_ptr<SBufferView> bufferView;
    };

    struct SSampler : SChildOfRootProperty
    {
        SSampler();

        int32_t magFilter;
        int32_t minFilter;
        int32_t wrapS;
        int32_t wrapT;
    };
}

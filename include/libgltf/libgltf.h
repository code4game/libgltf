#pragma once

#include <stdint.h>
#include <vector>
#include <map>
#include <string>
#include <memory>

namespace libgltf
{
    struct SGlTF;
    bool ParseByString(const std::wstring& _sContent, std::shared_ptr<SGlTF>& _pGlTF);

    /*!
     * struct: SGlTFProperty
     */
    struct SGlTFProperty
    {
        SGlTFProperty();

        // Check valid
        operator bool() const;

        std::shared_ptr<struct SExtras> extras;
        std::shared_ptr<struct SExtension> extensions;
    };

    /*!
     * struct: SGlTFChildofRootProperty
     */
    struct SGlTFChildofRootProperty : SGlTFProperty
    {
        SGlTFChildofRootProperty();

        // Check valid
        operator bool() const;

        // The user-defined name of this object.
        std::wstring name;
    };

    /*!
     * struct: SMaterial
     * The material appearance of a primitive.
     */
    struct SMaterial : SGlTFChildofRootProperty
    {
        SMaterial();

        // Check valid
        operator bool() const;

        // The alpha cutoff value of the material.
        float alphaCutoff;
        // The emissive map texture.
        std::shared_ptr<struct STextureInfo> emissiveTexture;
        // A set of parameter values that are used to define the metallic-roughness material model from Physically-Based Rendering (PBR) methodology. When not specified, all the default values of `pbrMetallicRoughness` apply.
        std::shared_ptr<struct SMaterialPBRMetallicRoughness> pbrMetallicRoughness;
        // The occlusion map texture.
        std::shared_ptr<struct SMaterialOcclusionTextureInfo> occlusionTexture;
        // The alpha rendering mode of the material.
        std::wstring alphaMode;
        // Specifies whether the material is double sided.
        bool doubleSided;
        // The normal map texture.
        std::shared_ptr<struct SMaterialNormalTextureInfo> normalTexture;
        // The emissive color of the material.
        std::vector<float> emissiveFactor;
    };

    /*!
     * struct: SAsset
     * Metadata about the glTF asset.
     */
    struct SAsset : SGlTFProperty
    {
        SAsset();

        // Check valid
        operator bool() const;

        // The minimum glTF version that this asset targets.
        std::wstring minVersion;
        // The glTF version that this asset targets.
        std::wstring version;
        // Tool that generated this glTF model.  Useful for debugging.
        std::wstring generator;
        // A copyright message suitable for display to credit the content creator.
        std::wstring copyright;
    };

    /*!
     * struct: SSampler
     * Texture sampler properties for filtering and wrapping modes.
     */
    struct SSampler : SGlTFChildofRootProperty
    {
        SSampler();

        // Check valid
        operator bool() const;

        // s wrapping mode.
        int32_t wrapS;
        // Minification filter.
        int32_t minFilter;
        // Magnification filter.
        int32_t magFilter;
        // t wrapping mode.
        int32_t wrapT;
    };

    /*!
     * struct: SAnimationSampler
     * Combines input and output accessors with an interpolation algorithm to define a keyframe graph (but not its target).
     */
    struct SAnimationSampler : SGlTFProperty
    {
        SAnimationSampler();

        // Check valid
        operator bool() const;

        // The index of an accessor containing keyframe input values, e.g., time.
        int32_t input;
        // The index of an accessor, containing keyframe output values.
        int32_t output;
        // Interpolation algorithm.
        std::wstring interpolation;
    };

    /*!
     * struct: SExtras
     * Application-specific data.
     */
    struct SExtras
    {
        SExtras();

        // Check valid
        operator bool() const;

    };

    /*!
     * struct: SScene
     * The root nodes of a scene.
     */
    struct SScene : SGlTFChildofRootProperty
    {
        SScene();

        // Check valid
        operator bool() const;

        // The indices of each root node.
        std::vector<int32_t> nodes;
    };

    /*!
     * struct: SCameraPerspective
     * A perspective camera containing properties to create a perspective projection matrix.
     */
    struct SCameraPerspective : SGlTFProperty
    {
        SCameraPerspective();

        // Check valid
        operator bool() const;

        // The floating-point aspect ratio of the field of view.
        float aspectRatio;
        // The floating-point vertical field of view in radians.
        float yfov;
        // The floating-point distance to the near clipping plane.
        float znear;
        // The floating-point distance to the far clipping plane.
        float zfar;
    };

    /*!
     * struct: SBufferView
     * A view into a buffer generally representing a subset of the buffer.
     */
    struct SBufferView : SGlTFChildofRootProperty
    {
        SBufferView();

        // Check valid
        operator bool() const;

        // The length of the bufferView in bytes.
        int32_t byteLength;
        // The index of the buffer.
        int32_t buffer;
        // The offset into the buffer in bytes.
        int32_t byteOffset;
        // The target that the GPU buffer should be bound to.
        int32_t target;
        // The stride, in bytes.
        int32_t byteStride;
    };

    /*!
     * struct: STextureInfo
     * Reference to a texture.
     */
    struct STextureInfo : SGlTFProperty
    {
        STextureInfo();

        // Check valid
        operator bool() const;

        // The index of the texture.
        int32_t index;
        // The set index of texture's TEXCOORD attribute used for texture coordinate mapping.
        int32_t texCoord;
    };

    /*!
     * struct: SMaterialNormalTextureInfo
     */
    struct SMaterialNormalTextureInfo : STextureInfo
    {
        SMaterialNormalTextureInfo();

        // Check valid
        operator bool() const;

        // The scalar multiplier applied to each normal vector of the normal texture.
        float scale;
    };

    /*!
     * struct: SMaterialOcclusionTextureInfo
     */
    struct SMaterialOcclusionTextureInfo : STextureInfo
    {
        SMaterialOcclusionTextureInfo();

        // Check valid
        operator bool() const;

        // A scalar multiplier controlling the amount of occlusion applied.
        float strength;
    };

    /*!
     * struct: SAccessorSparseValues
     * Array of size `accessor.sparse.count` times number of components storing the displaced accessor attributes pointed by `accessor.sparse.indices`.
     */
    struct SAccessorSparseValues : SGlTFProperty
    {
        SAccessorSparseValues();

        // Check valid
        operator bool() const;

        // The index of the bufferView with sparse values. Referenced bufferView can't have ARRAY_BUFFER or ELEMENT_ARRAY_BUFFER target.
        int32_t bufferView;
        // The offset relative to the start of the bufferView in bytes. Must be aligned.
        int32_t byteOffset;
    };

    /*!
     * struct: SAnimationChannelTarget
     * The index of the node and TRS property that an animation channel targets.
     */
    struct SAnimationChannelTarget : SGlTFProperty
    {
        SAnimationChannelTarget();

        // Check valid
        operator bool() const;

        // The index of the node to target.
        int32_t node;
        // The name of the node's TRS property to modify, or the "weights" of the Morph Targets it instantiates.
        std::wstring path;
    };

    /*!
     * struct: SMesh
     * A set of primitives to be rendered.  A node can contain one mesh.  A node's transform places the mesh in the scene.
     */
    struct SMesh : SGlTFChildofRootProperty
    {
        SMesh();

        // Check valid
        operator bool() const;

        // An array of primitives, each defining geometry to be rendered with a material.
        std::vector<std::shared_ptr<struct SMeshPrimitive>> primitives;
        // Array of weights to be applied to the Morph Targets.
        std::vector<float> weights;
    };

    /*!
     * struct: SAccessorSparse
     * Sparse storage of attributes that deviate from th
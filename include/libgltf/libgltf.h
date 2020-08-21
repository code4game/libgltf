/*
 * This software is released under the MIT license.
 *
 * Copyright (c) 2017-2019 Alex Chi, The Code 4 Game Organization
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

#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <map>
#include <memory>

#define LIBGLTF_MAJOR_VERSION    0
#define LIBGLTF_MINOR_VERSION    1
#define LIBGLTF_PATCH_VERSION    6

namespace libgltf
{
#if defined(LIBGLTF_CHARACTOR_ENCODING_IS_UTF16)
#   define      GLTFTEXT(t)             u##t
    typedef std::u16string              string_t;
#elif defined(LIBGLTF_CHARACTOR_ENCODING_IS_UTF32)
#   define      GLTFTEXT(t)             U##t
    typedef std::u32string              string_t;
#elif defined(LIBGLTF_CHARACTOR_ENCODING_IS_UNICODE)
#   define      GLTFTEXT(t)             L##t
    typedef std::wstring                string_t;
#else
#   define      GLTFTEXT(t)             t
    typedef std::string                 string_t;
#endif

    struct SGlTF;
    bool operator<<(std::shared_ptr<SGlTF>& _pGlTF, const string_t& _sContent);
    bool operator>>(const std::shared_ptr<SGlTF>& _pGlTF, string_t& _sContent);
    
    /*!
     * struct: SObject
     */
    struct SObject
    {
        SObject();
        string_t schemaType;
    };

    /*!
     * struct: SGlTFProperty
     */
    struct SGlTFProperty : SObject
    {
        SGlTFProperty();

        // Check valid
        operator bool() const;

        std::shared_ptr<struct SExtras> extras;
        std::shared_ptr<struct SExtension> extensions;
    };

    /*!
     * struct: SKHR_techniques_webglmaterialextension
     * The technique to use for a material and any additional uniform values.
     */
    struct SKHR_techniques_webglmaterialextension : SGlTFProperty
    {
        SKHR_techniques_webglmaterialextension();

        // Check valid
        operator bool() const;

        // The index of the technique.
        std::shared_ptr<struct SGlTFId> technique;
        // Dictionary object of uniform values.
        std::map<string_t, std::shared_ptr<struct SUniformValue>> values;
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
        string_t name;
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
        string_t alphaMode;
        // Specifies whether the material is double sided.
        bool doubleSided;
        // The normal map texture.
        std::shared_ptr<struct SMaterialNormalTextureInfo> normalTexture;
        // The emissive color of the material.
        std::vector<float> emissiveFactor;
    };

    /*!
     * struct: SKHR_texture_transformtextureInfoextension
     * glTF extension that enables shifting and scaling UV coordinates on a per-texture basis
     */
    struct SKHR_texture_transformtextureInfoextension : SGlTFProperty
    {
        SKHR_texture_transformtextureInfoextension();

        // Check valid
        operator bool() const;

        // Rotate the UVs by this many radians counter-clockwise around the origin.
        float rotation;
        // The scale factor applied to the components of the UV coordinates.
        std::vector<float> scale;
        // Overrides the textureInfo texCoord value if supplied, and if this extension is supported.
        int32_t texCoord;
        // The offset of the UV coordinate origin as a factor of the texture dimensions.
        std::vector<float> offset;
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
        string_t minVersion;
        // The glTF version that this asset targets.
        string_t version;
        // Tool that generated this glTF model.  Useful for debugging.
        string_t generator;
        // A copyright message suitable for display to credit the content creator.
        string_t copyright;
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
        std::shared_ptr<struct SGlTFId> input;
        // The index of an accessor, containing keyframe output values.
        std::shared_ptr<struct SGlTFId> output;
        // Interpolation algorithm.
        string_t interpolation;
    };

    /*!
     * struct: SExtras
     * Application-specific data.
     */
    struct SExtras : SObject
    {
        SExtras();

        // Check valid
        operator bool() const;
    };

    /*!
     * struct: SADOBE_materials_thin_transparencyglTFextension
     * glTF extension that defines properties to model physically plausible optical transparency.
     */
    struct SADOBE_materials_thin_transparencyglTFextension : SGlTFProperty
    {
        SADOBE_materials_thin_transparencyglTFextension();

        // Check valid
        operator bool() const;

        // The index of refraction of the material.
        float ior;
        // A greyscale texture that defines the transmission percentage of the surface. This will be multiplied by transmissionFactor.
        std::shared_ptr<struct STextureInfo> transmissionTexture;
        // The base percentage of light transmitted through the surface.
        float transmissionFactor;
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
        std::vector<std::shared_ptr<struct SGlTFId>> nodes;
    };

    /*!
     * struct: STechnique
     * A template for material appearances.
     */
    struct STechnique : SGlTFChildofRootProperty
    {
        STechnique();

        // Check valid
        operator bool() const;

        // The index of the program.
        std::shared_ptr<struct SGlTFId> program;
        // A dictionary object of `Uniform` objects.
        std::map<string_t, std::shared_ptr<struct SUniform>> uniforms;
        // A dictionary object of `Attribute` objects.
        std::map<string_t, std::shared_ptr<struct SAttribute>> attributes;
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

        // The total byte length of the buffer view.
        int32_t byteLength;
        // The index of the buffer.
        std::shared_ptr<struct SGlTFId> buffer;
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
        std::shared_ptr<struct SGlTFId> index;
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
     * struct: SSolarPanelGroup
     * A solar panel group definition.
     */
    struct SSolarPanelGroup : SGlTFProperty
    {
        SSolarPanelGroup();

        // Check valid
        operator bool() const;

        // The percentage, from 0.0 to 100.0, of how efficiently the solar cells convert solar to electrical energy.
        float efficiency;
        // The name of this solar panel group.  The group name must be unique within this model, and may not contain spaces.
        string_t name;
    };

    /*!
     * struct: SKHR_materials_clearcoatglTFextension
     * glTF extension that defines the clearcoat material layer.
     */
    struct SKHR_materials_clearcoatglTFextension : SGlTFProperty
    {
        SKHR_materials_clearcoatglTFextension();

        // Check valid
        operator bool() const;

        // The clearcoat layer roughness texture.
        std::shared_ptr<struct STextureInfo> clearcoatRoughnessTexture;
        // The clearcoat layer intensity.
        float clearcoatFactor;
        // The clearcoat layer intensity texture.
        std::shared_ptr<struct STextureInfo> clearcoatTexture;
        // The clearcoat normal map texture.
        std::shared_ptr<struct SMaterialNormalTextureInfo> clearcoatNormalTexture;
        // The clearcoat layer roughness.
        float clearcoatRoughnessFactor;
    };

    /*!
     * struct: SUniform
     * A uniform input to a technique, and an optional semantic and value.
     */
    struct SUniform : SGlTFProperty
    {
        SUniform();

        // Check valid
        operator bool() const;

        // When defined, the uniform is an array of count elements of the specified type.  Otherwise, the uniform is not an array.
        int32_t count;
        // The index of the node whose transform is used as the uniform's value.
        std::shared_ptr<struct SGlTFId> node;
        // Identifies a uniform with a well-known meaning.
        string_t semantic;
        // The value of the uniform.
        std::shared_ptr<struct SUniformValue> value;
        // The uniform type.
        int32_t type;
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
        std::shared_ptr<struct SGlTFId> bufferView;
        // The offset relative to the start of the bufferView in bytes. Must be aligned.
        int32_t byteOffset;
    };

    /*!
     * struct: SMSFT_lodglTFextension
     * glTF extension for specifying levels of detail (LOD).
     */
    struct SMSFT_lodglTFextension : SGlTFProperty
    {
        SMSFT_lodglTFextension();

        // Check valid
        operator bool() const;

        // Array containing the indices of progressively lower LOD nodes.
        std::vector<int32_t> ids;
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
        std::shared_ptr<struct SGlTFId> node;
        // The name of the node's TRS property to modify, or the "weights" of the Morph Targets it instantiates. For the "translation" property, the values that are provided by the sampler are the translation along the x, y, and z axes. For the "rotation" property, the values are a quaternion in the order (x, y, z, w), where w is the scalar. For the "scale" property, the values are the scaling factors along the x, y, and z axes.
        string_t path;
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
     * Sparse storage of attributes that deviate from their initialization value.
     */
    struct SAccessorSparse : SGlTFProperty
    {
        SAccessorSparse();

        // Check valid
        operator bool() const;

        // Number of entries stored in the sparse array.
        int32_t count;
        // Index array of size `count` that points to those accessor attributes that deviate from their initialization value. Indices must strictly increase.
        std::shared_ptr<struct SAccessorSparseIndices> indices;
        // Array of size `count` times number of components, storing the displaced accessor attributes pointed by `indices`. Substituted values must have the same `componentType` and number of components as the base accessor.
        std::shared_ptr<struct SAccessorSparseValues> values;
    };

    /*!
     * struct: SShader
     * A vertex or fragment shader. Exactly one of `uri` or `bufferView` must be provided for the GLSL source.
     */
    struct SShader : SGlTFChildofRootProperty
    {
        SShader();

        // Check valid
        operator bool() const;

        // The index of the bufferView that contains the GLSL shader source. Use this instead of the shader's uri property.
        std::shared_ptr<struct SGlTFId> bufferView;
        // The shader stage.
        int32_t type;
        // The uri of the GLSL source.
        string_t uri;
    };

    /*!
     * struct: SMeshPrimitive
     * Geometry to be rendered with the given material.
     */
    struct SMeshPrimitive : SGlTFProperty
    {
        SMeshPrimitive();

        // Check valid
        operator bool() const;

        // The index of the accessor that contains the indices.
        std::shared_ptr<struct SGlTFId> indices;
        // A dictionary object, where each key corresponds to mesh attribute semantic and each value is the index of the accessor containing attribute's data.
        std::map<string_t, std::shared_ptr<struct SGlTFId>> attributes;
        // The index of the material to apply to this primitive when rendering.
        std::shared_ptr<struct SGlTFId> material;
        // The type of primitives to render.
        int32_t mode;
        // An array of Morph Targets, each  Morph Target is a dictionary mapping attributes (only `POSITION`, `NORMAL`, and `TANGENT` supported) to their deviations in the Morph Target.
        std::vector<std::shared_ptr<struct SGlTFId>> targets;
    };

    /*!
     * struct: SArticulationStage
     * One stage of a model articulation definition.
     */
    struct SArticulationStage : SGlTFProperty
    {
        SArticulationStage();

        // Check valid
        operator bool() const;

        // The maximum value for the range of motion of this articulation stage.
        float maximumValue;
        // The initial value for this articulation stage.
        float initialValue;
        // The minimum value for the range of motion of this articulation stage.
        float minimumValue;
        // The name of this articulation stage.  The articulation stage name must be unique only within the containing articulation.  Articulation Stage names may not contain spaces.
        string_t name;
        // The type of motion applied by this articulation stage.
        string_t type;
    };

    /*!
     * struct: SKHR_materials_pbrSpecularGlossinessglTFextension
     * glTF extension that defines the specular-glossiness material model from Physically-Based Rendering (PBR) methodology.
     */
    struct SKHR_materials_pbrSpecularGlossinessglTFextension : SGlTFProperty
    {
        SKHR_materials_pbrSpecularGlossinessglTFextension();

        // Check valid
        operator bool() const;

        // The specular RGB color of the material.
        std::vector<float> specularFactor;
        // The glossiness or smoothness of the material.
        float glossinessFactor;
        // The diffuse texture.
        std::shared_ptr<struct STextureInfo> diffuseTexture;
        // The specular-glossiness texture.
        std::shared_ptr<struct STextureInfo> specularGlossinessTexture;
        // The reflected diffuse factor of the material.
        std::vector<float> diffuseFactor;
    };

    /*!
     * struct: SExtension
     * Dictionary object with extension-specific objects.
     */
    struct SExtension : SObject
    {
        SExtension();

        // Check valid
        operator bool() const;

        // Manual code lines
        std::map<string_t, std::shared_ptr<struct SObject>> properties;
    };

    /*!
     * struct: SAnimationChannel
     * Targets an animation's sampler at a node's property.
     */
    struct SAnimationChannel : SGlTFProperty
    {
        SAnimationChannel();

        // Check valid
        operator bool() const;

        // The index of the node and TRS property to target.
        std::shared_ptr<struct SAnimationChannelTarget> target;
        // The index of a sampler in this animation used to compute the value for the target.
        std::shared_ptr<struct SGlTFId> sampler;
    };

    /*!
     * struct: SGlTFId
     */
    struct SGlTFId : SObject
    {
        SGlTFId();

        // Check valid
        operator bool() const;

        operator int32_t() const;

        int32_t int32_tValue;
    };

    /*!
     * struct: SAccessorSparseIndices
     * Indices of those attributes that deviate from their initialization value.
     */
    struct SAccessorSparseIndices : SGlTFProperty
    {
        SAccessorSparseIndices();

        // Check valid
        operator bool() const;

        // The indices data type.
        int32_t componentType;
        // The index of the bufferView with sparse indices. Referenced bufferView can't have ARRAY_BUFFER or ELEMENT_ARRAY_BUFFER target.
        std::shared_ptr<struct SGlTFId> bufferView;
        // The offset relative to the start of the bufferView in bytes. Must be aligned.
        int32_t byteOffset;
    };

    /*!
     * struct: SNode
     * A node in the node hierarchy.  When the node contains `skin`, all `mesh.primitives` must contain `JOINTS_0` and `WEIGHTS_0` attributes.  A node can have either a `matrix` or any combination of `translation`/`rotation`/`scale` (TRS) properties. TRS properties are converted to matrices and postmultiplied in the `T * R * S` order to compose the transformation matrix; first the scale is applied to the vertices, then the rotation, and then the translation. If none are provided, the transform is the identity. When a node is targeted for animation (referenced by an animation.channel.target), only TRS properties may be present; `matrix` will not be present.
     */
    struct SNode : SGlTFChildofRootProperty
    {
        SNode();

        // Check valid
        operator bool() const;

        // The node's non-uniform scale, given as the scaling factors along the x, y, and z axes.
        std::vector<float> scale;
        // The node's unit quaternion rotation in the order (x, y, z, w), where w is the scalar.
        std::vector<float> rotation;
        // A floating-point 4x4 transformation matrix stored in column-major order.
        std::vector<float> matrix;
        // The index of the mesh in this node.
        std::shared_ptr<struct SGlTFId> mesh;
        // The index of the camera referenced by this node.
        std::shared_ptr<struct SGlTFId> camera;
        // The weights of the instantiated Morph Target. Number of elements must match number of Morph Targets of used mesh.
        std::vector<float> weights;
        // The index of the skin referenced by this node.
        std::shared_ptr<struct SGlTFId> skin;
        // The node's translation along the x, y, and z axes.
        std::vector<float> translation;
        // The indices of this node's children.
        std::vector<std::shared_ptr<struct SGlTFId>> children;
    };

    /*!
     * struct: SAnimation
     * A keyframe animation.
     */
    struct SAnimation : SGlTFChildofRootProperty
    {
        SAnimation();

        // Check valid
        operator bool() const;

        // An array of channels, each of which targets an animation's sampler at a node's property. Different channels of the same animation can't have equal targets.
        std::vector<std::shared_ptr<struct SAnimationChannel>> channels;
        // An array of samplers that combines input and output accessors with an interpolation algorithm to define a keyframe graph (but not its target).
        std::vector<std::shared_ptr<struct SAnimationSampler>> samplers;
    };

    /*!
     * struct: SSkin
     * Joints and matrices defining a skin.
     */
    struct SSkin : SGlTFChildofRootProperty
    {
        SSkin();

        // Check valid
        operator bool() const;

        // Indices of skeleton nodes, used as joints in this skin.
        std::vector<std::shared_ptr<struct SGlTFId>> joints;
        // The index of the accessor containing the floating-point 4x4 inverse-bind matrices.  The default is that each matrix is a 4x4 identity matrix, which implies that inverse-bind matrices were pre-applied.
        std::shared_ptr<struct SGlTFId> inverseBindMatrices;
        // The index of the node used as a skeleton root.
        std::shared_ptr<struct SGlTFId> skeleton;
    };

    /*!
     * struct: SMSFT_texture_ddsextension
     * glTF extension to specify textures using the DirectDraw Surface file format (DDS).
     */
    struct SMSFT_texture_ddsextension : SGlTFProperty
    {
        SMSFT_texture_ddsextension();

        // Check valid
        operator bool() const;

        // The index of the images node which points to a DDS texture file.
        std::shared_ptr<struct SGlTFId> source;
    };

    /*!
     * struct: SAGI_articulationsglTFextension
     * glTF Extension that defines metadata for applying external analysis or effects to a model.
     */
    struct SAGI_articulationsglTFextension : SGlTFProperty
    {
        SAGI_articulationsglTFextension();

        // Check valid
        operator bool() const;

        // An array of articulations.
        std::vector<std::shared_ptr<struct SArticulation>> articulations;
    };

    /*!
     * struct: SAttribute
     * An attribute input to a technique and the corresponding semantic.
     */
    struct SAttribute : SGlTFProperty
    {
        SAttribute();

        // Check valid
        operator bool() const;

        // Identifies a mesh attribute semantic.
        string_t semantic;
    };

    /*!
     * struct: SMaterialPBRMetallicRoughness
     * A set of parameter values that are used to define the metallic-roughness material model from Physically-Based Rendering (PBR) methodology.
     */
    struct SMaterialPBRMetallicRoughness : SGlTFProperty
    {
        SMaterialPBRMetallicRoughness();

        // Check valid
        operator bool() const;

        // The roughness of the material.
        float roughnessFactor;
        // The base color texture.
        std::shared_ptr<struct STextureInfo> baseColorTexture;
        // The metalness of the material.
        float metallicFactor;
        // The material's base color factor.
        std::vector<float> baseColorFactor;
        // The metallic-roughness texture.
        std::shared_ptr<struct STextureInfo> metallicRoughnessTexture;
    };

    /*!
     * struct: SKHR_draco_mesh_compressionextension
     */
    struct SKHR_draco_mesh_compressionextension : SGlTFProperty
    {
        SKHR_draco_mesh_compressionextension();

        // Check valid
        operator bool() const;

        // A dictionary object, where each key corresponds to an attribute and its unique attribute id stored in the compressed geometry.
        std::map<string_t, std::shared_ptr<struct SGlTFId>> attributes;
        // The index of the bufferView.
        std::shared_ptr<struct SGlTFId> bufferView;
    };

    /*!
     * struct: SCamera
     * A camera's projection.  A node can reference a camera to apply a transform to place the camera in the scene.
     */
    struct SCamera : SGlTFChildofRootProperty
    {
        SCamera();

        // Check valid
        operator bool() const;

        // Specifies if the camera uses a perspective or orthographic projection.
        string_t type;
        // A perspective camera containing properties to create a perspective projection matrix.
        std::shared_ptr<struct SCameraPerspective> perspective;
        // An orthographic camera containing properties to create an orthographic projection matrix.
        std::shared_ptr<struct SCameraOrthographic> orthographic;
    };

    /*!
     * struct: SImage
     * Image data used to create a texture. Image can be referenced by URI or `bufferView` index. `mimeType` is required in the latter case.
     */
    struct SImage : SGlTFChildofRootProperty
    {
        SImage();

        // Check valid
        operator bool() const;

        // The image's MIME type. Required if `bufferView` is defined.
        string_t mimeType;
        // The index of the bufferView that contains the image. Use this instead of the image's uri property.
        std::shared_ptr<struct SGlTFId> bufferView;
        // The uri of the image.
        string_t uri;
    };

    /*!
     * struct: SAGI_stk_metadataglTFNodeextension
     * glTF Extension for an individual node in a glTF model, to associate it with the model's root AGI_stk_metadata object.
     */
    struct SAGI_stk_metadataglTFNodeextension : SGlTFProperty
    {
        SAGI_stk_metadataglTFNodeextension();

        // Check valid
        operator bool() const;

        // The name of a Solar Panel Group that includes this node.
        string_t solarPanelGroupName;
        // Set to true to indicate that this node's geometry does not obscure any sensors' view in the STK Sensor Obscuration tool.
        bool noObscuration;
    };

    /*!
     * struct: STexture
     * A texture and its sampler.
     */
    struct STexture : SGlTFChildofRootProperty
    {
        STexture();

        // Check valid
        operator bool() const;

        // The index of the image used by this texture. When undefined, it is expected that an extension or other mechanism will supply an alternate texture source, otherwise behavior is undefined.
        std::shared_ptr<struct SGlTFId> source;
        // The index of the sampler used by this texture. When undefined, a sampler with repeat wrapping and auto filtering should be used.
        std::shared_ptr<struct SGlTFId> sampler;
    };

    /*!
     * struct: SAGI_articulationsglTFNodeextension
     * glTF Extension for an individual node in a glTF model, to associate it with the model's root AGI_articulations object.
     */
    struct SAGI_articulationsglTFNodeextension : SGlTFProperty
    {
        SAGI_articulationsglTFNodeextension();

        // Check valid
        operator bool() const;

        // The name of an Articulation that applies to this node.
        string_t articulationName;
        // Set to true to indicate that this node's origin and orientation act as an attach point for external objects, analysis, or effects.
        bool isAttachPoint;
    };

    /*!
     * struct: SKHR_materials_unlitglTFextension
     * glTF extension that defines the unlit material model.
     */
    struct SKHR_materials_unlitglTFextension : SGlTFProperty
    {
        SKHR_materials_unlitglTFextension();

        // Check valid
        operator bool() const;
    };

    /*!
     * struct: SCameraOrthographic
     * An orthographic camera containing properties to create an orthographic projection matrix.
     */
    struct SCameraOrthographic : SGlTFProperty
    {
        SCameraOrthographic();

        // Check valid
        operator bool() const;

        // The floating-point horizontal magnification of the view. Must not be zero.
        float xmag;
        // The floating-point vertical magnification of the view. Must not be zero.
        float ymag;
        // The floating-point distance to the far clipping plane. `zfar` must be greater than `znear`.
        float zfar;
        // The floating-point distance to the near clipping plane.
        float znear;
    };

    /*!
     * struct: SBuffer
     * A buffer points to binary geometry, animation, or skins.
     */
    struct SBuffer : SGlTFChildofRootProperty
    {
        SBuffer();

        // Check valid
        operator bool() const;

        // The length of the buffer in bytes.
        int32_t byteLength;
        // The uri of the buffer.
        string_t uri;
    };

    /*!
     * struct: SUniformValue
     */
    struct SUniformValue : SObject
    {
        SUniformValue();

        // Check valid
        operator bool() const;
    };

    /*!
     * struct: SAGI_stk_metadataglTFextension
     * glTF Extension that defines metadata for use with STK (Systems Tool Kit).
     */
    struct SAGI_stk_metadataglTFextension : SGlTFProperty
    {
        SAGI_stk_metadataglTFextension();

        // Check valid
        operator bool() const;

        // An array of solar panel groups.
        std::vector<std::shared_ptr<struct SSolarPanelGroup>> solarPanelGroups;
    };

    /*!
     * struct: SProgram
     * A shader program, including its vertex and fragment shaders.
     */
    struct SProgram : SGlTFChildofRootProperty
    {
        SProgram();

        // Check valid
        operator bool() const;

        // The index of the vertex shader.
        std::shared_ptr<struct SGlTFId> vertexShader;
        // The names of required WebGL 1.0 extensions.
        std::vector<string_t> glExtensions;
        // The index of the fragment shader.
        std::shared_ptr<struct SGlTFId> fragmentShader;
    };

    /*!
     * struct: SArticulation
     * A model articulation definition.
     */
    struct SArticulation : SGlTFProperty
    {
        SArticulation();

        // Check valid
        operator bool() const;

        // An array of stages, each of which defines a degree of freedom of movement.
        std::vector<std::shared_ptr<struct SArticulationStage>> stages;
        // The name of this articulation.  The articulation name must be unique within this model.  Articulation names may not contain spaces.
        string_t name;
        // The local forward vector for the associated node, for the purpose of pointing at a target or other object.
        std::vector<float> pointingVector;
    };

    /*!
     * struct: SAccessor
     * A typed view into a bufferView.  A bufferView contains raw binary data.  An accessor provides a typed view into a bufferView or a subset of a bufferView similar to how WebGL's `vertexAttribPointer()` defines an attribute in a buffer.
     */
    struct SAccessor : SGlTFChildofRootProperty
    {
        SAccessor();

        // Check valid
        operator bool() const;

        // The number of attributes referenced by this accessor.
        int32_t count;
        // Minimum value of each component in this attribute.
        std::vector<float> min;
        // Maximum value of each component in this attribute.
        std::vector<float> max;
        // The index of the bufferView.
        std::shared_ptr<struct SGlTFId> bufferView;
        // The datatype of components in the attribute.
        int32_t componentType;
        // The offset relative to the start of the bufferView in bytes.
        int32_t byteOffset;
        // Sparse storage of attributes that deviate from their initialization value.
        std::shared_ptr<struct SAccessorSparse> sparse;
        // Specifies if the attribute is a scalar, vector, or matrix.
        string_t type;
        // Specifies whether integer data values should be normalized.
        bool normalized;
    };

    /*!
     * struct: SEXT_texture_webpglTFextension
     * glTF extension to specify textures using the WebP image format.
     */
    struct SEXT_texture_webpglTFextension : SGlTFProperty
    {
        SEXT_texture_webpglTFextension();

        // Check valid
        operator bool() const;

        // The index of the images node which points to a WebP image.
        std::shared_ptr<struct SGlTFId> source;
    };

    /*!
     * struct: SKHR_techniques_webglglTFextension
     * Instances of shading techniques with external shader programs along with their parameterized values.  Shading techniques describe data types and semantics for GLSL vertex and fragment shader programs.
     */
    struct SKHR_techniques_webglglTFextension : SGlTFProperty
    {
        SKHR_techniques_webglglTFextension();

        // Check valid
        operator bool() const;

        // An array of `Technique` objects.
        std::vector<std::shared_ptr<struct STechnique>> techniques;
        // An array of `Shader` objects.
        std::vector<std::shared_ptr<struct SShader>> shaders;
        // An array of `Program` objects.
        std::vector<std::shared_ptr<struct SProgram>> programs;
    };

    /*!
     * struct: SGlTF
     * The root object for a glTF asset.
     */
    struct SGlTF : SGlTFProperty
    {
        SGlTF();

        // Check valid
        operator bool() const;

        // An array of textures.
        std::vector<std::shared_ptr<struct STexture>> textures;
        // An array of cameras.
        std::vector<std::shared_ptr<struct SCamera>> cameras;
        // An array of accessors.
        std::vector<std::shared_ptr<struct SAccessor>> accessors;
        // Names of glTF extensions used somewhere in this asset.
        std::vector<string_t> extensionsUsed;
        // An array of samplers.
        std::vector<std::shared_ptr<struct SSampler>> samplers;
        // An array of scenes.
        std::vector<std::shared_ptr<struct SScene>> scenes;
        // The index of the default scene.
        std::shared_ptr<struct SGlTFId> scene;
        // Names of glTF extensions required to properly load this asset.
        std::vector<string_t> extensionsRequired;
        // An array of meshes.
        std::vector<std::shared_ptr<struct SMesh>> meshes;
        // An array of keyframe animations.
        std::vector<std::shared_ptr<struct SAnimation>> animations;
        // An array of images.
        std::vector<std::shared_ptr<struct SImage>> images;
        // An array of nodes.
        std::vector<std::shared_ptr<struct SNode>> nodes;
        // An array of bufferViews.
        std::vector<std::shared_ptr<struct SBufferView>> bufferViews;
        // An array of skins.
        std::vector<std::shared_ptr<struct SSkin>> skins;
        // An array of materials.
        std::vector<std::shared_ptr<struct SMaterial>> materials;
        // An array of buffers.
        std::vector<std::shared_ptr<struct SBuffer>> buffers;
        // Metadata about the glTF asset.
        std::shared_ptr<struct SAsset> asset;
    };

    enum class EAccessorComponentType : uint32_t
    {
        NONE,
        BYTE,
        UNSIGNED_BYTE,
        SHORT,
        UNSIGNED_SHORT,
        INT,
        UNSIGNED_INT,
        FLOAT,
        MAX,
    };
    
    template<typename TType>
    class TComponentData
    {
    public:
        typedef TType value_type;

    public:
        operator EAccessorComponentType() const
        {
            return EAccessorComponentType::NONE;
        }
    };

#define LIBGLTF_ACCESSORCOMPONENTDATA(TType, EType)\
    template<>\
    class TComponentData<TType>\
    {\
    public:\
        operator EAccessorComponentType() const\
        {\
            return EType;\
        }\
        template<typename TTypeAnother>\
        bool operator==(TComponentData<TTypeAnother> _Another) const\
        {\
            return (EAccessorComponentType(_Another) == EType);\
        }\
    }
    LIBGLTF_ACCESSORCOMPONENTDATA(int8_t    , EAccessorComponentType::BYTE          );
    LIBGLTF_ACCESSORCOMPONENTDATA(uint8_t   , EAccessorComponentType::UNSIGNED_BYTE );
    LIBGLTF_ACCESSORCOMPONENTDATA(int16_t   , EAccessorComponentType::SHORT         );
    LIBGLTF_ACCESSORCOMPONENTDATA(uint16_t  , EAccessorComponentType::UNSIGNED_SHORT);
    LIBGLTF_ACCESSORCOMPONENTDATA(int32_t   , EAccessorComponentType::INT           );
    LIBGLTF_ACCESSORCOMPONENTDATA(uint32_t  , EAccessorComponentType::UNSIGNED_INT  );
    LIBGLTF_ACCESSORCOMPONENTDATA(float     , EAccessorComponentType::FLOAT         );

    struct SAccessorComponentType
    {
        size_t value;
        size_t size;
    };

    const SAccessorComponentType GSAccessorComponentTypes[uint32_t(EAccessorComponentType::MAX)] = {
        SAccessorComponentType{ 0   , 0                },
        SAccessorComponentType{ 5120, sizeof(int8_t)   },
        SAccessorComponentType{ 5121, sizeof(uint8_t)  },
        SAccessorComponentType{ 5122, sizeof(int16_t)  },
        SAccessorComponentType{ 5123, sizeof(uint16_t) },
        SAccessorComponentType{ 5124, sizeof(int32_t)  },
        SAccessorComponentType{ 5125, sizeof(uint32_t) },
        SAccessorComponentType{ 5126, sizeof(float)    }
    };

    enum class EAccessorType : uint8_t
    {
        NONE,
        SCALAR,
        VEC2,
        VEC3,
        VEC4,
        MAT2,
        MAT3,
        MAT4,
        MAX,
    };

    struct SAccessorType
    {
        string_t text;
        size_t dimension;
    };

    const SAccessorType GSAccessorTypes[uint8_t(EAccessorType::MAX)] = {
        SAccessorType{ GLTFTEXT("")         , 0  },
        SAccessorType{ GLTFTEXT("SCALAR")   , 1  },
        SAccessorType{ GLTFTEXT("VEC2")     , 2  },
        SAccessorType{ GLTFTEXT("VEC3")     , 3  },
        SAccessorType{ GLTFTEXT("VEC4")     , 4  },
        SAccessorType{ GLTFTEXT("MAT2")     , 4  },
        SAccessorType{ GLTFTEXT("MAT3")     , 9  },
        SAccessorType{ GLTFTEXT("MAT4")     , 16 }
    };

    int32_t AccessorComponentTypeToInt32(EAccessorComponentType _eType);
    EAccessorComponentType Int32ToAccessorComponentType(int32_t _iValue);
    const string_t& AccessorTypeToText(EAccessorType _eType);
    EAccessorType TextToAccessorType(const string_t& _eText, bool _bCaseCensitive = true);
    size_t SizeOfAccessorComponentType(EAccessorComponentType _eType);
    size_t DimensionOfAccessorType(EAccessorType _eType);
    size_t SizeOfAccessor(EAccessorComponentType _eAccessorComponentType, size_t _iCount, EAccessorType _eAccessorType);

    struct SBufferData
    {
        SBufferData();

        const uint8_t* buffer;
        size_t bufferSize;
        size_t bufferStride;
    };

#define LIBGLTF_ACCESSORCOMPONENT_CASE(TType, EType)\
    case EType: {\
        _Vector.Resize(count);\
        for (size_t i = 0; i < count; ++i)\
        {\
            for (size_t j = 0; j < dimensionof_accessor_type; ++j)\
            {\
                _Vector[i][j] = static_cast<typename TVector::TComponent>(*((TType*)bufferData.buffer + i * dimensionof_accessor_type + j));\
            }\
        }\
    } break

    /// help to operate the accessor data
    struct SAccessorData
    {
        SAccessorData();

        EAccessorComponentType componentType;
        size_t count;
        EAccessorType type;
        size_t bufferStride;
        SBufferData bufferData;

        template<typename TVector>
        bool operator>>(TVector& _Vector) const
        {
            const size_t dimensionof_accessor_type = DimensionOfAccessorType(type);
            // not allow to convert to another with the different dimension
            if (dimensionof_accessor_type != TVector::Dimension) return false;
            const size_t sizeof_data = SizeOfAccessor(componentType, 1, type);
            if (bufferStride != 0 && bufferStride != sizeof_data) return false;
            const size_t sizeof_accessor = sizeof_data * count;
            if (sizeof_accessor > bufferData.bufferSize) return false;

            if (count > 0)
            {
                EAccessorComponentType v_component_type = TComponentData<typename TVector::TComponent>();
                if (componentType == v_component_type)
                {
                    _Vector.Resize(count);
                    ::memcpy(_Vector.Data(), bufferData.buffer, sizeof_accessor);
                }
                else
                {
                    switch (componentType)
                    {
                        LIBGLTF_ACCESSORCOMPONENT_CASE(int8_t   , EAccessorComponentType::BYTE          );
                        LIBGLTF_ACCESSORCOMPONENT_CASE(uint8_t  , EAccessorComponentType::UNSIGNED_BYTE );
                        LIBGLTF_ACCESSORCOMPONENT_CASE(int16_t  , EAccessorComponentType::SHORT         );
                        LIBGLTF_ACCESSORCOMPONENT_CASE(uint16_t , EAccessorComponentType::UNSIGNED_SHORT);
                        LIBGLTF_ACCESSORCOMPONENT_CASE(int32_t  , EAccessorComponentType::INT           );
                        LIBGLTF_ACCESSORCOMPONENT_CASE(uint32_t , EAccessorComponentType::UNSIGNED_INT  );
                        LIBGLTF_ACCESSORCOMPONENT_CASE(float    , EAccessorComponentType::FLOAT         );

                    default:
                        return false;
                    }
                }
            }
            return true;
        }
    };

    class IAccessorStream
    {
    public:
        virtual bool operator<<(const SAccessorData& accessor_data) = 0;
    };

    /// a dimension vertex that supports to construct some vertex variables like vec2, vec3, vec4, etc
    template<size_t VDimension, typename TType>
    class TDimensionData
    {
    public:
        static const size_t Dimension = VDimension;

    public:
        const TType& operator[](size_t index) const
        {
            return m_aData[index];
        }

        TType& operator[](size_t index)
        {
            return m_aData[index];
        }

    private:
        TType m_aData[Dimension];
    };

    /// a vector, contains some vertex datas
    template<size_t VDimension, typename TType>
    class TDimensionVector : public std::vector<TDimensionData<VDimension, TType> >
    {
    public:
        typedef std::vector<TDimensionData<VDimension, TType> >     TSuper;
        typedef typename TSuper::value_type                         TValue;
        typedef TType                                               TComponent;

    public:
        static const size_t Dimension = TValue::Dimension;

    public:
        void Resize(size_t new_size)
        {
            TSuper::resize(new_size);
        }

        const TValue* Data() const
        {
            return TSuper::data();
        }

        TValue* Data()
        {
            return TSuper::data();
        }
    };

    /// help to pass the vector
    template<typename TVector>
    class TAccessorStream : public libgltf::IAccessorStream
    {
    public:
        explicit TAccessorStream(TVector& _Vector)
            : m_Vector(_Vector)
        {
            //
        }

    public:
        virtual bool operator<<(const libgltf::SAccessorData& accessor_data) override
        {
            return (accessor_data >> m_Vector);
        }

    private:
        TVector& m_Vector;
    };

    /// gltf loader
    class IglTFLoader
    {
    public:
        static std::shared_ptr<IglTFLoader> Create(const string_t& file);

    public:
        /// get the glTF structure
        virtual std::weak_ptr<struct SGlTF> glTF() = 0;

        /// load the indices data
        virtual bool GetOrLoadMeshPrimitiveIndicesData(size_t mesh_index, size_t primitive_index, std::shared_ptr<IAccessorStream> accessor_stream) = 0;

        /// load the attribute data like position, normal, texcoord, etc
        virtual bool GetOrLoadMeshPrimitiveAttributeData(size_t mesh_index, size_t primitive_index, const string_t& attribute, std::shared_ptr<IAccessorStream> accessor_stream) = 0;

        /// load the image data and type
        virtual bool GetOrLoadImageData(size_t index, std::vector<uint8_t>& data, string_t& data_type) = 0;
    };

}

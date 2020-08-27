/*
 * This software is released under the MIT license.
 *
 * Copyright (c) 2017-2020 Alex Chi, The Code 4 Game Organization
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

#include "libgltfpch.h"
#include "gltf_loader.h"

#include "utility.h"
#include "file_loader.h"

#if defined(LIBGLTF_USE_GOOGLE_DRACO)
#include "extensions/google_draco.h"
#endif

namespace libgltf
{
    class CBufferViewBufferStream : public IBufferStream
    {
    public:
        explicit CBufferViewBufferStream(const std::shared_ptr<SBufferView>& buffer_view, const std::shared_ptr<IBufferViewStream>& buffer_view_stream)
            : m_pBufferView(buffer_view)
            , m_pBufferViewStream(buffer_view_stream)
        {
            //
        }

    public:
        virtual bool operator<<(const SBufferData& buffer_data) override
        {
            if (buffer_data.buffer == nullptr || buffer_data.bufferSize == 0) return false;
            if (!m_pBufferView || !m_pBufferViewStream) return false;
            Verify(buffer_data.bufferSize >= static_cast<size_t>(m_pBufferView->byteOffset));
            SBufferData buffer_data_temp;
            buffer_data_temp = buffer_data;
            buffer_data_temp.buffer += m_pBufferView->byteOffset;
            buffer_data_temp.bufferSize -= m_pBufferView->byteOffset;
            buffer_data_temp.bufferStride = static_cast<size_t>(m_pBufferView->byteStride);
            return (*m_pBufferViewStream << buffer_data_temp);
        }

    private:
        const std::shared_ptr<SBufferView>& m_pBufferView;
        std::shared_ptr<IBufferViewStream> m_pBufferViewStream;
    };

    class CAccessorBufferViewStream : public IBufferViewStream
    {
    public:
        explicit CAccessorBufferViewStream(const std::shared_ptr<SAccessor>& accessor, const std::shared_ptr<IAccessorStream>& accessor_stream)
            : m_pAccessor(accessor)
            , m_pAccessorStream(accessor_stream)
        {
            //
        }

    public:
        virtual bool operator<<(const SBufferData& buffer_data) override
        {
            if (!m_pAccessor || !m_pAccessorStream) return false;
            Verify(buffer_data.bufferSize >= static_cast<size_t>(m_pAccessor->byteOffset));
            SAccessorData accessor_data;
            accessor_data.componentType = Int32ToAccessorComponentType(m_pAccessor->componentType);
            accessor_data.count = static_cast<size_t>(m_pAccessor->count);
            accessor_data.type = TextToAccessorType(m_pAccessor->type);
            accessor_data.bufferStride = buffer_data.bufferStride;
            const size_t sizeof_accessor = SizeOfAccessor(accessor_data.componentType, accessor_data.count, accessor_data.type);
            if (sizeof_accessor > (buffer_data.bufferSize - m_pAccessor->byteOffset)) return false;
            accessor_data.bufferData = buffer_data;
            accessor_data.bufferData.buffer += m_pAccessor->byteOffset;
            accessor_data.bufferData.bufferSize -= m_pAccessor->byteOffset;
            return (*m_pAccessorStream << accessor_data);
        }

    private:
        const std::shared_ptr<SAccessor>& m_pAccessor;
        std::shared_ptr<IAccessorStream> m_pAccessorStream;
    };

#if defined(LIBGLTF_USE_GOOGLE_DRACO)
    class CDracoBufferViewStream : public IBufferViewStream
    {
    public:
        explicit CDracoBufferViewStream(std::shared_ptr<CGoogleDraco>& google_draco, SKHR_draco_mesh_compressionextension* draco_extension, std::shared_ptr<IAccessorStream>& accessor_stream, bool use_attribute_id = false, size_t attribute_id = 0)
            : m_pGoogleDraco(google_draco)
            , m_pDracoExtension(draco_extension)
            , m_pAccessorStream(accessor_stream)
            , m_bUseAttributeID(use_attribute_id)
            , m_AttributeID(attribute_id)
        {
            //
        }

    public:
        virtual bool operator<<(const SBufferData& buffer_data) override
        {
            if (!m_pGoogleDraco || !m_pDracoExtension || !m_pAccessorStream) return false;
            size_t bufferview_index = static_cast<size_t>(int32_t(*m_pDracoExtension->bufferView));
            std::shared_ptr<IGoogleDracoStream> google_draco_stream = IGoogleDracoStream::Create(m_pAccessorStream, m_bUseAttributeID, m_AttributeID);
            return m_pGoogleDraco->GetOrDecode(bufferview_index, buffer_data, google_draco_stream);
        }

    private:
        std::shared_ptr<CGoogleDraco> m_pGoogleDraco;
        SKHR_draco_mesh_compressionextension* m_pDracoExtension;
        std::shared_ptr<IAccessorStream> m_pAccessorStream;
        bool m_bUseAttributeID;
        size_t m_AttributeID;
    };
#endif

    CGlTFLoader::SGLBHeader::SGLBHeader()
        : magic(0)
        , version(0)
        , length(0)
    {
        //
    }

    CGlTFLoader::SGLBChunk::SGLBChunk()
        : length(0)
        , type(0)
    {
        //
    }

    CGlTFLoader::CGlTFLoader(const string_t& _sFilePath)
        : m_glTF(nullptr)
        , m_pFileLoader(std::make_shared<CFileLoader>())
#if defined(LIBGLTF_USE_GOOGLE_DRACO)
        , m_pGoogleDraco(std::make_shared<CGoogleDraco>())
#endif
        , m_CacheBufferDatas()
        , m_CacheImageDatas()
    {
        if (!m_pFileLoader->Load(_sFilePath)) return;
        if (!m_pFileLoader->ReadByte(_sFilePath, 0, &m_GLBHeader, sizeof(m_GLBHeader))) return;

        /// is a glb file?
        if (m_GLBHeader.magic == ms_GLBMagicEntry)
        {
            {
                /// collect all chunks
                SGLBChunk glb_chunk;
                size_t offset = sizeof(m_GLBHeader);
                while (m_pFileLoader->ReadByte(_sFilePath, offset, &glb_chunk, sizeof(glb_chunk)))
                {
                    offset += sizeof(glb_chunk) + glb_chunk.length;
                    m_vGLBChunks.push_back(glb_chunk);
                }
            }
            {
                /// find a json chunk
                SGLBChunk glb_chunk_json;
                size_t offset = sizeof(m_GLBHeader);
                for (const SGLBChunk& glb_chunk : m_vGLBChunks)
                {
                    offset += sizeof(glb_chunk);
                    if (glb_chunk.type == ms_GLBChunkTypeJSON)
                    {
                        glb_chunk_json = glb_chunk;
                        break;
                    }
                    offset += glb_chunk.length;
                }
                if (glb_chunk_json.type == ms_GLBChunkTypeJSON)
                {
                    m_glTF = std::make_shared<SGlTF>();
                    if (!(m_glTF << m_pFileLoader->AsString(_sFilePath, offset, glb_chunk_json.length)))
                    {
                        m_glTF = nullptr;
                    }
                }
            }
        }
        else
        {
            m_glTF = std::make_shared<SGlTF>();
            if (!(m_glTF << m_pFileLoader->AsString(_sFilePath)))
            {
                m_glTF = nullptr;
            }
        }
    }

    bool CGlTFLoader::LoadByUri(const string_t& uri, std::vector<uint8_t>& data, string_t& data_type)
    {
        if (m_GLBHeader.magic != ms_GLBMagicEntry && uri.empty()) return false;

        if (uri.empty())
        {
            /// load the data if the file is .glb
            const std::vector<uint8_t>& default_file_data = (*m_pFileLoader)[GLTFTEXT("")];
            if (default_file_data.empty()) return false;
            SGLBChunk glb_chunk_bin;
            size_t offset = sizeof(m_GLBHeader);
            for (const SGLBChunk& glb_chunk : m_vGLBChunks)
            {
                offset += sizeof(glb_chunk);
                if (glb_chunk.type == ms_GLBChunkTypeBIN)
                {
                    glb_chunk_bin = glb_chunk;
                    break;
                }
                offset += glb_chunk.length;
            }
            if (glb_chunk_bin.type != ms_GLBChunkTypeBIN) return false;
            data.resize(glb_chunk_bin.length);
            ::memcpy(data.data(), default_file_data.data() + offset, glb_chunk_bin.length);
            size_t slash_index = data_type.find_last_of('/');
            if (data_type.size() > (slash_index + 1))
            {
                data_type = data_type.substr(slash_index + 1);
            }
            return true;
        }
        else
        {
            /// support embedded
            string_t data_encode;
            size_t data_index = 0;
            if (UriParse(uri, data_type, data_encode, data_index))
            {
                size_t slash_index = data_type.find_last_of('/');
                if (data_type.size() > (slash_index + 1))
                {
                    data_type = data_type.substr(slash_index + 1);
                }
                return (StringEqual(data_encode, GLTFTEXT("base64"), false) && base64::Decode(uri.substr(data_index), data));
            }
        }
        /// try to load from file
        if (!m_pFileLoader->Load(uri)) return false;
        data = (*m_pFileLoader)[uri];
        size_t dot_index = uri.find_last_of('.');
        if (uri.size() > (dot_index + 1))
        {
            data_type = uri.substr(dot_index + 1);
        }
        return true;
    }

    bool CGlTFLoader::LoadBuffer(const std::shared_ptr<SBuffer>& buffer, std::vector<uint8_t>& data)
    {
        if (!buffer) return false;
        string_t data_type;
        if (!LoadByUri(buffer->uri, data, data_type)) return false;
        return (data.size() == buffer->byteLength);
    }

    bool CGlTFLoader::LoadImage(const std::shared_ptr<SImage>& image, std::vector<uint8_t>& data, string_t& data_type)
    {
        if (!image) return false;
        data_type = image->mimeType;
        return LoadByUri(image->uri, data, data_type);
    }

    bool CGlTFLoader::GetOrLoadBufferData(size_t index, std::shared_ptr<IBufferStream>& buffer_stream)
    {
        SBufferData buffer_data;
        auto found_iterator = m_CacheBufferDatas.find(index);
        if (found_iterator != m_CacheBufferDatas.end())
        {
            buffer_data.buffer = found_iterator->second.data();
            buffer_data.bufferSize = found_iterator->second.size();
        }
        else
        {
            std::vector<uint8_t> loaded_buffer;
            if (m_glTF->buffers.size() > index)
            {
                LoadBuffer(m_glTF->buffers[index], loaded_buffer);
            }
            auto inserted_iterator = m_CacheBufferDatas.insert(std::make_pair(index, loaded_buffer)).first;
            buffer_data.buffer = inserted_iterator->second.data();
            buffer_data.bufferSize = inserted_iterator->second.size();
        }
        return (*buffer_stream << buffer_data);
    }

    bool CGlTFLoader::GetOrLoadBufferViewData(size_t index, std::shared_ptr<IBufferViewStream> buffer_view_stream)
    {
        if (m_glTF->bufferViews.size() <= index) return false;
        const std::shared_ptr<SBufferView>& buffer_view = m_glTF->bufferViews[index];
        Verify(!!buffer_view);
        std::shared_ptr<IBufferStream> buffer_stream = std::make_shared<CBufferViewBufferStream>(buffer_view, buffer_view_stream);
        return GetOrLoadBufferData(static_cast<size_t>(int32_t(*buffer_view->buffer)), buffer_stream);
    }

    bool CGlTFLoader::GetOrLoadAccessorData(size_t index, std::shared_ptr<IAccessorStream> accessor_stream)
    {
        if (!accessor_stream) return false;
        if (m_glTF->accessors.size() <= index) return false;
        const std::shared_ptr<SAccessor>& accessor = m_glTF->accessors[index];
        Verify(!!accessor);
        return GetOrLoadBufferViewData(static_cast<size_t>(int32_t(*accessor->bufferView)), std::make_shared<CAccessorBufferViewStream>(accessor, accessor_stream));
    }

    std::weak_ptr<libgltf::SGlTF> CGlTFLoader::glTF()
    {
        return m_glTF;
    }

    bool CGlTFLoader::GetOrLoadMeshPrimitiveIndicesData(size_t mesh_index, size_t primitive_index, std::shared_ptr<IAccessorStream> accessor_stream)
    {
        if (!m_glTF) return false;
        if (m_glTF->meshes.size() <= mesh_index) return false;
        const std::shared_ptr<SMesh>& mesh = m_glTF->meshes[mesh_index];
        if (!mesh || mesh->primitives.size() <= primitive_index) return false;
        const std::shared_ptr<SMeshPrimitive>& primitive = mesh->primitives[primitive_index];
        if (!primitive || !primitive->indices) return false;

        if (primitive->extensions)
        {
            const std::shared_ptr<SExtension>& extensions = primitive->extensions;
            std::shared_ptr<SObject> extension;
            if (StringMapFind<std::shared_ptr<SObject>>(extensions->properties, GLTFTEXT("KHR_draco_mesh_compression"), extension))
            {
#if defined(LIBGLTF_USE_GOOGLE_DRACO)
                SKHR_draco_mesh_compressionextension* extension_draco = reinterpret_cast<SKHR_draco_mesh_compressionextension*>(extension.get());
                size_t bufferview_index = static_cast<size_t>(int32_t(*extension_draco->bufferView));
                std::shared_ptr<IBufferViewStream> buffer_view_stream = std::make_shared<CDracoBufferViewStream>(m_pGoogleDraco, extension_draco, accessor_stream);
                return GetOrLoadBufferViewData(bufferview_index, buffer_view_stream);
#else
                return false;
#endif
            }
        }

        return GetOrLoadAccessorData(static_cast<size_t>(int32_t(*primitive->indices)), accessor_stream);
    }

    bool CGlTFLoader::GetOrLoadMeshPrimitiveAttributeData(size_t mesh_index, size_t primitive_index, const string_t& attribute, std::shared_ptr<IAccessorStream> accessor_stream)
    {
        if (!m_glTF) return false;
        if (m_glTF->meshes.size() <= mesh_index) return false;
        const std::shared_ptr<SMesh>& mesh = m_glTF->meshes[mesh_index];
        if (!mesh || mesh->primitives.size() <= primitive_index) return false;
        const std::shared_ptr<SMeshPrimitive>& primitive = mesh->primitives[primitive_index];
        if (!primitive) return false;

        if (primitive->extensions)
        {
            const std::shared_ptr<SExtension>& extensions = primitive->extensions;
            std::shared_ptr<SObject> extension;
            if (StringMapFind<std::shared_ptr<SObject>>(extensions->properties, GLTFTEXT("KHR_draco_mesh_compression"), extension))
            {
#if defined(LIBGLTF_USE_GOOGLE_DRACO)
                SKHR_draco_mesh_compressionextension* extension_draco = reinterpret_cast<SKHR_draco_mesh_compressionextension*>(extension.get());
                size_t bufferview_index = static_cast<size_t>(int32_t(*extension_draco->bufferView));
                std::shared_ptr<SGlTFId> attribute_id;
                if (!StringMapFind<std::shared_ptr<SGlTFId>>(extension_draco->attributes, attribute, attribute_id, false)) return false;
                std::shared_ptr<IBufferViewStream> buffer_view_stream = std::make_shared<CDracoBufferViewStream>(m_pGoogleDraco, extension_draco, accessor_stream, true, static_cast<size_t>(int32_t(*attribute_id)));
                return GetOrLoadBufferViewData(bufferview_index, buffer_view_stream);
#else
                return false;
#endif
            }
        }

        const std::map<string_t, std::shared_ptr<SGlTFId>>& primitive_attributes = primitive->attributes;
        std::shared_ptr<SGlTFId> attribute_access;
        if (!StringMapFind<std::shared_ptr<SGlTFId>>(primitive_attributes, attribute, attribute_access, false)) return false;
        return GetOrLoadAccessorData(static_cast<size_t>(int32_t(*attribute_access)), accessor_stream);
    }

    bool CGlTFLoader::GetOrLoadImageData(size_t index, std::vector<uint8_t>& data, string_t& data_type)
    {
        if (!m_glTF) return false;
        if (m_glTF->images.size() <= index) return false;
        std::map<size_t, std::pair<std::vector<uint8_t>, string_t>>::const_iterator found = m_CacheImageDatas.find(index);
        if (found != m_CacheImageDatas.end())
        {
            data = found->second.first;
            data_type = found->second.second;
            return true;
        }
        const std::shared_ptr<SImage>& image = m_glTF->images[index];
        if (!LoadImage(image, data, data_type)) return false;
        m_CacheImageDatas.insert(std::make_pair(index, std::make_pair(data, data_type)));
        return true;
    }

    const uint32_t CGlTFLoader::ms_GLBMagicEntry    = 0x46546C67;   // 'glTF'
    const uint32_t CGlTFLoader::ms_GLBChunkTypeJSON = 0x4E4F534A;   // 'JSON'
    const uint32_t CGlTFLoader::ms_GLBChunkTypeBIN  = 0x004E4942;   // 'BIN\0'
}

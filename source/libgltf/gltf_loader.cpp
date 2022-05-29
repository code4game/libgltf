/*
 * This software is released under the MIT license.
 *
 * Copyright (c) 2017-2022 Code 4 Game, Org. All Rights Reserved.
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

#include "gltf_loader.h"

#include "common.h"
#include "utility.h"

#if defined(LIBGLTF_WITH_GOOGLE_DRACO)
#    include "extensions/google_draco.h"
#endif

namespace libgltf
{
    class CBufferViewBufferStream : public IBufferStream
    {
    public:
        explicit CBufferViewBufferStream(const std::shared_ptr<SBufferView>&       _buffer_view,
                                         const std::shared_ptr<IBufferViewStream>& _buffer_view_stream)
            : m_pBufferView(_buffer_view)
            , m_pBufferViewStream(_buffer_view_stream)
        {
            //
        }

    public:
        virtual bool operator<<(const SBufferData& _buffer_data) override
        {
            if (_buffer_data.buffer == nullptr || _buffer_data.bufferSize == 0)
                return false;
            if (!m_pBufferView || !m_pBufferViewStream)
                return false;
            Verify(_buffer_data.bufferSize >= static_cast<std::size_t>(m_pBufferView->byteOffset));
            SBufferData buffer_data_temp;
            buffer_data_temp = _buffer_data;
            buffer_data_temp.buffer += m_pBufferView->byteOffset;
            buffer_data_temp.bufferSize -= m_pBufferView->byteOffset;
            if (m_pBufferView->byteLength < static_cast<std::size_t>(buffer_data_temp.bufferSize))
            {
                buffer_data_temp.bufferSize = m_pBufferView->byteLength;
            }
            buffer_data_temp.bufferStride = static_cast<std::size_t>(m_pBufferView->byteStride);
            return (*m_pBufferViewStream << buffer_data_temp);
        }

    private:
        const std::shared_ptr<SBufferView>& m_pBufferView;
        std::shared_ptr<IBufferViewStream>  m_pBufferViewStream;
    };

    class CBufferViewStream : public IBufferViewStream
    {
    public:
        explicit CBufferViewStream(std::vector<uint8_t>& _data)
            : m_Data(_data)
        {
            //
        }

    public:
        virtual bool operator<<(const SBufferData& _buffer_data)
        {
            m_Data.resize(_buffer_data.bufferSize);
            ::memcpy(&m_Data[0], _buffer_data.buffer, m_Data.size());
            return true;
        }

    private:
        std::vector<uint8_t>& m_Data;
    };

    class CAccessorBufferViewStream : public IBufferViewStream
    {
    public:
        explicit CAccessorBufferViewStream(const std::shared_ptr<SAccessor>& _accessor, const std::shared_ptr<IAccessorStream>& _accessor_stream)
            : m_pAccessor(_accessor)
            , m_pAccessorStream(_accessor_stream)
        {
            //
        }

    public:
        virtual bool operator<<(const SBufferData& _buffer_data) override
        {
            if (!m_pAccessor || !m_pAccessorStream)
                return false;
            Verify(_buffer_data.bufferSize >= static_cast<std::size_t>(m_pAccessor->byteOffset));
            SAccessorData accessor_data;
            accessor_data.componentType       = Int32ToAccessorComponentType(m_pAccessor->componentType);
            accessor_data.count               = static_cast<std::size_t>(m_pAccessor->count);
            accessor_data.type                = TextToAccessorType(m_pAccessor->type);
            accessor_data.bufferStride        = _buffer_data.bufferStride;
            const std::size_t sizeof_accessor = SizeOfAccessor(accessor_data.componentType, accessor_data.count, accessor_data.type);
            if (sizeof_accessor > (_buffer_data.bufferSize - m_pAccessor->byteOffset))
                return false;
            accessor_data.bufferData = _buffer_data;
            accessor_data.bufferData.buffer += m_pAccessor->byteOffset;
            accessor_data.bufferData.bufferSize -= m_pAccessor->byteOffset;
            return (*m_pAccessorStream << accessor_data);
        }

    private:
        const std::shared_ptr<SAccessor>& m_pAccessor;
        std::shared_ptr<IAccessorStream>  m_pAccessorStream;
    };

#if defined(LIBGLTF_WITH_GOOGLE_DRACO)
    class CDracoBufferViewStream : public IBufferViewStream
    {
    public:
        explicit CDracoBufferViewStream(std::unique_ptr<CGoogleDraco>&        _google_draco,
                                        SKHR_draco_mesh_compressionextension* _draco_extension,
                                        std::shared_ptr<IAccessorStream>&     _accessor_stream,
                                        bool                                  _use_attribute_id = false,
                                        std::size_t                           _attribute_id     = 0)
            : m_pGoogleDraco(_google_draco)
            , m_pDracoExtension(_draco_extension)
            , m_pAccessorStream(_accessor_stream)
            , m_bUseAttributeID(_use_attribute_id)
            , m_AttributeID(_attribute_id)
        {
            //
        }

    public:
        virtual bool operator<<(const SBufferData& _buffer_data) override
        {
            if (!m_pGoogleDraco || !m_pDracoExtension || !m_pAccessorStream)
                return false;

            std::size_t                         bufferview_index    = static_cast<std::size_t>(int32_t(*m_pDracoExtension->bufferView));
            std::shared_ptr<IGoogleDracoStream> google_draco_stream = IGoogleDracoStream::Create(m_pAccessorStream, m_bUseAttributeID, m_AttributeID);
            return m_pGoogleDraco->GetOrDecode(bufferview_index, _buffer_data, google_draco_stream);
        }

    private:
        std::unique_ptr<CGoogleDraco>&        m_pGoogleDraco;
        SKHR_draco_mesh_compressionextension* m_pDracoExtension;
        std::shared_ptr<IAccessorStream>      m_pAccessorStream;
        bool                                  m_bUseAttributeID;
        std::size_t                           m_AttributeID;
    };
#endif

    CglTFLoader::SGLBHeader::SGLBHeader()
        : magic(0)
        , version(0)
        , length(0)
    {
        //
    }

    CglTFLoader::SGLBChunk::SGLBChunk()
        : length(0)
        , type(0)
    {
        //
    }

    CglTFLoader::CglTFLoader(std::function<std::shared_ptr<std::istream>(const std::string&)> _reader)
        : m_glTF(nullptr)
        , m_Reader(_reader)
        , m_CacheDatas()
#if defined(LIBGLTF_WITH_GOOGLE_DRACO)
        , m_pGoogleDraco(std::make_unique<CGoogleDraco>())
#endif
    {
        std::shared_ptr<std::istream> reader_ptr = m_Reader("");
        if (!reader_ptr)
            return;

        m_CacheDatas[""] = std::make_pair<std::vector<uint8_t>, std::string>(std::vector<uint8_t>(), "glb");

        std::vector<uint8_t> data;
        {
            reader_ptr->seekg(0, std::ios::end);
            data.resize(reader_ptr->tellg());
            reader_ptr->seekg(0, std::ios::beg);
            if (!data.empty())
                reader_ptr->read((std::istream::char_type*)&data[0], data.size());
        }

        /// check the format. gltf or glb?
        const SGLBHeader* glb_header_ptr = nullptr;
        if (data.size() > sizeof(SGLBHeader))
        {
            glb_header_ptr = (SGLBHeader*)&data[0];
            if (glb_header_ptr->magic != ms_GLBMagicEntry)
                glb_header_ptr = nullptr;
        }

        std::string txt_json;
        if (glb_header_ptr)
        {
            /// collect all chunks
            std::vector<const SGLBChunk*> glb_chunk_ptrs;

            std::size_t offset = sizeof(SGLBHeader);
            while (offset < data.size())
            {
                const SGLBChunk* glb_chunk_ptr = (SGLBChunk*)&data[offset];
                glb_chunk_ptrs.push_back(glb_chunk_ptr);

                offset += glb_chunk_ptr->length + sizeof(SGLBChunk);
            }

            /// find the json chunk
            const SGLBChunk* glb_chunk_json_ptr = nullptr;
            for (const SGLBChunk* glb_chunk_ptr : glb_chunk_ptrs)
            {
                if (glb_chunk_ptr->type != ms_GLBChunkTypeJSON)
                    continue;

                txt_json.resize(glb_chunk_ptr->length);
                if (!txt_json.empty())
                    ::memcpy(&txt_json[0], (const uint8_t*)glb_chunk_ptr + sizeof(SGLBChunk), glb_chunk_ptr->length);
                break;
            }

            /// find the binary chunk
            for (const SGLBChunk* glb_chunk_ptr : glb_chunk_ptrs)
            {
                if (glb_chunk_ptr->type != ms_GLBChunkTypeBIN)
                    continue;

                std::pair<std::vector<uint8_t>, std::string>& data = m_CacheDatas[""];
                data.first.resize(glb_chunk_ptr->length);
                if (!data.first.empty())
                    ::memcpy(&data.first[0], (const uint8_t*)glb_chunk_ptr + sizeof(SGLBChunk), glb_chunk_ptr->length);
                break;
            }
        }
        else
        {
            txt_json.resize(data.size());
            if (!txt_json.empty())
                ::memcpy(&txt_json[0], &data[0], data.size());
        }

        if (txt_json.empty())
            return;

        m_glTF = std::make_unique<SGlTF>();
        if (!(*m_glTF << txt_json))
            m_glTF = nullptr;
        else
        {
            // TODO: check version?
        }
    }

    bool CglTFLoader::LoadByUri(const std::string& _uri, const uint8_t*& _data_ptr, std::size_t& _data_size, std::string& _data_type)
    {
        std::map<std::string, std::pair<std::vector<uint8_t>, std::string>>::const_iterator it_found = m_CacheDatas.find(_uri);
        if (it_found != m_CacheDatas.end())
        {
            const std::pair<std::vector<uint8_t>, std::string>& cache_data = it_found->second;
            _data_ptr                                                      = cache_data.first.data();
            _data_size                                                     = cache_data.first.size();
            _data_type                                                     = cache_data.second;
            return true;
        }

        std::pair<std::vector<uint8_t>, std::string>& cache_data = m_CacheDatas[_uri];

        bool is_loaded = false;
        {
            /// support embedded
            std::string data_encode;
            std::size_t data_index = 0;
            if (UriParse(_uri, _data_type, data_encode, data_index))
            {
                if (!StringEqual(data_encode, "base64", false) && base64::Decode(_uri.substr(data_index), cache_data.first))
                    return false;

                cache_data.second = "base64";

                is_loaded = true;
            }
        }

        if (!is_loaded)
        {
            /// try to load from file
            if (std::shared_ptr<std::istream> reader_ptr = m_Reader(_uri))
            {
                reader_ptr->seekg(0, std::ios::end);
                cache_data.first.resize(reader_ptr->tellg());
                reader_ptr->seekg(0, std::ios::beg);
                if (!cache_data.first.empty())
                    reader_ptr->read((std::istream::char_type*)&cache_data.first[0], cache_data.first.size());

                std::string file_type;
                std::size_t dot_index = _uri.find_last_of('.');
                if (_uri.size() > (dot_index + 1))
                    cache_data.second = "file/" + _uri.substr(dot_index + 1);

                is_loaded = true;
            }
        }

        _data_ptr  = cache_data.first.data();
        _data_size = cache_data.first.size();
        _data_type = cache_data.second;
        return is_loaded;
    }

    bool CglTFLoader::LoadBuffer(const std::shared_ptr<SBuffer>& _buffer, const uint8_t*& _data_ptr, std::size_t& _data_size)
    {
        if (!_buffer)
            return false;
        std::string data_type;
        if (!LoadByUri(_buffer->uri, _data_ptr, _data_size, data_type))
            return false;
        return (_data_size == _buffer->byteLength);
    }

    bool CglTFLoader::LoadImage(const std::shared_ptr<SImage>& _image, std::vector<uint8_t>& _data, std::string& _type)
    {
        if (!_image)
            return false;

        _type = _image->mimeType;
        if (!_image->uri.empty())
        {
            const uint8_t* data_ptr  = nullptr;
            std::size_t    data_size = 0;
            if (!LoadByUri(_image->uri, data_ptr, data_size, _type))
                return false;

            _data.resize(data_size);
            ::memcpy(&_data[0], data_ptr, data_size);
            return true;
        }
        if (!_image->bufferView)
            return false;

        std::shared_ptr<CBufferViewStream> image_stream = std::make_shared<CBufferViewStream>(_data);
        return LoadBufferViewData(static_cast<std::size_t>(int32_t(*_image->bufferView)), image_stream);
    }

    bool CglTFLoader::LoadBufferData(std::size_t _index, std::shared_ptr<IBufferStream>& _buffer_stream)
    {
        SBufferData buffer_data;
        LoadBuffer(m_glTF->buffers[_index], buffer_data.buffer, buffer_data.bufferSize);
        return (*_buffer_stream << buffer_data);
    }

    bool CglTFLoader::LoadBufferViewData(std::size_t _index, std::shared_ptr<IBufferViewStream> _buffer_view_stream)
    {
        if (m_glTF->bufferViews.size() <= _index)
            return false;
        const std::shared_ptr<SBufferView>& buffer_view = m_glTF->bufferViews[_index];
        Verify(!!buffer_view);
        std::shared_ptr<IBufferStream> buffer_stream = std::make_shared<CBufferViewBufferStream>(buffer_view, _buffer_view_stream);
        return LoadBufferData(static_cast<std::size_t>(int32_t(*buffer_view->buffer)), buffer_stream);
    }

    bool CglTFLoader::LoadAccessorData(std::size_t _index, std::shared_ptr<IAccessorStream> _accessor_stream)
    {
        if (!_accessor_stream)
            return false;
        if (m_glTF->accessors.size() <= _index)
            return false;
        const std::shared_ptr<SAccessor>& accessor = m_glTF->accessors[_index];
        Verify(!!accessor);
        return LoadBufferViewData(static_cast<std::size_t>(int32_t(*accessor->bufferView)),
                                       std::make_shared<CAccessorBufferViewStream>(accessor, _accessor_stream));
    }

    const std::unique_ptr<SGlTF>& CglTFLoader::glTF() const
    {
        return m_glTF;
    }

    bool CglTFLoader::LoadMeshPrimitiveIndicesData(std::size_t                      _mesh_index, //
                                                   std::size_t                      _primitive_index,
                                                   std::shared_ptr<IAccessorStream> _accessor_stream)
    {
        if (!m_glTF)
            return false;
        if (m_glTF->meshes.size() <= _mesh_index)
            return false;
        const std::shared_ptr<SMesh>& mesh = m_glTF->meshes[_mesh_index];
        if (!mesh || mesh->primitives.size() <= _primitive_index)
            return false;
        const std::shared_ptr<SMeshPrimitive>& primitive = mesh->primitives[_primitive_index];
        if (!primitive || !primitive->indices)
            return false;

        if (primitive->extensions)
        {
            const std::shared_ptr<SExtension>& extensions = primitive->extensions;
            std::shared_ptr<SObject>           extension;
            if (StringMapFind<std::shared_ptr<SObject>>(extensions->properties, "KHR_draco_mesh_compression", extension))
            {
#if defined(LIBGLTF_WITH_GOOGLE_DRACO)
                SKHR_draco_mesh_compressionextension* extension_draco  = reinterpret_cast<SKHR_draco_mesh_compressionextension*>(extension.get());
                std::size_t                           bufferview_index = static_cast<std::size_t>(int32_t(*extension_draco->bufferView));
                std::shared_ptr<IBufferViewStream>    buffer_view_stream =
                    std::make_shared<CDracoBufferViewStream>(m_pGoogleDraco, extension_draco, _accessor_stream);
                return LoadBufferViewData(bufferview_index, buffer_view_stream);
#else
                return false;
#endif
            }
        }

        return LoadAccessorData(static_cast<std::size_t>(int32_t(*primitive->indices)), _accessor_stream);
    }

    bool CglTFLoader::LoadMeshPrimitiveAttributeData(std::size_t                      _mesh_index,
                                                     std::size_t                      _primitive_index,
                                                     const std::string&               _attribute,
                                                     std::shared_ptr<IAccessorStream> _accessor_stream)
    {
        if (!m_glTF)
            return false;
        if (m_glTF->meshes.size() <= _mesh_index)
            return false;
        const std::shared_ptr<SMesh>& mesh = m_glTF->meshes[_mesh_index];
        if (!mesh || mesh->primitives.size() <= _primitive_index)
            return false;
        const std::shared_ptr<SMeshPrimitive>& primitive = mesh->primitives[_primitive_index];
        if (!primitive)
            return false;

        if (primitive->extensions)
        {
            const std::shared_ptr<SExtension>& extensions = primitive->extensions;
            std::shared_ptr<SObject>           extension;
            if (StringMapFind<std::shared_ptr<SObject>>(extensions->properties, "KHR_draco_mesh_compression", extension))
            {
#if defined(LIBGLTF_WITH_GOOGLE_DRACO)
                SKHR_draco_mesh_compressionextension* extension_draco  = reinterpret_cast<SKHR_draco_mesh_compressionextension*>(extension.get());
                std::size_t                           bufferview_index = static_cast<std::size_t>(int32_t(*extension_draco->bufferView));
                std::shared_ptr<SGlTFId>              attribute_id;
                if (!StringMapFind<std::shared_ptr<SGlTFId>>(extension_draco->attributes, _attribute, attribute_id, false))
                    return false;
                std::shared_ptr<IBufferViewStream> buffer_view_stream = std::make_shared<CDracoBufferViewStream>(
                    m_pGoogleDraco, extension_draco, _accessor_stream, true, static_cast<std::size_t>(int32_t(*attribute_id)));
                return LoadBufferViewData(bufferview_index, buffer_view_stream);
#else
                return false;
#endif
            }
        }

        const std::map<std::string, std::shared_ptr<SGlTFId>>& primitive_attributes = primitive->attributes;
        std::shared_ptr<SGlTFId>                               attribute_access;
        if (!StringMapFind<std::shared_ptr<SGlTFId>>(primitive_attributes, _attribute, attribute_access, false))
            return false;
        return LoadAccessorData(static_cast<std::size_t>(int32_t(*attribute_access)), _accessor_stream);
    }

    bool CglTFLoader::LoadImageData(std::size_t _index, std::vector<uint8_t>& _data, std::string& _type)
    {
        if (!m_glTF)
            return false;
        if (m_glTF->images.size() <= _index)
            return false;

        const std::shared_ptr<SImage>& image = m_glTF->images[_index];
        return LoadImage(image, _data, _type);
    }

    const uint32_t CglTFLoader::ms_GLBMagicEntry    = 0x46546C67; // 'glTF'
    const uint32_t CglTFLoader::ms_GLBChunkTypeJSON = 0x4E4F534A; // 'JSON'
    const uint32_t CglTFLoader::ms_GLBChunkTypeBIN  = 0x004E4942; // 'BIN\0'
} // namespace libgltf

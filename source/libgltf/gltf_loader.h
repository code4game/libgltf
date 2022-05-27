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

#pragma once

#include "libgltf/libgltf.h"

#if defined(LIBGLTF_WITH_GOOGLE_DRACO)
#    include "extensions/google_draco.h"
#endif

namespace libgltf
{
    class IBufferStream
    {
    public:
        virtual bool operator<<(const SBufferData& _buffer_data) = 0;
    };

    class IBufferViewStream
    {
    public:
        virtual bool operator<<(const SBufferData& _buffer_data) = 0;
    };

    class CglTFLoader : public IglTFLoader
    {
        struct SGLBHeader
        {
            SGLBHeader();

            uint32_t magic;
            uint32_t version;
            uint32_t length;
        };

        struct SGLBChunk
        {
            SGLBChunk();

            uint32_t length;
            uint32_t type;
        };

    public:
        explicit CglTFLoader(std::function<std::shared_ptr<std::istream>(const std::string&)> _reader);

    protected:
        bool LoadByUri(const std::string& _uri, const uint8_t*& _data_ptr, std::size_t& _data_size, std::string& _data_type);
        bool LoadBuffer(const std::shared_ptr<SBuffer>& _buffer, const uint8_t*& _data_ptr, std::size_t& _data_size);
        bool LoadImage(const std::shared_ptr<SImage>& _image, std::vector<uint8_t>& _data, std::string& _type);
        bool LoadBufferData(std::size_t _index, std::shared_ptr<IBufferStream>& _buffer_stream);
        bool LoadBufferViewData(std::size_t _index, std::shared_ptr<IBufferViewStream> _buffer_view_stream);
        bool LoadAccessorData(std::size_t _index, std::shared_ptr<IAccessorStream> _accessor_stream);

    public:
        virtual const std::unique_ptr<SGlTF>& glTF() const override;
        virtual bool                          LoadMeshPrimitiveIndicesData(std::size_t                      _mesh_index, //
                                                                           std::size_t                      _primitive_index,
                                                                           std::shared_ptr<IAccessorStream> _accessor_stream) override;
        virtual bool                          LoadMeshPrimitiveAttributeData(std::size_t                      _mesh_index,
                                                                             std::size_t                      _primitive_index,
                                                                             const std::string&               _attribute,
                                                                             std::shared_ptr<IAccessorStream> _accessor_stream) override;
        virtual bool                          LoadImageData(std::size_t _index, std::vector<uint8_t>& _data, std::string& _type) override;

    protected:
        std::unique_ptr<SGlTF> m_glTF;

    private:
        std::function<std::shared_ptr<std::istream>(const std::string&)>    m_Reader;
        std::map<std::string, std::pair<std::vector<uint8_t>, std::string>> m_CacheDatas;
#if defined(LIBGLTF_WITH_GOOGLE_DRACO)
        std::unique_ptr<CGoogleDraco> m_pGoogleDraco;
#endif

    public:
        static const uint32_t ms_GLBMagicEntry;
        static const uint32_t ms_GLBChunkTypeJSON;
        static const uint32_t ms_GLBChunkTypeBIN;
    };
} // namespace libgltf

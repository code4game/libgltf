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

#include "common.h"

namespace libgltf
{
    class IBufferStream
    {
    public:
        virtual bool operator<<(const SBufferData& buffer_data) = 0;
    };

    class IBufferViewStream
    {
    public:
        virtual bool operator<<(const SBufferData& buffer_data) = 0;
    };

    class CGlTFLoader : public IglTFLoader
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
        explicit CGlTFLoader(const string_t& file);

    protected:
        bool LoadByUri(const string_t& uri, std::vector<uint8_t>& data, string_t& data_type);
        bool LoadBuffer(const std::shared_ptr<SBuffer>& buffer, std::vector<uint8_t>& data);
        bool LoadImage(const std::shared_ptr<SImage>& image, std::vector<uint8_t>& data, string_t& data_type);
        bool GetOrLoadBufferData(size_t index, std::shared_ptr<IBufferStream>& buffer_stream);
        bool GetOrLoadBufferViewData(size_t index, std::shared_ptr<IBufferViewStream> buffer_view_stream);
        bool GetOrLoadAccessorData(size_t index, std::shared_ptr<IAccessorStream> accessor_stream);

    public:
        virtual std::weak_ptr<SGlTF> glTF() override;
        virtual bool GetOrLoadMeshPrimitiveIndicesData(size_t mesh_index, size_t primitive_index, std::shared_ptr<IAccessorStream> accessor_stream) override;
        virtual bool GetOrLoadMeshPrimitiveAttributeData(size_t mesh_index, size_t primitive_index, const string_t& attribute, std::shared_ptr<IAccessorStream> accessor_stream) override;
        virtual bool GetOrLoadImageData(size_t index, std::vector<uint8_t>& data, string_t& data_type) override;

    protected:
        std::shared_ptr<SGlTF> m_glTF;

    private:
        std::shared_ptr<class CFileLoader> m_pFileLoader;
        SGLBHeader m_GLBHeader;
        std::vector<SGLBChunk> m_vGLBChunks;
#if defined(LIBGLTF_USE_GOOGLE_DRACO)
        std::shared_ptr<class CGoogleDraco> m_pGoogleDraco;
#endif
        std::map<size_t, std::vector<uint8_t>> m_CacheBufferDatas;
        std::map<size_t, std::pair<std::vector<uint8_t>, string_t>> m_CacheImageDatas;

    public:
        static const uint32_t ms_GLBMagicEntry;
        static const uint32_t ms_GLBChunkTypeJSON;
        static const uint32_t ms_GLBChunkTypeBIN;
    };
}

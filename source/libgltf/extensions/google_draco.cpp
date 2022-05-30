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

#include "google_draco.h"

#include <draco/compression/decode.h>

namespace libgltf
{
    class CGoogleDracoStream : public IGoogleDracoStream
    {
    public:
        explicit CGoogleDracoStream(std::shared_ptr<IAccessorStream>& _pAccessorStream, bool _bUseAttributeID, size_t _AttributeID)
            : m_pAccessorStream(_pAccessorStream)
            , m_bUseAttributeID(_bUseAttributeID)
            , m_AttributeID(_AttributeID)
        {
            //
        }

    public:
        virtual bool UseAttributeID() const override
        {
            return m_bUseAttributeID;
        }

        virtual size_t GetAttributeID() const override
        {
            return m_AttributeID;
        }

    public:
        virtual bool operator<<(const SAccessorData& _AccessorData) override
        {
            if (!m_pAccessorStream) return false;
            return (*m_pAccessorStream << _AccessorData);
        }

    private:
        std::shared_ptr<IAccessorStream> m_pAccessorStream;
        bool m_bUseAttributeID;
        size_t m_AttributeID;
    };

    std::shared_ptr<IGoogleDracoStream> IGoogleDracoStream::Create(std::shared_ptr<IAccessorStream> _pAccessorStream, bool _bUseAttributeID, size_t _AttributeID)
    {
        return std::make_shared<CGoogleDracoStream>(_pAccessorStream, _bUseAttributeID, _AttributeID);
    }

    CGoogleDraco::SDracoPointAttribute::SDracoPointAttribute()
        : accessorData()
        , buffer()
    {
        //
    }

    CGoogleDraco::SDracoMesh::SDracoMesh(std::shared_ptr<draco::Mesh> _pDracoMesh)
        : dracoMesh(_pDracoMesh)
        , indices()
        , pointAttributes()
    {
        //
    }

    CGoogleDraco::CGoogleDraco()
        : m_DracoMeshes()
    {
        //
    }

    bool CGoogleDraco::GetOrDecode(size_t _Index, const SBufferData& _BufferDataEncoded, std::shared_ptr<IGoogleDracoStream> _pGoogleDracoStream)
    {
        if (_BufferDataEncoded.buffer == nullptr || _BufferDataEncoded.bufferSize == 0 || !_pGoogleDracoStream) return false;

        std::map<size_t, std::shared_ptr<SDracoMesh>>::iterator found_iterator = m_DracoMeshes.find(_Index);
        if (found_iterator == m_DracoMeshes.end())
        {
            draco::DecoderBuffer DracoDecoderBuffer;
            DracoDecoderBuffer.Init((const char*)_BufferDataEncoded.buffer, _BufferDataEncoded.bufferSize);
            auto StatusOrGeometryType = draco::Decoder::GetEncodedGeometryType(&DracoDecoderBuffer);
            if (StatusOrGeometryType.ok() && StatusOrGeometryType.value() == draco::TRIANGULAR_MESH)
            {
                draco::Decoder DracoDecoder;
                auto DracoStatusOrMesh = DracoDecoder.DecodeMeshFromBuffer(&DracoDecoderBuffer);
                if (DracoStatusOrMesh.ok())
                {
                    std::unique_ptr<draco::Mesh> DracoMesh = std::move(DracoStatusOrMesh).value();
                    found_iterator = m_DracoMeshes.insert(std::make_pair(_Index, std::make_shared<SDracoMesh>(std::shared_ptr<draco::Mesh>(std::move(DracoMesh))))).first;
                }
            }
        }
        if (found_iterator == m_DracoMeshes.end())
        {
            found_iterator = m_DracoMeshes.insert(std::make_pair(_Index, std::shared_ptr<SDracoMesh>())).first;
        }
        std::shared_ptr<SDracoMesh> found_draco_mesh = found_iterator->second;
        if (!found_draco_mesh) return false;
        const std::shared_ptr<draco::Mesh>& draco_mesh = found_draco_mesh->dracoMesh;
        if (!draco_mesh) return false;

        if (_pGoogleDracoStream->UseAttributeID())
        {
            std::map<size_t, SDracoPointAttribute>::iterator found_point_iterator = found_draco_mesh->pointAttributes.find(_pGoogleDracoStream->GetAttributeID());
            if (found_point_iterator == found_draco_mesh->pointAttributes.end())
            {
                if (const draco::PointAttribute* point_attribute = draco_mesh->GetAttributeByUniqueId(static_cast<uint32_t>(_pGoogleDracoStream->GetAttributeID())))
                {
                    const int8_t components_num = point_attribute->num_components();
                    if (components_num > 0)
                    {
                        const size_t map_size = point_attribute->indices_map_size();
                        SDracoPointAttribute draco_point_attribute;
                        SAccessorData& accessor_data = draco_point_attribute.accessorData;
                        accessor_data.componentType = EAccessorComponentType::FLOAT;
                        accessor_data.count = map_size;
                        switch (components_num)
                        {
                        case 1:
                            accessor_data.type = EAccessorType::SCALAR;
                            break;

                        case 2:
                            accessor_data.type = EAccessorType::VEC2;
                            break;

                        case 3:
                            accessor_data.type = EAccessorType::VEC3;
                            break;

                        case 4:
                            accessor_data.type = EAccessorType::VEC4;
                            break;

                        default:
                            return false;
                        }
                        const size_t sizeof_data = SizeOfAccessor(accessor_data.componentType, 1, accessor_data.type);
                        draco_point_attribute.buffer.resize(sizeof_data * accessor_data.count);
                        //TODO: size_t to uint32_t
                        for (draco::PointIndex i(0); i < static_cast<uint32_t>(accessor_data.count); ++i)
                        {
                            point_attribute->GetMappedValue(i, draco_point_attribute.buffer.data() + sizeof_data * i.value());
                        }
                        found_point_iterator = found_draco_mesh->pointAttributes.insert(std::make_pair(_pGoogleDracoStream->GetAttributeID(), draco_point_attribute)).first;
                    }
                }
            }
            if (found_point_iterator == found_draco_mesh->pointAttributes.end())
            {
                SDracoPointAttribute draco_point_attribute;
                found_point_iterator = found_draco_mesh->pointAttributes.insert(std::make_pair(_pGoogleDracoStream->GetAttributeID(), draco_point_attribute)).first;
            }
            {
                const SDracoPointAttribute& draco_point_attribute = found_point_iterator->second;
                SAccessorData accessor_data = draco_point_attribute.accessorData;
                accessor_data.bufferData.buffer = draco_point_attribute.buffer.data();
                accessor_data.bufferData.bufferSize = draco_point_attribute.buffer.size();
                return (*_pGoogleDracoStream << accessor_data);
            }
        }
        else
        {
            const draco::FaceIndex::ValueType num_faces = draco_mesh->num_faces();
            if (found_draco_mesh->indices.size() != num_faces)
            {
                found_draco_mesh->indices.resize(num_faces * 3);
                for (draco::FaceIndex i(0); i < num_faces; ++i)
                {
                    const draco::Mesh::Face& draco_face = draco_mesh->face(i);
                    for (uint8_t j = 0; j < 3; ++j)
                    {
                        found_draco_mesh->indices[i.value() * 3 + j] = draco_face[j].value();
                    }
                }
            }
            SAccessorData accessor_data;
            accessor_data.componentType = EAccessorComponentType::UNSIGNED_INT;
            accessor_data.count = found_draco_mesh->indices.size();
            accessor_data.type = EAccessorType::SCALAR;
            accessor_data.bufferData.buffer = (uint8_t*)found_draco_mesh->indices.data();
            accessor_data.bufferData.bufferSize = found_draco_mesh->indices.size() * sizeof(uint32_t);
            return (*_pGoogleDracoStream << accessor_data);
        }
        return false;
    }
}

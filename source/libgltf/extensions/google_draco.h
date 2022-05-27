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

#include "common.h"

namespace draco
{
    class Mesh;
}

namespace libgltf
{
    class IGoogleDracoStream
    {
    public:
        virtual bool UseAttributeID() const = 0;
        virtual size_t GetAttributeID() const = 0;

    public:
        virtual bool operator<<(const SAccessorData& _AccessorData) = 0;

    public:
        static std::shared_ptr<IGoogleDracoStream> Create(std::shared_ptr<IAccessorStream> _pAccessorStream, bool _bUseAttributeID, size_t _AttributeID);
    };

    class CGoogleDraco
    {
        struct SDracoPointAttribute
        {
            explicit SDracoPointAttribute();

            SAccessorData accessorData;
            std::vector<uint8_t> buffer;
        };

        struct SDracoMesh
        {
            explicit SDracoMesh(std::shared_ptr<class draco::Mesh> _pDracoMesh);

            std::shared_ptr<class draco::Mesh> dracoMesh;
            std::vector<uint32_t> indices;
            std::map<size_t, SDracoPointAttribute> pointAttributes;
        };

    public:
        CGoogleDraco();

    public:
        bool GetOrDecode(size_t _Index, const SBufferData& _BufferDataEncoded, std::shared_ptr<IGoogleDracoStream> _pGoogleDracoStream);

    private:
        std::map<size_t, std::shared_ptr<SDracoMesh> > m_DracoMeshes;
    };
}

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

#include "libgltfpch.h"
#include "libgltf.h"

#include "gltf_loader.h"
#include "utility.h"

namespace libgltf
{
    int32_t AccessorComponentTypeToInt32(EAccessorComponentType _eType)
    {
        return static_cast<int32_t>(GSAccessorComponentTypes[uint32_t(_eType)].value);
    }

    EAccessorComponentType Int32ToAccessorComponentType(int32_t _iValue)
    {
        for (uint32_t i = 0; i < uint32_t(EAccessorComponentType::MAX); ++i)
        {
            if (GSAccessorComponentTypes[i].value != _iValue) continue;
            return EAccessorComponentType(i);
        }
        return EAccessorComponentType::NONE;
    }

    const libgltf::string_t& AccessorTypeToText(EAccessorType _eType)
    {
        return GSAccessorTypes[uint8_t(_eType)].text;
    }

    EAccessorType TextToAccessorType(const string_t& _sText, bool _bCaseCensitive /*= true*/)
    {
        for (uint8_t i = 0; i < uint8_t(EAccessorType::MAX); ++i)
        {
            if (!StringEqual(GSAccessorTypes[i].text, _sText, _bCaseCensitive)) continue;
            return EAccessorType(i);
        }
        return EAccessorType::NONE;
    }

    size_t SizeOfAccessorComponentType(EAccessorComponentType _eType)
    {
        return GSAccessorComponentTypes[uint32_t(_eType)].size;
    };

    size_t DimensionOfAccessorType(EAccessorType _eType)
    {
        return GSAccessorTypes[uint8_t(_eType)].dimension;
    }

    size_t SizeOfAccessor(EAccessorComponentType _eAccessorComponentType, size_t _iCount, EAccessorType _eAccessorType)
    {
        return (SizeOfAccessorComponentType(_eAccessorComponentType) * _iCount * DimensionOfAccessorType(_eAccessorType));
    }

    SBufferData::SBufferData()
        : buffer(nullptr)
        , bufferSize(0)
        , bufferStride(0)
    {
        //
    }

    SAccessorData::SAccessorData()
        : componentType(EAccessorComponentType::NONE)
        , count(0)
        , type(EAccessorType::NONE)
        , bufferStride(0)
        , bufferData()
    {
        //
    }

    std::shared_ptr<IglTFLoader> IglTFLoader::Create(const string_t& file)
    {
        return std::make_shared<CGlTFLoader>(file);
    }
}

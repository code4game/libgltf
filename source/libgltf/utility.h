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

namespace libgltf
{
    bool StringEqual(const std::string& value0, const std::string& value1, bool case_sensitive = true);
    bool StringStartWith(const std::string& value_target, const std::string& value_start, bool case_sensitive = true);

    template<class TType>
    bool StringMapFind(const std::map<std::string, TType>& value_source, const std::string& value_target, TType& value_founded, bool case_sensitive = true)
    {
        if (case_sensitive)
        {
            typename std::map<std::string, TType>::const_iterator iterator_founded = value_source.find(value_target);
            if (iterator_founded == value_source.cend()) return false;
            value_founded = iterator_founded->second;
            return true;
        }
        for (const std::pair<std::string, TType>& item : value_source)
        {
            if (!StringEqual(item.first, value_target, case_sensitive)) continue;
            value_founded = item.second;
            return true;
        }
        return false;
    }

    namespace base64
    {
        bool Encode(const std::vector<uint8_t>& data, std::string& value);
        bool Decode(const std::string& value, std::vector<uint8_t>& data);
        std::string Encode(const std::string& value);
        std::string Decode(const std::string& value);
    }

    bool UriParse(const std::string& value, std::string& data_type, std::string& data_encode, size_t& data_start);
}

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

#pragma once

#include "libgltf/libgltf.h"

namespace libgltf
{
    bool StringEqual(const string_t& value0, const string_t& value1, bool case_sensitive = true);
    bool StringStartWith(const string_t& value_target, const string_t& value_start, bool case_sensitive = true);

    template<class TType>
    bool StringMapFind(const std::map<string_t, TType>& value_source, const string_t& value_target, TType& value_founded, bool case_sensitive = true)
    {
        if (case_sensitive)
        {
            typename std::map<string_t, TType>::const_iterator iterator_founded = value_source.find(value_target);
            if (iterator_founded == value_source.cend()) return false;
            value_founded = iterator_founded->second;
            return true;
        }
        for (const std::pair<string_t, TType>& item : value_source)
        {
            if (!StringEqual(item.first, value_target, case_sensitive)) continue;
            value_founded = item.second;
            return true;
        }
        return false;
    }

    namespace base64
    {
        bool Encode(const std::vector<uint8_t>& data, string_t& value);
        bool Decode(const string_t& value, std::vector<uint8_t>& data);
        string_t Encode(const string_t& value);
        string_t Decode(const string_t& value);
    }

    bool UriParse(const string_t& value, string_t& data_type, string_t& data_encode, size_t& data_start);
}

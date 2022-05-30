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

#include "utility.h"
#include "libgltfparser.h"

#include "common.h"

#include <iostream>

namespace libgltf
{
    int8_t CharacterIsAlphabet(std::string::value_type v)
    {
        if (v >= 'a' && v <= 'z') return -1;
        if (v >= 'A' && v <= 'Z') return 1;
        return 0;
    }

    std::string::value_type CharacterToLower(std::string::value_type v)
    {
        int8_t is_alphabet = CharacterIsAlphabet(v);
        if (is_alphabet == 0 || is_alphabet == -1) return v;
        return v + 'a' - 'A';
    }

    bool CharacterEqual(std::string::value_type v0, std::string::value_type v1, bool case_sensitive = true)
    {
        if (case_sensitive) return (v0 == v1);
        return (CharacterToLower(v0) == CharacterToLower(v1));
    }

    bool StringEqual(const std::string& value0, const std::string& value1, bool case_sensitive /*= true*/)
    {
        if (value0.size() != value1.size()) return false;
        if (case_sensitive) return (value0.compare(value1) == 0);
        for (size_t i = 0; i < value1.size(); ++i)
        {
            if (CharacterEqual(value0[i], value1[i], case_sensitive)) continue;
            return false;
        }
        return true;
    }

    bool StringStartWith(const std::string& value_target, const std::string& value_start, bool case_sensitive /*= true*/)
    {
        if (value_target.size() < value_start.size()) return false;
        return StringEqual(value_target.substr(0, value_start.size()), value_start, case_sensitive);
    }

    namespace base64
    {
        /// reference: https://github.com/ReneNyffenegger/cpp-base64
        static const std::string Base64Chars = u8"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

        static inline bool IsIt(char v)
        {
            return (isalnum(v) || (v == '+') || (v == '/'));
        }

        bool Encode(const std::vector<uint8_t>& data, std::string& value)
        {
            value.clear();
            if (data.empty()) return true;

            std::string result;
            size_t i = 0;
            uint8_t char_array_3[3];
            uint8_t char_array_4[4];

            size_t index = 0;
            size_t in_len = data.size();
            while (index < in_len)
            {
                char_array_3[i++] = data[index++];
                if (i == 3)
                {
                    char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
                    char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
                    char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
                    char_array_4[3] = char_array_3[2] & 0x3f;

                    for (i = 0; (i < 4); i++)
                    {
                        result += Base64Chars[char_array_4[i]];
                    }
                    i = 0;
                }
            }

            if (i)
            {
                for (size_t j = i; j < 3; j++)
                {
                    char_array_3[j] = '\0';
                }

                char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
                char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
                char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);

                for (size_t j = 0; (j < i + 1); j++)
                {
                    result += Base64Chars[char_array_4[j]];
                }

                while ((i++ < 3))
                {
                    result += '=';
                }
            }

            value = result;
            return true;
        }

        bool Decode(const std::string& value, std::vector<uint8_t>& data)
        {
            data.clear();
            if (value.empty()) return true;

            size_t in_len = value.size();
            size_t i = 0;
            size_t in_ = 0;
            uint8_t char_array_4[4], char_array_3[3];

            while (in_len-- && (value[in_] != '='))
            {
                if (!IsIt(value[in_])) return false;
                char_array_4[i++] = value[in_]; in_++;
                if (i == 4)
                {
                    for (i = 0; i < 4; i++)
                    {
                        char_array_4[i] = static_cast<uint8_t>(Base64Chars.find(char_array_4[i]));
                    }

                    char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
                    char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
                    char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

                    for (i = 0; (i < 3); i++)
                    {
                        data.push_back(char_array_3[i]);
                    }
                    i = 0;
                }
            }

            if (i)
            {
                for (size_t j = 0; j < i; j++)
                {
                    char_array_4[j] = static_cast<uint8_t>(Base64Chars.find(char_array_4[j]));
                }

                char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
                char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);

                for (size_t j = 0; (j < i - 1); j++)
                {
                    data.push_back(char_array_3[j]);
                }
            }
            return true;
        }

        std::string Encode(const std::string& value)
        {
            if (value.empty()) return value;

            std::vector<uint8_t> data;
            data.resize(value.length());
            ::memcpy((void*)data.data(), (void*)value.data(), sizeof(uint8_t) * data.size());
            std::string result;
            Encode(data, result);
            return result;
        }

        std::string Decode(const std::string& value)
        {
            if (value.empty()) return value;

            std::vector<uint8_t> data;
            if (!Decode(value, data) || data.empty()) return std::string();

            std::string result;
            result.resize(data.size());
            ::memcpy((void*)result.data(), (void*)data.data(), sizeof(uint8_t) * data.size());
            return result;
        }
    }

    bool UriParse(const std::string& value, std::string& data_type, std::string& data_encode, size_t& data_start)
    {
        static const std::string data = "data:";
        if (!StringStartWith(value, data)) return false;
        static const size_t data_index = data.length();

        size_t slash_index = value.find_first_of("/", data_index);
        if (slash_index >= value.length()) return false;
        ++slash_index;

        size_t semicolon_index = value.find_first_of(";", slash_index);
        if (semicolon_index >= value.length()) return false;
        ++semicolon_index;

        size_t comma_index = value.find_first_of(",", semicolon_index);
        if (comma_index >= value.length()) return false;
        ++comma_index;

        data_type = value.substr(data_index, semicolon_index - data_index - 1);
        data_encode = value.substr(semicolon_index, comma_index - semicolon_index - 1);
        data_start = comma_index;
        return true;
    }
}

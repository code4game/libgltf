/*
 * This software is released under the MIT license.
 *
 * Copyright (c) 2017-2021 Alex Chi, The Code 4 Game Organization
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

#include "common.h"

#include <cassert>
#include <codecvt>
#include <locale>

namespace libgltf
{
    void Verify(bool condition)
    {
        assert(condition);
#ifdef NDEBUG
        if (!condition) ::abort();
#endif
    }

    std::u16string UTF8ToUTF16(const std::string& _sValue)
    {
#if defined(_MSC_VER) && _MSC_VER >= 1900 && _MSC_VER < 2000
        static_assert(sizeof(int16_t) == sizeof(char16_t), "int32_t and char32_t should have same size");
        auto converted_content = std::wstring_convert<std::codecvt_utf8_utf16<int16_t>, int16_t>{}.from_bytes(_sValue.c_str());
        std::u16string res;
        res = (reinterpret_cast<const char16_t*>(converted_content.data()));
        return res;
#else
        return std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t>{}.from_bytes(_sValue.c_str());
#endif
    }

    std::string UTF16ToUTF8(const std::u16string& _sValue)
    {
#if defined(_MSC_VER) && _MSC_VER >= 1900 && _MSC_VER < 2000
        static_assert(sizeof(int16_t) == sizeof(char16_t), "int32_t and char32_t should have same size");
        return std::wstring_convert<std::codecvt_utf8_utf16<int16_t>, int16_t>{}.to_bytes(reinterpret_cast<const int16_t*>(_sValue.c_str())).data();
#else
        return std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t>{}.to_bytes(_sValue.c_str());
#endif
    }

    std::u32string UTF8ToUTF32(const std::string& _sValue)
    {
#if defined(_MSC_VER) && _MSC_VER >= 1900 && _MSC_VER < 2000
        static_assert(sizeof(int32_t) == sizeof(char32_t), "int32_t and char32_t should have same size");
        auto converted_content = std::wstring_convert<std::codecvt_utf8_utf16<int32_t>, int32_t>{}.from_bytes(_sValue.c_str());
        std::u32string res;
        res = (reinterpret_cast<const char32_t*>(converted_content.data()));
        return res;
#else
        return std::wstring_convert<std::codecvt_utf8_utf16<char32_t>, char32_t>{}.from_bytes(_sValue.c_str());
#endif
    }

    std::string UTF32ToUTF8(const std::u32string& _sValue)
    {
#if defined(_MSC_VER) && _MSC_VER >= 1900 && _MSC_VER < 2000
        static_assert(sizeof(int32_t) == sizeof(char32_t), "int32_t and char32_t should have same size");
        return std::wstring_convert<std::codecvt_utf8_utf16<int32_t>, int32_t>{}.to_bytes(reinterpret_cast<const int32_t*>(_sValue.c_str())).data();
#else
        return std::wstring_convert<std::codecvt_utf8_utf16<char32_t>, char32_t>{}.to_bytes(_sValue.c_str());
#endif
    }

    std::wstring UTF8ToUNICODE(const std::string& _sValue)
    {
        return std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>{}.from_bytes(_sValue.c_str());
    }

    std::string UNICODEToUTF8(const std::wstring& _sValue)
    {
        return std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>{}.to_bytes(_sValue.c_str());
    }
}

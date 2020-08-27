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

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

namespace libgltf
{
#if defined(LIBGLTF_CHARACTOR_ENCODING_IS_UTF16)
    typedef char16_t                                GLTFChar;
#elif defined(LIBGLTF_CHARACTOR_ENCODING_IS_UTF32)
    typedef char32_t                                GLTFChar;
#elif defined(LIBGLTF_CHARACTOR_ENCODING_IS_UNICODE)
    typedef wchar_t                                 GLTFChar;
#else
    typedef char                                    GLTFChar;
#endif

#if defined(LIBGLTF_CHARACTOR_ENCODING_IS_UTF16)
    typedef rapidjson::UTF16<GLTFChar>              JSONChar;
#elif defined(LIBGLTF_CHARACTOR_ENCODING_IS_UTF32)
    typedef rapidjson::UTF32<GLTFChar>              JSONChar;
#elif defined(LIBGLTF_CHARACTOR_ENCODING_IS_UNICODE)
    typedef rapidjson::UTF16<GLTFChar>              JSONChar;
#else
    typedef rapidjson::UTF8<GLTFChar>               JSONChar;
#endif

    typedef rapidjson::SizeType                     JSONSizeType;

    typedef rapidjson::GenericDocument<JSONChar>                GLTFCharDocument;
    typedef rapidjson::GenericValue<JSONChar>                   GLTFCharValue;
    typedef rapidjson::GenericStringRef<GLTFChar>               GLTFStringRef;
    typedef rapidjson::GenericStringBuffer<JSONChar>            GLTFStringBuffer;
    typedef rapidjson::Writer<GLTFStringBuffer, JSONChar>       GLTFWriter;

    typedef rapidjson::GenericArray<true, GLTFCharValue>        GLTFCharConstArray;
    typedef rapidjson::GenericArray<false, GLTFCharValue>       GLTFCharArray;
    typedef rapidjson::GenericObject<true, GLTFCharValue>       GLTFCharConstObject;
    typedef rapidjson::GenericObject<false, GLTFCharValue>      GLTFCharObject;
}


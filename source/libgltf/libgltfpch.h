#pragma once

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#if defined(UNICODE)
#if defined(USING_CHAR16) || defined(USING_CHAR32)
#   define      GLTFTEXT(t)         u##t
#else
#   define      GLTFTEXT(t)         L##t
#endif
#else
#   define      GLTFTEXT(t)         t
#endif

namespace libgltf
{
#if defined(UNICODE)
#if defined(USING_CHAR16)
    typedef rapidjson::UTF16<char16_t>                          GLTFCharType;
#elif defined(USING_CHAR32)
    typedef rapidjson::UTF32<char32_t>                          GLTFCharType;
#else
    typedef rapidjson::UTF8<wchar_t>                            GLTFCharType;
#endif
#else
    typedef rapidjson::UTF8<char>                               GLTFCharType;
#endif

    typedef rapidjson::GenericDocument<GLTFCharType>            GLTFCharDocument;
    typedef rapidjson::GenericValue<GLTFCharType>               GLTFCharValue;
    typedef rapidjson::GenericStringBuffer<GLTFCharType>        GLTFStringBuffer;
    typedef rapidjson::Writer<GLTFStringBuffer, GLTFCharType>   GLTFWriter;

    typedef rapidjson::GenericArray<true, GLTFCharValue>        GLTFCharConstArray;
    typedef rapidjson::GenericArray<false, GLTFCharValue>       GLTFCharArray;
    typedef rapidjson::GenericObject<true, GLTFCharValue>       GLTFCharConstObject;
    typedef rapidjson::GenericObject<false, GLTFCharValue>      GLTFCharObject;
}


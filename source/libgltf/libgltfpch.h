#pragma once

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#if defined(UNICODE)
#   define      GLTFTEXT(t)         L##t
#else
#   define      GLTFTEXT(t)         t
#endif

namespace libgltf
{
#if defined(UNICODE)
    typedef rapidjson::UTF16<>                                  GLTFCharType;
#else
    typedef rapidjson::UTF8<>                                   GLTFCharType;
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


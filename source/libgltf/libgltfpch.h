#pragma once

#include <rapidjson/document.h>

#if defined(PLATFORM_WINDOWS)
#   if defined(UNICODE)
#       define      GLTFTEXT(t)         L##t
#   else
#       define      GLTFTEXT(t)         t
#   endif
#else
#       define      GLTFTEXT(t)         t
#endif

namespace libgltf
{
#if defined(PLATFORM_WINDOWS)
#   if defined(UNICODE)
    typedef rapidjson::GenericDocument<rapidjson::UTF16<>>      GLTFCharDocument;
    typedef rapidjson::GenericValue<rapidjson::UTF16<>>         GLTFCharValue;
#   else
    typedef rapidjson::GenericDocument<rapidjson::UTF8<>>      GLTFCharDocument;
    typedef rapidjson::GenericValue<rapidjson::UTF8<>>         GLTFCharValue;
#   endif
#else
    typedef rapidjson::GenericDocument<rapidjson::UTF8<>>      GLTFCharDocument;
    typedef rapidjson::GenericValue<rapidjson::UTF8<>>         GLTFCharValue;
#endif
    typedef rapidjson::GenericArray<true, GLTFCharValue>        GLTFCharConstArray;
    typedef rapidjson::GenericArray<false, GLTFCharValue>       GLTFCharArray;
    typedef rapidjson::GenericObject<true, GLTFCharValue>       GLTFCharConstObject;
    typedef rapidjson::GenericObject<false, GLTFCharValue>      GLTFCharObject;
}

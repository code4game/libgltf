#pragma once

#include <rapidjson/document.h>

namespace libgltf
{
    typedef rapidjson::GenericDocument<rapidjson::UTF16<>>      GLTFCharDocument;
    typedef rapidjson::GenericValue<rapidjson::UTF16<>>         GLTFCharValue;
    typedef rapidjson::GenericArray<true, GLTFCharValue>        GLTFCharConstArray;
    typedef rapidjson::GenericArray<false, GLTFCharValue>       GLTFCharArray;
    typedef rapidjson::GenericObject<true, GLTFCharValue>       GLTFCharConstObject;
    typedef rapidjson::GenericObject<false, GLTFCharValue>      GLTFCharObject;
}

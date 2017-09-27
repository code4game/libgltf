#pragma once

#include <rapidjson/document.h>

namespace libgltf
{
    typedef rapidjson::GenericDocument<rapidjson::UTF16<>>      WCharDocument;
    typedef rapidjson::GenericValue<rapidjson::UTF16<>>         WCharValue;
    typedef rapidjson::GenericArray<true, WCharValue>           WCharConstArray;
    typedef rapidjson::GenericArray<false, WCharValue>          WCharArray;
    typedef rapidjson::GenericObject<true, WCharValue>          WCharConstObject;
    typedef rapidjson::GenericObject<false, WCharValue>         WCharObject;
}

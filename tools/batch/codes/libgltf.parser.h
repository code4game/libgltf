#include "libgltf/libgltf.h"

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

namespace libgltf
{
    typedef rapidjson::UTF8<char>                          JSONChar;
    typedef rapidjson::SizeType                            JSONSizeType;
    typedef rapidjson::GenericDocument<JSONChar>           JSONCharDocument;
    typedef rapidjson::GenericValue<JSONChar>              JSONCharValue;
    typedef rapidjson::GenericStringRef<char>              JSONStringRef;
    typedef rapidjson::GenericStringBuffer<JSONChar>       JSONStringBuffer;
    typedef rapidjson::Writer<JSONStringBuffer, JSONChar>  JSONWriter;
    typedef rapidjson::GenericArray<true, JSONCharValue>   JSONCharConstArray;
    typedef rapidjson::GenericArray<false, JSONCharValue>  JSONCharArray;
    typedef rapidjson::GenericObject<true, JSONCharValue>  JSONCharConstObject;
    typedef rapidjson::GenericObject<false, JSONCharValue> JSONCharObject;
}

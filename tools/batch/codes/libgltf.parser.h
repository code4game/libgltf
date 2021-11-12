#include "libgltf/libgltf.h"

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include <memory>
#include <vector>

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

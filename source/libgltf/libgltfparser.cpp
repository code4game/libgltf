#include "libgltf.h"

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>

namespace libgltf
{
    typedef rapidjson::GenericDocument<rapidjson::UTF16<>>  WCharDocument;
    typedef rapidjson::GenericValue<rapidjson::UTF16<>>     WCharValue;
    typedef rapidjson::GenericObject<true, WCharValue>      WCharConstObject;
    typedef rapidjson::GenericObject<false, WCharValue>     WCharObject;

    bool ParseByString(const std::wstring& _sContent, std::shared_ptr<SGlTF>& _pGlTF)
    {
        WCharDocument json_doc;
        json_doc.Parse(_sContent.c_str());
        if (!json_doc.IsObject())
        {
            return false;
        }

        _pGlTF = std::make_shared<SGlTF>();

        const WCharObject& json_root = json_doc.GetObject();
        {
            const WCharObject::ConstMemberIterator& json_member_asset = json_root.FindMember(L"asset");
            if (json_member_asset != json_root.end())
            {
                _pGlTF->asset = std::make_shared<SAsset>();

                const WCharConstObject& json_asset = json_member_asset->value.GetObject();
                const WCharObject::ConstMemberIterator& json_member_asset_generator = json_asset.FindMember(L"generator");
                if (json_member_asset_generator != json_asset.end())
                {
                    _pGlTF->asset->generator = json_member_asset_generator->value.GetString();
                }
            }
        }

        //
        return false;
    }
}

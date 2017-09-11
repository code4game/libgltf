#include "libgltfpch.h"
#include "libgltfparser.h"

namespace libgltf
{
    bool operator<<(std::shared_ptr<SGlTF>& _pGlTF, const WCharValue& _JsonValue)
    {
        std::shared_ptr<SGlTF> result_ptr = std::make_shared<SGlTF>();

        const WCharObject::ConstMemberIterator& json_member_asset = _JsonValue.FindMember(L"asset");
        if (json_member_asset != _JsonValue.MemberEnd())
        {
            if (!(result_ptr->asset << json_member_asset->value)) return false;
        }

        //

        _pGlTF = result_ptr;
        return true;
    }

    bool operator<<(std::shared_ptr<SAsset>& _pAsset, const WCharValue& _JsonValue)
    {
        std::shared_ptr<SAsset> result_ptr = std::make_shared<SAsset>();

        const WCharConstObject& json_asset = _JsonValue.GetObject();
        const WCharObject::ConstMemberIterator& json_member_generator = json_asset.FindMember(L"generator");
        if (json_member_generator != json_asset.MemberEnd())
        {
            result_ptr->generator = json_member_generator->value.GetString();
        }

        //

        _pAsset = result_ptr;
        return true;
    }

    bool operator<<(std::shared_ptr<SExtras>& _pExtras, const WCharValue& _JsonValue)
    {
        std::shared_ptr<SExtras> result_ptr = std::make_shared<SExtras>();
        //
        return true;
    }

    bool ParseByString(const std::wstring& _sContent, std::shared_ptr<SGlTF>& _pGlTF)
    {
        WCharDocument json_doc;
        json_doc.Parse(_sContent.c_str());
        if (!json_doc.IsObject()) return false;

        return (_pGlTF << json_doc);
    }
}

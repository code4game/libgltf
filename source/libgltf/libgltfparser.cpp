#include "libgltfpch.h"
#include "libgltfparser.h"
#include "libgltf.h"

namespace libgltf
{
    class GLTFCharDocumentPtrWrapper
    {
    public:
        explicit GLTFCharDocumentPtrWrapper(GLTFCharDocument*& _pDocument) : m_pDocument(_pDocument) { }
        ~GLTFCharDocumentPtrWrapper() { m_pDocument = nullptr; }
    private:
        GLTFCharDocument*& m_pDocument;
    };
    GLTFCharDocument* g_json_doc_ptr = nullptr;

    bool operator<<(std::shared_ptr<SGlTF>& _pGlTF, const GLTFString& _sContent)
    {
        GLTFCharDocument json_doc;
        json_doc.Parse(_sContent.c_str());
        if (!json_doc.IsObject()) return false;
        return (_pGlTF << json_doc.GetObject());
    }

    bool operator>>(const std::shared_ptr<SGlTF>& _pGlTF, GLTFString& _sContent)
    {
        if (!_pGlTF) return false;
        GLTFCharDocument json_doc;
        g_json_doc_ptr = &json_doc;
        GLTFCharDocumentPtrWrapper wrapper(g_json_doc_ptr);
        if (!(_pGlTF >> json_doc)) return false;
        GLTFStringBuffer json_string_buffer;
        GLTFWriter json_writer(json_string_buffer);
        if (!json_doc.Accept(json_writer)) return false;
        _sContent = json_string_buffer.GetString();
        return true;
    }

    bool operator<<(bool& _rData, const GLTFCharValue& _JsonValue)
    {
        if (!_JsonValue.IsBool()) return false;
        _rData = _JsonValue.GetBool();
        return true;
    }

    bool operator>>(bool _bData, GLTFCharValue& _JsonValue)
    {
        _JsonValue.SetBool(_bData);
        return true;
    }

    bool operator<<(int32_t& _rData, const GLTFCharValue& _JsonValue)
    {
        if (!_JsonValue.IsInt()) return false;
        _rData = _JsonValue.GetInt();
        return true;
    }

    bool operator>>(int32_t _iData, GLTFCharValue& _JsonValue)
    {
        _JsonValue.SetInt(_iData);
        return true;
    }

    bool operator<<(float& _rData, const GLTFCharValue& _JsonValue)
    {
        if (_JsonValue.IsFloat())
        {
            _rData = _JsonValue.GetFloat();
            return true;
        }
        if (_JsonValue.IsInt())
        {
            _rData = static_cast<float>(_JsonValue.GetInt());
            return true;
        }
        return false;
    }

    bool operator>>(float _fData, GLTFCharValue& _JsonValue)
    {
        _JsonValue.SetFloat(_fData);
        return true;
    }

    bool operator<<(GLTFString& _rData, const GLTFCharValue& _JsonValue)
    {
        if (!_JsonValue.IsString()) return false;
        _rData = _JsonValue.GetString();
        return true;
    }

    bool operator>>(const GLTFString& _rsData, GLTFCharValue& _JsonValue)
    {
        if (!g_json_doc_ptr) return false;
        _JsonValue.SetString(_rsData.c_str(), g_json_doc_ptr->GetAllocator());
        return true;
    }

    template<typename TData>
    bool operator<<(std::vector<TData>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        if (!_JsonValue.IsArray()) return false;
        std::vector<TData> datas;
        const GLTFCharConstArray& json_array = _JsonValue.GetArray();
        size_t len = json_array.Size();
        if (len == 0) return true;
        datas.resize(len);
        for (size_t i = 0; i < len; ++i)
        {
            if (datas[i] << json_array[static_cast<rapidjson::SizeType>(i)]) continue;
            return false;
        }
        _vDatas = datas;
        return true;
    }

    template<typename TData>
    bool operator>>(const std::vector<TData>& _vDatas, GLTFCharValue& _JsonValue)
    {
        if (_vDatas.empty() || !g_json_doc_ptr) return false;
        GLTFCharValue& json_array = _JsonValue.SetArray();
        for (const TData& data : _vDatas)
        {
            GLTFCharValue json_value;
            if (!(data >> json_value)) return false;
            json_array.PushBack(json_value, g_json_doc_ptr->GetAllocator());
        }
        return true;
    }

    template<typename TData>
    bool operator<<(std::map<GLTFString, TData>& _mDatas, const GLTFCharValue& _JsonValue)
    {
        if (!_JsonValue.IsObject()) return false;
        std::map<GLTFString, TData> datas;
        const GLTFCharConstObject& json_object = _JsonValue.GetObject();
        for (GLTFCharConstObject::ConstMemberIterator cit = json_object.MemberBegin(); cit != json_object.MemberEnd(); ++cit)
        {
            TData data;
            if (!(data << cit->value)) return false;
            datas.insert(std::make_pair(cit->name.GetString(), data));
        }
        _mDatas = datas;
        return true;
    }

    template<typename TData>
    bool operator>>(const std::map<GLTFString, TData>& _mDatas, GLTFCharValue& _JsonValue)
    {
        if (_mDatas.empty() || !g_json_doc_ptr) return false;
        _JsonValue.SetObject();
        for (const std::pair<GLTFString, TData>& data : _mDatas)
        {
            GLTFCharValue json_value;
            if (!(data.second >> json_value)) return false;
            GLTFCharValue json_key(data.first.c_str(), g_json_doc_ptr->GetAllocator());
            _JsonValue.AddMember(json_key, json_value, g_json_doc_ptr->GetAllocator());
        }
        return true;
    }

    bool operator<<(SGlTFProperty& _rData, const GLTFCharValue& _JsonValue)
    {
        if (_JsonValue.HasMember(GLTFTEXT("extras")) && _JsonValue[GLTFTEXT("extras")].IsObject())
        {
            if (!(_rData.extras << _JsonValue[GLTFTEXT("extras")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("extensions")) && _JsonValue[GLTFTEXT("extensions")].IsObject())
        {
            if (!(_rData.extensions << _JsonValue[GLTFTEXT("extensions")])) return false;
        }
        _rData.schemaType = GLTFTEXT("glTFProperty.schema.json");
        return true;
    }

    bool operator>>(const SGlTFProperty& _rData, GLTFCharValue& _JsonValue)
    {
        if (!g_json_doc_ptr) return false;
        _JsonValue.SetObject();
        if (!!_rData.extras)
        {
            GLTFCharValue json_value;
            if (!(_rData.extras >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("extras"), json_value, g_json_doc_ptr->GetAllocator());
        }
        if (!!_rData.extensions)
        {
            GLTFCharValue json_value;
            if (!(_rData.extensions >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("extensions"), json_value, g_json_doc_ptr->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SGlTFProperty>& _pData, const GLTFCharValue& _JsonValue)
    {
        std::shared_ptr<SGlTFProperty> data_ptr = !!_pData ? _pData : std::make_shared<SGlTFProperty>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const std::shared_ptr<SGlTFProperty>& _pData, GLTFCharValue& _JsonValue)
    {
        if (!_pData) return false;
        return (*_pData >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SGlTFProperty>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SGlTFProperty>>(_vDatas, _JsonValue);
    }

    bool operator>>(const std::vector<std::shared_ptr<SGlTFProperty>>& _vDatas, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SGlTFProperty>>(_vDatas, _JsonValue);
    }

    bool operator<<(SMaterial& _rData, const GLTFCharValue& _JsonValue)
    {
        {
            SGlTFChildofRootProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("alphaCutoff")) && _JsonValue[GLTFTEXT("alphaCutoff")].IsFloat())
        {
            _rData.alphaCutoff = _JsonValue[GLTFTEXT("alphaCutoff")].GetFloat();
        }
        if (_JsonValue.HasMember(GLTFTEXT("emissiveTexture")) && _JsonValue[GLTFTEXT("emissiveTexture")].IsObject())
        {
            if (!(_rData.emissiveTexture << _JsonValue[GLTFTEXT("emissiveTexture")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("pbrMetallicRoughness")) && _JsonValue[GLTFTEXT("pbrMetallicRoughness")].IsObject())
        {
            if (!(_rData.pbrMetallicRoughness << _JsonValue[GLTFTEXT("pbrMetallicRoughness")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("occlusionTexture")) && _JsonValue[GLTFTEXT("occlusionTexture")].IsObject())
        {
            if (!(_rData.occlusionTexture << _JsonValue[GLTFTEXT("occlusionTexture")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("alphaMode")) && _JsonValue[GLTFTEXT("alphaMode")].IsString())
        {
            _rData.alphaMode = _JsonValue[GLTFTEXT("alphaMode")].GetString();
        }
        if (_JsonValue.HasMember(GLTFTEXT("doubleSided")) && _JsonValue[GLTFTEXT("doubleSided")].IsBool())
        {
            _rData.doubleSided = _JsonValue[GLTFTEXT("doubleSided")].GetBool();
        }
        if (_JsonValue.HasMember(GLTFTEXT("normalTexture")) && _JsonValue[GLTFTEXT("normalTexture")].IsObject())
        {
            if (!(_rData.normalTexture << _JsonValue[GLTFTEXT("normalTexture")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("emissiveFactor")) && _JsonValue[GLTFTEXT("emissiveFactor")].IsArray())
        {
            if (!(_rData.emissiveFactor << _JsonValue[GLTFTEXT("emissiveFactor")])) return false;
        }
        _rData.schemaType = GLTFTEXT("material.schema.json");
        return true;
    }

    bool operator>>(const SMaterial& _rData, GLTFCharValue& _JsonValue)
    {
        if (!g_json_doc_ptr) return false;
        _JsonValue.SetObject();
        {
            const SGlTFChildofRootProperty& super_ptr = _rData;
            if (!(super_ptr >> _JsonValue)) return false;
        }
        {
            GLTFCharValue json_value;
            if (!(_rData.alphaCutoff >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("alphaCutoff"), json_value, g_json_doc_ptr->GetAllocator());
        }
        if (!!_rData.emissiveTexture)
        {
            GLTFCharValue json_value;
            if (!(_rData.emissiveTexture >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("emissiveTexture"), json_value, g_json_doc_ptr->GetAllocator());
        }
        if (!!_rData.pbrMetallicRoughness)
        {
            GLTFCharValue json_value;
            if (!(_rData.pbrMetallicRoughness >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("pbrMetallicRoughness"), json_value, g_json_doc_ptr->GetAllocator());
        }
        if (!!_rData.occlusionTexture)
        {
            GLTFCharValue json_value;
            if (!(_rData.occlusionTexture >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("occlusionTexture"), json_value, g_json_doc_ptr->GetAllocator());
        }
        {
            GLTFCharValue json_value;
            if (!(_rData.alphaMode >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("alphaMode"), json_value, g_json_doc_ptr->GetAllocator());
        }
        {
            GLTFCharValue json_value;
            if (!(_rData.doubleSided >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("doubleSided"), json_value, g_json_doc_ptr->GetAllocator());
        }
        if (!!_rData.normalTexture)
        {
            GLTFCharValue json_value;
            if (!(_rData.normalTexture >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("normalTexture"), json_value, g_json_doc_ptr->GetAllocator());
        }
        if (!_rData.emissiveFactor.empty())
        {
            GLTFCharValue json_value;
            if (!(_rData.emissiveFactor >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("emissiveFactor"), json_value, g_json_doc_ptr->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SMaterial>& _pData, const GLTFCharValue& _JsonValue)
    {
        std::shared_ptr<SMaterial> data_ptr = !!_pData ? _pData : std::make_shared<SMaterial>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const std::shared_ptr<SMaterial>& _pData, GLTFCharValue& _JsonValue)
    {
        if (!_pData) return false;
        return (*_pData >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SMaterial>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SMaterial>>(_vDatas, _JsonValue);
    }

    bool operator>>(const std::vector<std::shared_ptr<SMaterial>>& _vDatas, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SMaterial>>(_vDatas, _JsonValue);
    }

    bool operator<<(SAsset& _rData, const GLTFCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("minVersion")) && _JsonValue[GLTFTEXT("minVersion")].IsString())
        {
            _rData.minVersion = _JsonValue[GLTFTEXT("minVersion")].GetString();
        }
        if (_JsonValue.HasMember(GLTFTEXT("version")) && _JsonValue[GLTFTEXT("version")].IsString())
        {
            _rData.version = _JsonValue[GLTFTEXT("version")].GetString();
        }
        if (_JsonValue.HasMember(GLTFTEXT("generator")) && _JsonValue[GLTFTEXT("generator")].IsString())
        {
            _rData.generator = _JsonValue[GLTFTEXT("generator")].GetString();
        }
        if (_JsonValue.HasMember(GLTFTEXT("copyright")) && _JsonValue[GLTFTEXT("copyright")].IsString())
        {
            _rData.copyright = _JsonValue[GLTFTEXT("copyright")].GetString();
        }
        _rData.schemaType = GLTFTEXT("asset.schema.json");
        return true;
    }

    bool operator>>(const SAsset& _rData, GLTFCharValue& _JsonValue)
    {
        if (!g_json_doc_ptr) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr >> _JsonValue)) return false;
        }
        {
            GLTFCharValue json_value;
            if (!(_rData.minVersion >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("minVersion"), json_value, g_json_doc_ptr->GetAllocator());
        }
        {
            GLTFCharValue json_value;
            if (!(_rData.version >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("version"), json_value, g_json_doc_ptr->GetAllocator());
        }
        {
            GLTFCharValue json_value;
            if (!(_rData.generator >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("generator"), json_value, g_json_doc_ptr->GetAllocator());
        }
        {
            GLTFCharValue json_value;
            if (!(_rData.copyright >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("copyright"), json_value, g_json_doc_ptr->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SAsset>& _pData, const GLTFCharValue& _JsonValue)
    {
        std::shared_ptr<SAsset> data_ptr = !!_pData ? _pData : std::make_shared<SAsset>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const std::shared_ptr<SAsset>& _pData, GLTFCharValue& _JsonValue)
    {
        if (!_pData) return false;
        return (*_pData >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SAsset>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SAsset>>(_vDatas, _JsonValue);
    }

    bool operator>>(const std::vector<std::shared_ptr<SAsset>>& _vDatas, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SAsset>>(_vDatas, _JsonValue);
    }

    bool operator<<(SSampler& _rData, const GLTFCharValue& _JsonValue)
    {
        {
            SGlTFChildofRootProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("wrapS")) && _JsonValue[GLTFTEXT("wrapS")].IsInt())
        {
            _rData.wrapS = _JsonValue[GLTFTEXT("wrapS")].GetInt();
        }
        if (_JsonValue.HasMember(GLTFTEXT("minFilter")) && _JsonValue[GLTFTEXT("minFilter")].IsInt())
        {
            _rData.minFilter = _JsonValue[GLTFTEXT("minFilter")].GetInt();
        }
        if (_JsonValue.HasMember(GLTFTEXT("magFilter")) && _JsonValue[GLTFTEXT("magFilter")].IsInt())
        {
            _rData.magFilter = _JsonValue[GLTFTEXT("magFilter")].GetInt();
        }
        if (_JsonValue.HasMember(GLTFTEXT("wrapT")) && _JsonValue[GLTFTEXT("wrapT")].IsInt())
        {
            _rData.wrapT = _JsonValue[GLTFTEXT("wrapT")].GetInt();
        }
        _rData.schemaType = GLTFTEXT("sampler.schema.json");
        return true;
    }

    bool operator>>(const SSampler& _rData, GLTFCharValue& _JsonValue)
    {
        if (!g_json_doc_ptr) return false;
        _JsonValue.SetObject();
        {
            const SGlTFChildofRootProperty& super_ptr = _rData;
            if (!(super_ptr >> _JsonValue)) return false;
        }
        {
            GLTFCharValue json_value;
            if (!(_rData.wrapS >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("wrapS"), json_value, g_json_doc_ptr->GetAllocator());
        }
        {
            GLTFCharValue json_value;
            if (!(_rData.minFilter >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("minFilter"), json_value, g_json_doc_ptr->GetAllocator());
        }
        {
            GLTFCharValue json_value;
            if (!(_rData.magFilter >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("magFilter"), json_value, g_json_doc_ptr->GetAllocator());
        }
        {
            GLTFCharValue json_value;
            if (!(_rData.wrapT >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("wrapT"), json_value, g_json_doc_ptr->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SSampler>& _pData, const GLTFCharValue& _JsonValue)
    {
        std::shared_ptr<SSampler> data_ptr = !!_pData ? _pData : std::make_shared<SSampler>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const std::shared_ptr<SSampler>& _pData, GLTFCharValue& _JsonValue)
    {
        if (!_pData) return false;
        return (*_pData >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SSampler>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SSampler>>(_vDatas, _JsonValue);
    }

    bool operator>>(const std::vector<std::shared_ptr<SSampler>>& _vDatas, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SSampler>>(_vDatas, _JsonValue);
    }

    bool operator<<(SGlTFChildofRootProperty& _rData, const GLTFCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("name")) && _JsonValue[GLTFTEXT("name")].IsString())
        {
            _rData.name = _JsonValue[GLTFTEXT("name")].GetString();
        }
        _rData.schemaType = GLTFTEXT("glTFChildOfRootProperty.schema.json");
        return true;
    }

    bool operator>>(const SGlTFChildofRootProperty& _rData, GLTFCharValue& _JsonValue)
    {
        if (!g_json_doc_ptr) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr >> _JsonValue)) return false;
        }
        {
            GLTFCharValue json_value;
            if (!(_rData.name >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("name"), json_value, g_json_doc_ptr->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SGlTFChildofRootProperty>& _pData, const GLTFCharValue& _JsonValue)
    {
        std::shared_ptr<SGlTFChildofRootProperty> data_ptr = !!_pData ? _pData : std::make_shared<SGlTFChildofRootProperty>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const std::shared_ptr<SGlTFChildofRootProperty>& _pData, GLTFCharValue& _JsonValue)
    {
        if (!_pData) return false;
        return (*_pData >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SGlTFChildofRootProperty>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SGlTFChildofRootProperty>>(_vDatas, _JsonValue);
    }

    bool operator>>(const std::vector<std::shared_ptr<SGlTFChildofRootProperty>>& _vDatas, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SGlTFChildofRootProperty>>(_vDatas, _JsonValue);
    }

    bool operator<<(SAnimationSampler& _rData, const GLTFCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("input")) && _JsonValue[GLTFTEXT("input")].IsInt())
        {
            if (!(_rData.input << _JsonValue[GLTFTEXT("input")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("output")) && _JsonValue[GLTFTEXT("output")].IsInt())
        {
            if (!(_rData.output << _JsonValue[GLTFTEXT("output")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("interpolation")) && _JsonValue[GLTFTEXT("interpolation")].IsString())
        {
            _rData.interpolation = _JsonValue[GLTFTEXT("interpolation")].GetString();
        }
        _rData.schemaType = GLTFTEXT("animation.sampler.schema.json");
        return true;
    }

    bool operator>>(const SAnimationSampler& _rData, GLTFCharValue& _JsonValue)
    {
        if (!g_json_doc_ptr) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr >> _JsonValue)) return false;
        }
        if (!!_rData.input)
        {
            GLTFCharValue json_value;
            if (!(_rData.input >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("input"), json_value, g_json_doc_ptr->GetAllocator());
        }
        if (!!_rData.output)
        {
            GLTFCharValue json_value;
            if (!(_rData.output >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("output"), json_value, g_json_doc_ptr->GetAllocator());
        }
        {
            GLTFCharValue json_value;
            if (!(_rData.interpolation >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("interpolation"), json_value, g_json_doc_ptr->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SAnimationSampler>& _pData, const GLTFCharValue& _JsonValue)
    {
        std::shared_ptr<SAnimationSampler> data_ptr = !!_pData ? _pData : std::make_shared<SAnimationSampler>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const std::shared_ptr<SAnimationSampler>& _pData, GLTFCharValue& _JsonValue)
    {
        if (!_pData) return false;
        return (*_pData >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SAnimationSampler>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SAnimationSampler>>(_vDatas, _JsonValue);
    }

    bool operator>>(const std::vector<std::shared_ptr<SAnimationSampler>>& _vDatas, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SAnimationSampler>>(_vDatas, _JsonValue);
    }

    bool operator<<(SExtras& _rData, const GLTFCharValue& _JsonValue)
    {
        _rData.schemaType = GLTFTEXT("extras.schema.json");
        return true;
    }

    bool operator>>(const SExtras& _rData, GLTFCharValue& _JsonValue)
    {
        if (!g_json_doc_ptr) return false;
        _JsonValue.SetObject();
        return true;
    }

    bool operator<<(std::shared_ptr<SExtras>& _pData, const GLTFCharValue& _JsonValue)
    {
        std::shared_ptr<SExtras> data_ptr = !!_pData ? _pData : std::make_shared<SExtras>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const std::shared_ptr<SExtras>& _pData, GLTFCharValue& _JsonValue)
    {
        if (!_pData) return false;
        return (*_pData >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SExtras>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SExtras>>(_vDatas, _JsonValue);
    }

    bool operator>>(const std::vector<std::shared_ptr<SExtras>>& _vDatas, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SExtras>>(_vDatas, _JsonValue);
    }

    bool operator<<(SScene& _rData, const GLTFCharValue& _JsonValue)
    {
        {
            SGlTFChildofRootProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("nodes")) && _JsonValue[GLTFTEXT("nodes")].IsArray())
        {
            if (!(_rData.nodes << _JsonValue[GLTFTEXT("nodes")])) return false;
        }
        _rData.schemaType = GLTFTEXT("scene.schema.json");
        return true;
    }

    bool operator>>(const SScene& _rData, GLTFCharValue& _JsonValue)
    {
        if (!g_json_doc_ptr) return false;
        _JsonValue.SetObject();
        {
            const SGlTFChildofRootProperty& super_ptr = _rData;
            if (!(super_ptr >> _JsonValue)) return false;
        }
        if (!_rData.nodes.empty())
        {
            GLTFCharValue json_value;
            if (!(_rData.nodes >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("nodes"), json_value, g_json_doc_ptr->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SScene>& _pData, const GLTFCharValue& _JsonValue)
    {
        std::shared_ptr<SScene> data_ptr = !!_pData ? _pData : std::make_shared<SScene>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const std::shared_ptr<SScene>& _pData, GLTFCharValue& _JsonValue)
    {
        if (!_pData) return false;
        return (*_pData >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SScene>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SScene>>(_vDatas, _JsonValue);
    }

    bool operator>>(const std::vector<std::shared_ptr<SScene>>& _vDatas, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SScene>>(_vDatas, _JsonValue);
    }

    bool operator<<(SCameraPerspective& _rData, const GLTFCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("aspectRatio")) && _JsonValue[GLTFTEXT("aspectRatio")].IsFloat())
        {
            _rData.aspectRatio = _JsonValue[GLTFTEXT("aspectRatio")].GetFloat();
        }
        if (_JsonValue.HasMember(GLTFTEXT("yfov")) && _JsonValue[GLTFTEXT("yfov")].IsFloat())
        {
            _rData.yfov = _JsonValue[GLTFTEXT("yfov")].GetFloat();
        }
        if (_JsonValue.HasMember(GLTFTEXT("znear")) && _JsonValue[GLTFTEXT("znear")].IsFloat())
        {
            _rData.znear = _JsonValue[GLTFTEXT("znear")].GetFloat();
        }
        if (_JsonValue.HasMember(GLTFTEXT("zfar")) && _JsonValue[GLTFTEXT("zfar")].IsFloat())
        {
            _rData.zfar = _JsonValue[GLTFTEXT("zfar")].GetFloat();
        }
        _rData.schemaType = GLTFTEXT("camera.perspective.schema.json");
        return true;
    }

    bool operator>>(const SCameraPerspective& _rData, GLTFCharValue& _JsonValue)
    {
        if (!g_json_doc_ptr) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr >> _JsonValue)) return false;
        }
        {
            GLTFCharValue json_value;
            if (!(_rData.aspectRatio >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("aspectRatio"), json_value, g_json_doc_ptr->GetAllocator());
        }
        {
            GLTFCharValue json_value;
            if (!(_rData.yfov >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("yfov"), json_value, g_json_doc_ptr->GetAllocator());
        }
        {
            GLTFCharValue json_value;
            if (!(_rData.znear >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("znear"), json_value, g_json_doc_ptr->GetAllocator());
        }
        {
            GLTFCharValue json_value;
            if (!(_rData.zfar >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("zfar"), json_value, g_json_doc_ptr->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SCameraPerspective>& _pData, const GLTFCharValue& _JsonValue)
    {
        std::shared_ptr<SCameraPerspective> data_ptr = !!_pData ? _pData : std::make_shared<SCameraPerspective>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const std::shared_ptr<SCameraPerspective>& _pData, GLTFCharValue& _JsonValue)
    {
        if (!_pData) return false;
        return (*_pData >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SCameraPerspective>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SCameraPerspective>>(_vDatas, _JsonValue);
    }

    bool operator>>(const std::vector<std::shared_ptr<SCameraPerspective>>& _vDatas, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SCameraPerspective>>(_vDatas, _JsonValue);
    }

    bool operator<<(SBufferView& _rData, const GLTFCharValue& _JsonValue)
    {
        {
            SGlTFChildofRootProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("byteLength")) && _JsonValue[GLTFTEXT("byteLength")].IsInt())
        {
            _rData.byteLength = _JsonValue[GLTFTEXT("byteLength")].GetInt();
        }
        if (_JsonValue.HasMember(GLTFTEXT("buffer")) && _JsonValue[GLTFTEXT("buffer")].IsInt())
        {
            if (!(_rData.buffer << _JsonValue[GLTFTEXT("buffer")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("byteOffset")) && _JsonValue[GLTFTEXT("byteOffset")].IsInt())
        {
            _rData.byteOffset = _JsonValue[GLTFTEXT("byteOffset")].GetInt();
        }
        if (_JsonValue.HasMember(GLTFTEXT("target")) && _JsonValue[GLTFTEXT("target")].IsInt())
        {
            _rData.target = _JsonValue[GLTFTEXT("target")].GetInt();
        }
        if (_JsonValue.HasMember(GLTFTEXT("byteStride")) && _JsonValue[GLTFTEXT("byteStride")].IsInt())
        {
            _rData.byteStride = _JsonValue[GLTFTEXT("byteStride")].GetInt();
        }
        _rData.schemaType = GLTFTEXT("bufferView.schema.json");
        return true;
    }

    bool operator>>(const SBufferView& _rData, GLTFCharValue& _JsonValue)
    {
        if (!g_json_doc_ptr) return false;
        _JsonValue.SetObject();
        {
            const SGlTFChildofRootProperty& super_ptr = _rData;
            if (!(super_ptr >> _JsonValue)) return false;
        }
        {
            GLTFCharValue json_value;
            if (!(_rData.byteLength >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("byteLength"), json_value, g_json_doc_ptr->GetAllocator());
        }
        if (!!_rData.buffer)
        {
            GLTFCharValue json_value;
            if (!(_rData.buffer >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("buffer"), json_value, g_json_doc_ptr->GetAllocator());
        }
        {
            GLTFCharValue json_value;
            if (!(_rData.byteOffset >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("byteOffset"), json_value, g_json_doc_ptr->GetAllocator());
        }
        {
            GLTFCharValue json_value;
            if (!(_rData.target >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("target"), json_value, g_json_doc_ptr->GetAllocator());
        }
        {
            GLTFCharValue json_value;
            if (!(_rData.byteStride >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("byteStride"), json_value, g_json_doc_ptr->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SBufferView>& _pData, const GLTFCharValue& _JsonValue)
    {
        std::shared_ptr<SBufferView> data_ptr = !!_pData ? _pData : std::make_shared<SBufferView>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const std::shared_ptr<SBufferView>& _pData, GLTFCharValue& _JsonValue)
    {
        if (!_pData) return false;
        return (*_pData >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SBufferView>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SBufferView>>(_vDatas, _JsonValue);
    }

    bool operator>>(const std::vector<std::shared_ptr<SBufferView>>& _vDatas, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SBufferView>>(_vDatas, _JsonValue);
    }

    bool operator<<(SMaterialNormalTextureInfo& _rData, const GLTFCharValue& _JsonValue)
    {
        {
            STextureInfo& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("scale")) && _JsonValue[GLTFTEXT("scale")].IsFloat())
        {
            _rData.scale = _JsonValue[GLTFTEXT("scale")].GetFloat();
        }
        _rData.schemaType = GLTFTEXT("material.normalTextureInfo.schema.json");
        return true;
    }

    bool operator>>(const SMaterialNormalTextureInfo& _rData, GLTFCharValue& _JsonValue)
    {
        if (!g_json_doc_ptr) return false;
        _JsonValue.SetObject();
        {
            const STextureInfo& super_ptr = _rData;
            if (!(super_ptr >> _JsonValue)) return false;
        }
        {
            GLTFCharValue json_value;
            if (!(_rData.scale >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("scale"), json_value, g_json_doc_ptr->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SMaterialNormalTextureInfo>& _pData, const GLTFCharValue& _JsonValue)
    {
        std::shared_ptr<SMaterialNormalTextureInfo> data_ptr = !!_pData ? _pData : std::make_shared<SMaterialNormalTextureInfo>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const std::shared_ptr<SMaterialNormalTextureInfo>& _pData, GLTFCharValue& _JsonValue)
    {
        if (!_pData) return false;
        return (*_pData >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SMaterialNormalTextureInfo>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SMaterialNormalTextureInfo>>(_vDatas, _JsonValue);
    }

    bool operator>>(const std::vector<std::shared_ptr<SMaterialNormalTextureInfo>>& _vDatas, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SMaterialNormalTextureInfo>>(_vDatas, _JsonValue);
    }

    bool operator<<(SMaterialOcclusionTextureInfo& _rData, const GLTFCharValue& _JsonValue)
    {
        {
            STextureInfo& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("strength")) && _JsonValue[GLTFTEXT("strength")].IsFloat())
        {
            _rData.strength = _JsonValue[GLTFTEXT("strength")].GetFloat();
        }
        _rData.schemaType = GLTFTEXT("material.occlusionTextureInfo.schema.json");
        return true;
    }

    bool operator>>(const SMaterialOcclusionTextureInfo& _rData, GLTFCharValue& _JsonValue)
    {
        if (!g_json_doc_ptr) return false;
        _JsonValue.SetObject();
        {
            const STextureInfo& super_ptr = _rData;
            if (!(super_ptr >> _JsonValue)) return false;
        }
        {
            GLTFCharValue json_value;
            if (!(_rData.strength >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("strength"), json_value, g_json_doc_ptr->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SMaterialOcclusionTextureInfo>& _pData, const GLTFCharValue& _JsonValue)
    {
        std::shared_ptr<SMaterialOcclusionTextureInfo> data_ptr = !!_pData ? _pData : std::make_shared<SMaterialOcclusionTextureInfo>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const std::shared_ptr<SMaterialOcclusionTextureInfo>& _pData, GLTFCharValue& _JsonValue)
    {
        if (!_pData) return false;
        return (*_pData >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SMaterialOcclusionTextureInfo>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SMaterialOcclusionTextureInfo>>(_vDatas, _JsonValue);
    }

    bool operator>>(const std::vector<std::shared_ptr<SMaterialOcclusionTextureInfo>>& _vDatas, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SMaterialOcclusionTextureInfo>>(_vDatas, _JsonValue);
    }

    bool operator<<(SAccessorSparseValues& _rData, const GLTFCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("bufferView")) && _JsonValue[GLTFTEXT("bufferView")].IsInt())
        {
            if (!(_rData.bufferView << _JsonValue[GLTFTEXT("bufferView")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("byteOffset")) && _JsonValue[GLTFTEXT("byteOffset")].IsInt())
        {
            _rData.byteOffset = _JsonValue[GLTFTEXT("byteOffset")].GetInt();
        }
        _rData.schemaType = GLTFTEXT("accessor.sparse.values.schema.json");
        return true;
    }

    bool operator>>(const SAccessorSparseValues& _rData, GLTFCharValue& _JsonValue)
    {
        if (!g_json_doc_ptr) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr >> _JsonValue)) return false;
        }
        if (!!_rData.bufferView)
        {
            GLTFCharValue json_value;
            if (!(_rData.bufferView >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("bufferView"), json_value, g_json_doc_ptr->GetAllocator());
        }
        {
            GLTFCharValue json_value;
            if (!(_rData.byteOffset >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("byteOffset"), json_value, g_json_doc_ptr->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SAccessorSparseValues>& _pData, const GLTFCharValue& _JsonValue)
    {
        std::shared_ptr<SAccessorSparseValues> data_ptr = !!_pData ? _pData : std::make_shared<SAccessorSparseValues>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const std::shared_ptr<SAccessorSparseValues>& _pData, GLTFCharValue& _JsonValue)
    {
        if (!_pData) return false;
        return (*_pData >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SAccessorSparseValues>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SAccessorSparseValues>>(_vDatas, _JsonValue);
    }

    bool operator>>(const std::vector<std::shared_ptr<SAccessorSparseValues>>& _vDatas, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SAccessorSparseValues>>(_vDatas, _JsonValue);
    }

    bool operator<<(SAnimationChannelTarget& _rData, const GLTFCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("node")) && _JsonValue[GLTFTEXT("node")].IsInt())
        {
            if (!(_rData.node << _JsonValue[GLTFTEXT("node")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("path")) && _JsonValue[GLTFTEXT("path")].IsString())
        {
            _rData.path = _JsonValue[GLTFTEXT("path")].GetString();
        }
        _rData.schemaType = GLTFTEXT("animation.channel.target.schema.json");
        return true;
    }

    bool operator>>(const SAnimationChannelTarget& _rData, GLTFCharValue& _JsonValue)
    {
        if (!g_json_doc_ptr) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr >> _JsonValue)) return false;
        }
        if (!!_rData.node)
        {
            GLTFCharValue json_value;
            if (!(_rData.node >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("node"), json_value, g_json_doc_ptr->GetAllocator());
        }
        {
            GLTFCharValue json_value;
            if (!(_rData.path >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("path"), json_value, g_json_doc_ptr->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SAnimationChannelTarget>& _pData, const GLTFCharValue& _JsonValue)
    {
        std::shared_ptr<SAnimationChannelTarget> data_ptr = !!_pData ? _pData : std::make_shared<SAnimationChannelTarget>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const std::shared_ptr<SAnimationChannelTarget>& _pData, GLTFCharValue& _JsonValue)
    {
        if (!_pData) return false;
        return (*_pData >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SAnimationChannelTarget>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SAnimationChannelTarget>>(_vDatas, _JsonValue);
    }

    bool operator>>(const std::vector<std::shared_ptr<SAnimationChannelTarget>>& _vDatas, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SAnimationChannelTarget>>(_vDatas, _JsonValue);
    }

    bool operator<<(SMesh& _rData, const GLTFCharValue& _JsonValue)
    {
        {
            SGlTFChildofRootProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("primitives")) && _JsonValue[GLTFTEXT("primitives")].IsArray())
        {
            if (!(_rData.primitives << _JsonValue[GLTFTEXT("primitives")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("weights")) && _JsonValue[GLTFTEXT("weights")].IsArray())
        {
            if (!(_rData.weights << _JsonValue[GLTFTEXT("weights")])) return false;
        }
        _rData.schemaType = GLTFTEXT("mesh.schema.json");
        return true;
    }

    bool operator>>(const SMesh& _rData, GLTFCharValue& _JsonValue)
    {
        if (!g_json_doc_ptr) return false;
        _JsonValue.SetObject();
        {
            const SGlTFChildofRootProperty& super_ptr = _rData;
            if (!(super_ptr >> _JsonValue)) return false;
        }
        if (!_rData.primitives.empty())
        {
            GLTFCharValue json_value;
            if (!(_rData.primitives >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("primitives"), json_value, g_json_doc_ptr->GetAllocator());
        }
        if (!_rData.weights.empty())
        {
            GLTFCharValue json_value;
            if (!(_rData.weights >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("weights"), json_value, g_json_doc_ptr->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SMesh>& _pData, const GLTFCharValue& _JsonValue)
    {
        std::shared_ptr<SMesh> data_ptr = !!_pData ? _pData : std::make_shared<SMesh>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const std::shared_ptr<SMesh>& _pData, GLTFCharValue& _JsonValue)
    {
        if (!_pData) return false;
        return (*_pData >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SMesh>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SMesh>>(_vDatas, _JsonValue);
    }

    bool operator>>(const std::vector<std::shared_ptr<SMesh>>& _vDatas, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SMesh>>(_vDatas, _JsonValue);
    }

    bool operator<<(SAccessorSparse& _rData, const GLTFCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("count")) && _JsonValue[GLTFTEXT("count")].IsInt())
        {
            _rData.count = _JsonValue[GLTFTEXT("count")].GetInt();
        }
        if (_JsonValue.HasMember(GLTFTEXT("indices")) && _JsonValue[GLTFTEXT("indices")].IsObject())
        {
            if (!(_rData.indices << _JsonValue[GLTFTEXT("indices")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("values")) && _JsonValue[GLTFTEXT("values")].IsObject())
        {
            if (!(_rData.values << _JsonValue[GLTFTEXT("values")])) return false;
        }
        _rData.schemaType = GLTFTEXT("accessor.sparse.schema.json");
        return true;
    }

    bool operator>>(const SAccessorSparse& _rData, GLTFCharValue& _JsonValue)
    {
        if (!g_json_doc_ptr) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr >> _JsonValue)) return false;
        }
        {
            GLTFCharValue json_value;
            if (!(_rData.count >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("count"), json_value, g_json_doc_ptr->GetAllocator());
        }
        if (!!_rData.indices)
        {
            GLTFCharValue json_value;
            if (!(_rData.indices >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("indices"), json_value, g_json_doc_ptr->GetAllocator());
        }
        if (!!_rData.values)
        {
            GLTFCharValue json_value;
            if (!(_rData.values >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("values"), json_value, g_json_doc_ptr->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SAccessorSparse>& _pData, const GLTFCharValue& _JsonValue)
    {
        std::shared_ptr<SAccessorSparse> data_ptr = !!_pData ? _pData : std::make_shared<SAccessorSparse>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const std::shared_ptr<SAccessorSparse>& _pData, GLTFCharValue& _JsonValue)
    {
        if (!_pData) return false;
        return (*_pData >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SAccessorSparse>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SAccessorSparse>>(_vDatas, _JsonValue);
    }

    bool operator>>(const std::vector<std::shared_ptr<SAccessorSparse>>& _vDatas, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SAccessorSparse>>(_vDatas, _JsonValue);
    }

    bool operator<<(SMeshPrimitive& _rData, const GLTFCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("indices")) && _JsonValue[GLTFTEXT("indices")].IsInt())
        {
            if (!(_rData.indices << _JsonValue[GLTFTEXT("indices")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("attributes")) && _JsonValue[GLTFTEXT("attributes")].IsObject())
        {
            if (!(_rData.attributes << _JsonValue[GLTFTEXT("attributes")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("material")) && _JsonValue[GLTFTEXT("material")].IsInt())
        {
            if (!(_rData.material << _JsonValue[GLTFTEXT("material")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("mode")) && _JsonValue[GLTFTEXT("mode")].IsInt())
        {
            _rData.mode = _JsonValue[GLTFTEXT("mode")].GetInt();
        }
        if (_JsonValue.HasMember(GLTFTEXT("targets")) && _JsonValue[GLTFTEXT("targets")].IsArray())
        {
            if (!(_rData.targets << _JsonValue[GLTFTEXT("targets")])) return false;
        }
        _rData.schemaType = GLTFTEXT("mesh.primitive.schema.json");
        return true;
    }

    bool operator>>(const SMeshPrimitive& _rData, GLTFCharValue& _JsonValue)
    {
        if (!g_json_doc_ptr) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr >> _JsonValue)) return false;
        }
        if (!!_rData.indices)
        {
            GLTFCharValue json_value;
            if (!(_rData.indices >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("indices"), json_value, g_json_doc_ptr->GetAllocator());
        }
        if (!_rData.attributes.empty())
        {
            GLTFCharValue json_value;
            if (!(_rData.attributes >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("attributes"), json_value, g_json_doc_ptr->GetAllocator());
        }
        if (!!_rData.material)
        {
            GLTFCharValue json_value;
            if (!(_rData.material >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("material"), json_value, g_json_doc_ptr->GetAllocator());
        }
        {
            GLTFCharValue json_value;
            if (!(_rData.mode >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("mode"), json_value, g_json_doc_ptr->GetAllocator());
        }
        if (!_rData.targets.empty())
        {
            GLTFCharValue json_value;
            if (!(_rData.targets >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("targets"), json_value, g_json_doc_ptr->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SMeshPrimitive>& _pData, const GLTFCharValue& _JsonValue)
    {
        std::shared_ptr<SMeshPrimitive> data_ptr = !!_pData ? _pData : std::make_shared<SMeshPrimitive>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const std::shared_ptr<SMeshPrimitive>& _pData, GLTFCharValue& _JsonValue)
    {
        if (!_pData) return false;
        return (*_pData >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SMeshPrimitive>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SMeshPrimitive>>(_vDatas, _JsonValue);
    }

    bool operator>>(const std::vector<std::shared_ptr<SMeshPrimitive>>& _vDatas, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SMeshPrimitive>>(_vDatas, _JsonValue);
    }

    bool operator<<(SKHR_materials_pbrSpecularGlossinessglTFextension& _rData, const GLTFCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("specularFactor")) && _JsonValue[GLTFTEXT("specularFactor")].IsArray())
        {
            if (!(_rData.specularFactor << _JsonValue[GLTFTEXT("specularFactor")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("glossinessFactor")) && _JsonValue[GLTFTEXT("glossinessFactor")].IsFloat())
        {
            _rData.glossinessFactor = _JsonValue[GLTFTEXT("glossinessFactor")].GetFloat();
        }
        if (_JsonValue.HasMember(GLTFTEXT("diffuseTexture")) && _JsonValue[GLTFTEXT("diffuseTexture")].IsObject())
        {
            if (!(_rData.diffuseTexture << _JsonValue[GLTFTEXT("diffuseTexture")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("specularGlossinessTexture")) && _JsonValue[GLTFTEXT("specularGlossinessTexture")].IsObject())
        {
            if (!(_rData.specularGlossinessTexture << _JsonValue[GLTFTEXT("specularGlossinessTexture")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("diffuseFactor")) && _JsonValue[GLTFTEXT("diffuseFactor")].IsArray())
        {
            if (!(_rData.diffuseFactor << _JsonValue[GLTFTEXT("diffuseFactor")])) return false;
        }
        _rData.schemaType = GLTFTEXT("glTF.KHR_materials_pbrSpecularGlossiness.schema.json");
        return true;
    }

    bool operator>>(const SKHR_materials_pbrSpecularGlossinessglTFextension& _rData, GLTFCharValue& _JsonValue)
    {
        if (!g_json_doc_ptr) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr >> _JsonValue)) return false;
        }
        if (!_rData.specularFactor.empty())
        {
            GLTFCharValue json_value;
            if (!(_rData.specularFactor >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("specularFactor"), json_value, g_json_doc_ptr->GetAllocator());
        }
        {
            GLTFCharValue json_value;
            if (!(_rData.glossinessFactor >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("glossinessFactor"), json_value, g_json_doc_ptr->GetAllocator());
        }
        if (!!_rData.diffuseTexture)
        {
            GLTFCharValue json_value;
            if (!(_rData.diffuseTexture >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("diffuseTexture"), json_value, g_json_doc_ptr->GetAllocator());
        }
        if (!!_rData.specularGlossinessTexture)
        {
            GLTFCharValue json_value;
            if (!(_rData.specularGlossinessTexture >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("specularGlossinessTexture"), json_value, g_json_doc_ptr->GetAllocator());
        }
        if (!_rData.diffuseFactor.empty())
        {
            GLTFCharValue json_value;
            if (!(_rData.diffuseFactor >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("diffuseFactor"), json_value, g_json_doc_ptr->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SKHR_materials_pbrSpecularGlossinessglTFextension>& _pData, const GLTFCharValue& _JsonValue)
    {
        std::shared_ptr<SKHR_materials_pbrSpecularGlossinessglTFextension> data_ptr = !!_pData ? _pData : std::make_shared<SKHR_materials_pbrSpecularGlossinessglTFextension>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const std::shared_ptr<SKHR_materials_pbrSpecularGlossinessglTFextension>& _pData, GLTFCharValue& _JsonValue)
    {
        if (!_pData) return false;
        return (*_pData >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SKHR_materials_pbrSpecularGlossinessglTFextension>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SKHR_materials_pbrSpecularGlossinessglTFextension>>(_vDatas, _JsonValue);
    }

    bool operator>>(const std::vector<std::shared_ptr<SKHR_materials_pbrSpecularGlossinessglTFextension>>& _vDatas, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SKHR_materials_pbrSpecularGlossinessglTFextension>>(_vDatas, _JsonValue);
    }

    bool operator<<(SExtension& _rData, const GLTFCharValue& _JsonValue)
    {
        // Manual code lines
        if (_JsonValue.HasMember(GLTFTEXT("KHR_draco_mesh_compression")) && _JsonValue[GLTFTEXT("KHR_draco_mesh_compression")].IsObject())
        {
            std::shared_ptr<SKHR_draco_mesh_compressionextension> extension;
            if (!(extension << _JsonValue[GLTFTEXT("KHR_draco_mesh_compression")])) return false;
            _rData.properties.insert(std::make_pair(GLTFTEXT("KHR_draco_mesh_compression"), extension));
        }
        if (_JsonValue.HasMember(GLTFTEXT("KHR_materials_pbrSpecularGlossiness")) && _JsonValue[GLTFTEXT("KHR_materials_pbrSpecularGlossiness")].IsObject())
        {
            std::shared_ptr<SKHR_materials_pbrSpecularGlossinessglTFextension> extension;
            if (!(extension << _JsonValue[GLTFTEXT("KHR_materials_pbrSpecularGlossiness")])) return false;
            _rData.properties.insert(std::make_pair(GLTFTEXT("KHR_materials_pbrSpecularGlossiness"), extension));
        }
        _rData.schemaType = GLTFTEXT("extension.schema.json");
        return true;
    }

    bool operator>>(const SExtension& _rData, GLTFCharValue& _JsonValue)
    {
        // Manual code lines
        if (!_rData || !g_json_doc_ptr) return false;
        _JsonValue.SetObject();
        for (const std::pair<GLTFString, std::shared_ptr<SObject>>& extensionProperty : _rData.properties)
        {
            const std::shared_ptr<SObject>& extensionValue = extensionProperty.second;
            if (!extensionValue) return false;
            if (extensionValue->schemaType == GLTFTEXT("node.KHR_draco_mesh_compression.schema.json"))
            {
                const SKHR_draco_mesh_compressionextension& extension = *((const SKHR_draco_mesh_compressionextension*)extensionValue.get());
                GLTFCharValue json_value;
                if (!(extension >> json_value)) return false;
                _JsonValue.AddMember(GLTFTEXT("KHR_draco_mesh_compression"), json_value, g_json_doc_ptr->GetAllocator());
            }
            else if (extensionValue->schemaType == GLTFTEXT("glTF.KHR_materials_pbrSpecularGlossiness.schema.json"))
            {
                const SKHR_materials_pbrSpecularGlossinessglTFextension& extension = *((const SKHR_materials_pbrSpecularGlossinessglTFextension*)extensionValue.get());
                GLTFCharValue json_value;
                if (!(extension >> json_value)) return false;
                _JsonValue.AddMember(GLTFTEXT("KHR_materials_pbrSpecularGlossiness"), json_value, g_json_doc_ptr->GetAllocator());
            }
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SExtension>& _pData, const GLTFCharValue& _JsonValue)
    {
        std::shared_ptr<SExtension> data_ptr = !!_pData ? _pData : std::make_shared<SExtension>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const std::shared_ptr<SExtension>& _pData, GLTFCharValue& _JsonValue)
    {
        if (!_pData) return false;
        return (*_pData >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SExtension>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SExtension>>(_vDatas, _JsonValue);
    }

    bool operator>>(const std::vector<std::shared_ptr<SExtension>>& _vDatas, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SExtension>>(_vDatas, _JsonValue);
    }

    bool operator<<(SAnimationChannel& _rData, const GLTFCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("target")) && _JsonValue[GLTFTEXT("target")].IsObject())
        {
            if (!(_rData.target << _JsonValue[GLTFTEXT("target")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("sampler")) && _JsonValue[GLTFTEXT("sampler")].IsInt())
        {
            if (!(_rData.sampler << _JsonValue[GLTFTEXT("sampler")])) return false;
        }
        _rData.schemaType = GLTFTEXT("animation.channel.schema.json");
        return true;
    }

    bool operator>>(const SAnimationChannel& _rData, GLTFCharValue& _JsonValue)
    {
        if (!g_json_doc_ptr) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr >> _JsonValue)) return false;
        }
        if (!!_rData.target)
        {
            GLTFCharValue json_value;
            if (!(_rData.target >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("target"), json_value, g_json_doc_ptr->GetAllocator());
        }
        if (!!_rData.sampler)
        {
            GLTFCharValue json_value;
            if (!(_rData.sampler >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("sampler"), json_value, g_json_doc_ptr->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SAnimationChannel>& _pData, const GLTFCharValue& _JsonValue)
    {
        std::shared_ptr<SAnimationChannel> data_ptr = !!_pData ? _pData : std::make_shared<SAnimationChannel>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const std::shared_ptr<SAnimationChannel>& _pData, GLTFCharValue& _JsonValue)
    {
        if (!_pData) return false;
        return (*_pData >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SAnimationChannel>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SAnimationChannel>>(_vDatas, _JsonValue);
    }

    bool operator>>(const std::vector<std::shared_ptr<SAnimationChannel>>& _vDatas, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SAnimationChannel>>(_vDatas, _JsonValue);
    }

    bool operator<<(SGlTFId& _rData, const GLTFCharValue& _JsonValue)
    {
        if (!(_rData.int32_tValue << _JsonValue)) return false;
        _rData.schemaType = GLTFTEXT("glTFid.schema.json");
        return true;
    }

    bool operator>>(const SGlTFId& _rData, GLTFCharValue& _JsonValue)
    {
        if (!g_json_doc_ptr) return false;
        _JsonValue.SetObject();
        if (!(_rData.int32_tValue >> _JsonValue)) return false;
        return true;
    }

    bool operator<<(std::shared_ptr<SGlTFId>& _pData, const GLTFCharValue& _JsonValue)
    {
        std::shared_ptr<SGlTFId> data_ptr = !!_pData ? _pData : std::make_shared<SGlTFId>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const std::shared_ptr<SGlTFId>& _pData, GLTFCharValue& _JsonValue)
    {
        if (!_pData) return false;
        return (*_pData >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SGlTFId>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SGlTFId>>(_vDatas, _JsonValue);
    }

    bool operator>>(const std::vector<std::shared_ptr<SGlTFId>>& _vDatas, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SGlTFId>>(_vDatas, _JsonValue);
    }

    bool operator<<(SAccessorSparseIndices& _rData, const GLTFCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("componentType")) && _JsonValue[GLTFTEXT("componentType")].IsInt())
        {
            _rData.componentType = _JsonValue[GLTFTEXT("componentType")].GetInt();
        }
        if (_JsonValue.HasMember(GLTFTEXT("bufferView")) && _JsonValue[GLTFTEXT("bufferView")].IsInt())
        {
            if (!(_rData.bufferView << _JsonValue[GLTFTEXT("bufferView")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("byteOffset")) && _JsonValue[GLTFTEXT("byteOffset")].IsInt())
        {
            _rData.byteOffset = _JsonValue[GLTFTEXT("byteOffset")].GetInt();
        }
        _rData.schemaType = GLTFTEXT("accessor.sparse.indices.schema.json");
        return true;
    }

    bool operator>>(const SAccessorSparseIndices& _rData, GLTFCharValue& _JsonValue)
    {
        if (!g_json_doc_ptr) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr >> _JsonValue)) return false;
        }
        {
            GLTFCharValue json_value;
            if (!(_rData.componentType >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("componentType"), json_value, g_json_doc_ptr->GetAllocator());
        }
        if (!!_rData.bufferView)
        {
            GLTFCharValue json_value;
            if (!(_rData.bufferView >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("bufferView"), json_value, g_json_doc_ptr->GetAllocator());
        }
        {
            GLTFCharValue json_value;
            if (!(_rData.byteOffset >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("byteOffset"), json_value, g_json_doc_ptr->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SAccessorSparseIndices>& _pData, const GLTFCharValue& _JsonValue)
    {
        std::shared_ptr<SAccessorSparseIndices> data_ptr = !!_pData ? _pData : std::make_shared<SAccessorSparseIndices>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const std::shared_ptr<SAccessorSparseIndices>& _pData, GLTFCharValue& _JsonValue)
    {
        if (!_pData) return false;
        return (*_pData >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SAccessorSparseIndices>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SAccessorSparseIndices>>(_vDatas, _JsonValue);
    }

    bool operator>>(const std::vector<std::shared_ptr<SAccessorSparseIndices>>& _vDatas, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SAccessorSparseIndices>>(_vDatas, _JsonValue);
    }

    bool operator<<(SNode& _rData, const GLTFCharValue& _JsonValue)
    {
        {
            SGlTFChildofRootProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("scale")) && _JsonValue[GLTFTEXT("scale")].IsArray())
        {
            if (!(_rData.scale << _JsonValue[GLTFTEXT("scale")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("rotation")) && _JsonValue[GLTFTEXT("rotation")].IsArray())
        {
            if (!(_rData.rotation << _JsonValue[GLTFTEXT("rotation")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("matrix")) && _JsonValue[GLTFTEXT("matrix")].IsArray())
        {
            if (!(_rData.matrix << _JsonValue[GLTFTEXT("matrix")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("mesh")) && _JsonValue[GLTFTEXT("mesh")].IsInt())
        {
            if (!(_rData.mesh << _JsonValue[GLTFTEXT("mesh")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("camera")) && _JsonValue[GLTFTEXT("camera")].IsInt())
        {
            if (!(_rData.camera << _JsonValue[GLTFTEXT("camera")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("weights")) && _JsonValue[GLTFTEXT("weights")].IsArray())
        {
            if (!(_rData.weights << _JsonValue[GLTFTEXT("weights")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("skin")) && _JsonValue[GLTFTEXT("skin")].IsInt())
        {
            if (!(_rData.skin << _JsonValue[GLTFTEXT("skin")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("translation")) && _JsonValue[GLTFTEXT("translation")].IsArray())
        {
            if (!(_rData.translation << _JsonValue[GLTFTEXT("translation")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("children")) && _JsonValue[GLTFTEXT("children")].IsArray())
        {
            if (!(_rData.children << _JsonValue[GLTFTEXT("children")])) return false;
        }
        _rData.schemaType = GLTFTEXT("node.schema.json");
        return true;
    }

    bool operator>>(const SNode& _rData, GLTFCharValue& _JsonValue)
    {
        if (!g_json_doc_ptr) return false;
        _JsonValue.SetObject();
        {
            const SGlTFChildofRootProperty& super_ptr = _rData;
            if (!(super_ptr >> _JsonValue)) return false;
        }
        if (!_rData.scale.empty())
        {
            GLTFCharValue json_value;
            if (!(_rData.scale >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("scale"), json_value, g_json_doc_ptr->GetAllocator());
        }
        if (!_rData.rotation.empty())
        {
            GLTFCharValue json_value;
            if (!(_rData.rotation >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("rotation"), json_value, g_json_doc_ptr->GetAllocator());
        }
        if (!_rData.matrix.empty())
        {
            GLTFCharValue json_value;
            if (!(_rData.matrix >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("matrix"), json_value, g_json_doc_ptr->GetAllocator());
        }
        if (!!_rData.mesh)
        {
            GLTFCharValue json_value;
            if (!(_rData.mesh >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("mesh"), json_value, g_json_doc_ptr->GetAllocator());
        }
        if (!!_rData.camera)
        {
            GLTFCharValue json_value;
            if (!(_rData.camera >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("camera"), json_value, g_json_doc_ptr->GetAllocator());
        }
        if (!_rData.weights.empty())
        {
            GLTFCharValue json_value;
            if (!(_rData.weights >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("weights"), json_value, g_json_doc_ptr->GetAllocator());
        }
        if (!!_rData.skin)
        {
            GLTFCharValue json_value;
            if (!(_rData.skin >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("skin"), json_value, g_json_doc_ptr->GetAllocator());
        }
        if (!_rData.translation.empty())
        {
            GLTFCharValue json_value;
            if (!(_rData.translation >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("translation"), json_value, g_json_doc_ptr->GetAllocator());
        }
        if (!_rData.children.empty())
        {
            GLTFCharValue json_value;
            if (!(_rData.children >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("children"), json_value, g_json_doc_ptr->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SNode>& _pData, const GLTFCharValue& _JsonValue)
    {
        std::shared_ptr<SNode> data_ptr = !!_pData ? _pData : std::make_shared<SNode>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const std::shared_ptr<SNode>& _pData, GLTFCharValue& _JsonValue)
    {
        if (!_pData) return false;
        return (*_pData >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SNode>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SNode>>(_vDatas, _JsonValue);
    }

    bool operator>>(const std::vector<std::shared_ptr<SNode>>& _vDatas, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SNode>>(_vDatas, _JsonValue);
    }

    bool operator<<(SAnimation& _rData, const GLTFCharValue& _JsonValue)
    {
        {
            SGlTFChildofRootProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("channels")) && _JsonValue[GLTFTEXT("channels")].IsArray())
        {
            if (!(_rData.channels << _JsonValue[GLTFTEXT("channels")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("samplers")) && _JsonValue[GLTFTEXT("samplers")].IsArray())
        {
            if (!(_rData.samplers << _JsonValue[GLTFTEXT("samplers")])) return false;
        }
        _rData.schemaType = GLTFTEXT("animation.schema.json");
        return true;
    }

    bool operator>>(const SAnimation& _rData, GLTFCharValue& _JsonValue)
    {
        if (!g_json_doc_ptr) return false;
        _JsonValue.SetObject();
        {
            const SGlTFChildofRootProperty& super_ptr = _rData;
            if (!(super_ptr >> _JsonValue)) return false;
        }
        if (!_rData.channels.empty())
        {
            GLTFCharValue json_value;
            if (!(_rData.channels >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("channels"), json_value, g_json_doc_ptr->GetAllocator());
        }
        if (!_rData.samplers.empty())
        {
            GLTFCharValue json_value;
            if (!(_rData.samplers >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("samplers"), json_value, g_json_doc_ptr->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SAnimation>& _pData, const GLTFCharValue& _JsonValue)
    {
        std::shared_ptr<SAnimation> data_ptr = !!_pData ? _pData : std::make_shared<SAnimation>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const std::shared_ptr<SAnimation>& _pData, GLTFCharValue& _JsonValue)
    {
        if (!_pData) return false;
        return (*_pData >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SAnimation>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SAnimation>>(_vDatas, _JsonValue);
    }

    bool operator>>(const std::vector<std::shared_ptr<SAnimation>>& _vDatas, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SAnimation>>(_vDatas, _JsonValue);
    }

    bool operator<<(SSkin& _rData, const GLTFCharValue& _JsonValue)
    {
        {
            SGlTFChildofRootProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("joints")) && _JsonValue[GLTFTEXT("joints")].IsArray())
        {
            if (!(_rData.joints << _JsonValue[GLTFTEXT("joints")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("inverseBindMatrices")) && _JsonValue[GLTFTEXT("inverseBindMatrices")].IsInt())
        {
            if (!(_rData.inverseBindMatrices << _JsonValue[GLTFTEXT("inverseBindMatrices")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("skeleton")) && _JsonValue[GLTFTEXT("skeleton")].IsInt())
        {
            if (!(_rData.skeleton << _JsonValue[GLTFTEXT("skeleton")])) return false;
        }
        _rData.schemaType = GLTFTEXT("skin.schema.json");
        return true;
    }

    bool operator>>(const SSkin& _rData, GLTFCharValue& _JsonValue)
    {
        if (!g_json_doc_ptr) return false;
        _JsonValue.SetObject();
        {
            const SGlTFChildofRootProperty& super_ptr = _rData;
            if (!(super_ptr >> _JsonValue)) return false;
        }
        if (!_rData.joints.empty())
        {
            GLTFCharValue json_value;
            if (!(_rData.joints >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("joints"), json_value, g_json_doc_ptr->GetAllocator());
        }
        if (!!_rData.inverseBindMatrices)
        {
            GLTFCharValue json_value;
            if (!(_rData.inverseBindMatrices >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("inverseBindMatrices"), json_value, g_json_doc_ptr->GetAllocator());
        }
        if (!!_rData.skeleton)
        {
            GLTFCharValue json_value;
            if (!(_rData.skeleton >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("skeleton"), json_value, g_json_doc_ptr->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SSkin>& _pData, const GLTFCharValue& _JsonValue)
    {
        std::shared_ptr<SSkin> data_ptr = !!_pData ? _pData : std::make_shared<SSkin>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const std::shared_ptr<SSkin>& _pData, GLTFCharValue& _JsonValue)
    {
        if (!_pData) return false;
        return (*_pData >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SSkin>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SSkin>>(_vDatas, _JsonValue);
    }

    bool operator>>(const std::vector<std::shared_ptr<SSkin>>& _vDatas, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SSkin>>(_vDatas, _JsonValue);
    }

    bool operator<<(SMaterialPBRMetallicRoughness& _rData, const GLTFCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("roughnessFactor")) && _JsonValue[GLTFTEXT("roughnessFactor")].IsFloat())
        {
            _rData.roughnessFactor = _JsonValue[GLTFTEXT("roughnessFactor")].GetFloat();
        }
        if (_JsonValue.HasMember(GLTFTEXT("baseColorTexture")) && _JsonValue[GLTFTEXT("baseColorTexture")].IsObject())
        {
            if (!(_rData.baseColorTexture << _JsonValue[GLTFTEXT("baseColorTexture")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("metallicFactor")) && _JsonValue[GLTFTEXT("metallicFactor")].IsFloat())
        {
            _rData.metallicFactor = _JsonValue[GLTFTEXT("metallicFactor")].GetFloat();
        }
        if (_JsonValue.HasMember(GLTFTEXT("baseColorFactor")) && _JsonValue[GLTFTEXT("baseColorFactor")].IsArray())
        {
            if (!(_rData.baseColorFactor << _JsonValue[GLTFTEXT("baseColorFactor")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("metallicRoughnessTexture")) && _JsonValue[GLTFTEXT("metallicRoughnessTexture")].IsObject())
        {
            if (!(_rData.metallicRoughnessTexture << _JsonValue[GLTFTEXT("metallicRoughnessTexture")])) return false;
        }
        _rData.schemaType = GLTFTEXT("material.pbrMetallicRoughness.schema.json");
        return true;
    }

    bool operator>>(const SMaterialPBRMetallicRoughness& _rData, GLTFCharValue& _JsonValue)
    {
        if (!g_json_doc_ptr) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr >> _JsonValue)) return false;
        }
        {
            GLTFCharValue json_value;
            if (!(_rData.roughnessFactor >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("roughnessFactor"), json_value, g_json_doc_ptr->GetAllocator());
        }
        if (!!_rData.baseColorTexture)
        {
            GLTFCharValue json_value;
            if (!(_rData.baseColorTexture >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("baseColorTexture"), json_value, g_json_doc_ptr->GetAllocator());
        }
        {
            GLTFCharValue json_value;
            if (!(_rData.metallicFactor >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("metallicFactor"), json_value, g_json_doc_ptr->GetAllocator());
        }
        if (!_rData.baseColorFactor.empty())
        {
            GLTFCharValue json_value;
            if (!(_rData.baseColorFactor >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("baseColorFactor"), json_value, g_json_doc_ptr->GetAllocator());
        }
        if (!!_rData.metallicRoughnessTexture)
        {
            GLTFCharValue json_value;
            if (!(_rData.metallicRoughnessTexture >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("metallicRoughnessTexture"), json_value, g_json_doc_ptr->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SMaterialPBRMetallicRoughness>& _pData, const GLTFCharValue& _JsonValue)
    {
        std::shared_ptr<SMaterialPBRMetallicRoughness> data_ptr = !!_pData ? _pData : std::make_shared<SMaterialPBRMetallicRoughness>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const std::shared_ptr<SMaterialPBRMetallicRoughness>& _pData, GLTFCharValue& _JsonValue)
    {
        if (!_pData) return false;
        return (*_pData >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SMaterialPBRMetallicRoughness>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SMaterialPBRMetallicRoughness>>(_vDatas, _JsonValue);
    }

    bool operator>>(const std::vector<std::shared_ptr<SMaterialPBRMetallicRoughness>>& _vDatas, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SMaterialPBRMetallicRoughness>>(_vDatas, _JsonValue);
    }

    bool operator<<(SKHR_draco_mesh_compressionextension& _rData, const GLTFCharValue& _JsonValue)
    {
        if (_JsonValue.HasMember(GLTFTEXT("attributes")) && _JsonValue[GLTFTEXT("attributes")].IsObject())
        {
            if (!(_rData.attributes << _JsonValue[GLTFTEXT("attributes")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("bufferView")) && _JsonValue[GLTFTEXT("bufferView")].IsInt())
        {
            if (!(_rData.bufferView << _JsonValue[GLTFTEXT("bufferView")])) return false;
        }
        _rData.schemaType = GLTFTEXT("node.KHR_draco_mesh_compression.schema.json");
        return true;
    }

    bool operator>>(const SKHR_draco_mesh_compressionextension& _rData, GLTFCharValue& _JsonValue)
    {
        if (!g_json_doc_ptr) return false;
        _JsonValue.SetObject();
        if (!_rData.attributes.empty())
        {
            GLTFCharValue json_value;
            if (!(_rData.attributes >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("attributes"), json_value, g_json_doc_ptr->GetAllocator());
        }
        if (!!_rData.bufferView)
        {
            GLTFCharValue json_value;
            if (!(_rData.bufferView >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("bufferView"), json_value, g_json_doc_ptr->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SKHR_draco_mesh_compressionextension>& _pData, const GLTFCharValue& _JsonValue)
    {
        std::shared_ptr<SKHR_draco_mesh_compressionextension> data_ptr = !!_pData ? _pData : std::make_shared<SKHR_draco_mesh_compressionextension>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const std::shared_ptr<SKHR_draco_mesh_compressionextension>& _pData, GLTFCharValue& _JsonValue)
    {
        if (!_pData) return false;
        return (*_pData >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SKHR_draco_mesh_compressionextension>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SKHR_draco_mesh_compressionextension>>(_vDatas, _JsonValue);
    }

    bool operator>>(const std::vector<std::shared_ptr<SKHR_draco_mesh_compressionextension>>& _vDatas, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SKHR_draco_mesh_compressionextension>>(_vDatas, _JsonValue);
    }

    bool operator<<(SCamera& _rData, const GLTFCharValue& _JsonValue)
    {
        {
            SGlTFChildofRootProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("type")) && _JsonValue[GLTFTEXT("type")].IsString())
        {
            _rData.type = _JsonValue[GLTFTEXT("type")].GetString();
        }
        if (_JsonValue.HasMember(GLTFTEXT("perspective")) && _JsonValue[GLTFTEXT("perspective")].IsObject())
        {
            if (!(_rData.perspective << _JsonValue[GLTFTEXT("perspective")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("orthographic")) && _JsonValue[GLTFTEXT("orthographic")].IsObject())
        {
            if (!(_rData.orthographic << _JsonValue[GLTFTEXT("orthographic")])) return false;
        }
        _rData.schemaType = GLTFTEXT("camera.schema.json");
        return true;
    }

    bool operator>>(const SCamera& _rData, GLTFCharValue& _JsonValue)
    {
        if (!g_json_doc_ptr) return false;
        _JsonValue.SetObject();
        {
            const SGlTFChildofRootProperty& super_ptr = _rData;
            if (!(super_ptr >> _JsonValue)) return false;
        }
        {
            GLTFCharValue json_value;
            if (!(_rData.type >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("type"), json_value, g_json_doc_ptr->GetAllocator());
        }
        if (!!_rData.perspective)
        {
            GLTFCharValue json_value;
            if (!(_rData.perspective >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("perspective"), json_value, g_json_doc_ptr->GetAllocator());
        }
        if (!!_rData.orthographic)
        {
            GLTFCharValue json_value;
            if (!(_rData.orthographic >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("orthographic"), json_value, g_json_doc_ptr->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SCamera>& _pData, const GLTFCharValue& _JsonValue)
    {
        std::shared_ptr<SCamera> data_ptr = !!_pData ? _pData : std::make_shared<SCamera>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const std::shared_ptr<SCamera>& _pData, GLTFCharValue& _JsonValue)
    {
        if (!_pData) return false;
        return (*_pData >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SCamera>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SCamera>>(_vDatas, _JsonValue);
    }

    bool operator>>(const std::vector<std::shared_ptr<SCamera>>& _vDatas, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SCamera>>(_vDatas, _JsonValue);
    }

    bool operator<<(SImage& _rData, const GLTFCharValue& _JsonValue)
    {
        {
            SGlTFChildofRootProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("mimeType")) && _JsonValue[GLTFTEXT("mimeType")].IsString())
        {
            _rData.mimeType = _JsonValue[GLTFTEXT("mimeType")].GetString();
        }
        if (_JsonValue.HasMember(GLTFTEXT("bufferView")) && _JsonValue[GLTFTEXT("bufferView")].IsInt())
        {
            if (!(_rData.bufferView << _JsonValue[GLTFTEXT("bufferView")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("uri")) && _JsonValue[GLTFTEXT("uri")].IsString())
        {
            _rData.uri = _JsonValue[GLTFTEXT("uri")].GetString();
        }
        _rData.schemaType = GLTFTEXT("image.schema.json");
        return true;
    }

    bool operator>>(const SImage& _rData, GLTFCharValue& _JsonValue)
    {
        if (!g_json_doc_ptr) return false;
        _JsonValue.SetObject();
        {
            const SGlTFChildofRootProperty& super_ptr = _rData;
            if (!(super_ptr >> _JsonValue)) return false;
        }
        {
            GLTFCharValue json_value;
            if (!(_rData.mimeType >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("mimeType"), json_value, g_json_doc_ptr->GetAllocator());
        }
        if (!!_rData.bufferView)
        {
            GLTFCharValue json_value;
            if (!(_rData.bufferView >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("bufferView"), json_value, g_json_doc_ptr->GetAllocator());
        }
        {
            GLTFCharValue json_value;
            if (!(_rData.uri >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("uri"), json_value, g_json_doc_ptr->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SImage>& _pData, const GLTFCharValue& _JsonValue)
    {
        std::shared_ptr<SImage> data_ptr = !!_pData ? _pData : std::make_shared<SImage>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const std::shared_ptr<SImage>& _pData, GLTFCharValue& _JsonValue)
    {
        if (!_pData) return false;
        return (*_pData >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SImage>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SImage>>(_vDatas, _JsonValue);
    }

    bool operator>>(const std::vector<std::shared_ptr<SImage>>& _vDatas, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SImage>>(_vDatas, _JsonValue);
    }

    bool operator<<(STexture& _rData, const GLTFCharValue& _JsonValue)
    {
        {
            SGlTFChildofRootProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("source")) && _JsonValue[GLTFTEXT("source")].IsInt())
        {
            if (!(_rData.source << _JsonValue[GLTFTEXT("source")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("sampler")) && _JsonValue[GLTFTEXT("sampler")].IsInt())
        {
            if (!(_rData.sampler << _JsonValue[GLTFTEXT("sampler")])) return false;
        }
        _rData.schemaType = GLTFTEXT("texture.schema.json");
        return true;
    }

    bool operator>>(const STexture& _rData, GLTFCharValue& _JsonValue)
    {
        if (!g_json_doc_ptr) return false;
        _JsonValue.SetObject();
        {
            const SGlTFChildofRootProperty& super_ptr = _rData;
            if (!(super_ptr >> _JsonValue)) return false;
        }
        if (!!_rData.source)
        {
            GLTFCharValue json_value;
            if (!(_rData.source >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("source"), json_value, g_json_doc_ptr->GetAllocator());
        }
        if (!!_rData.sampler)
        {
            GLTFCharValue json_value;
            if (!(_rData.sampler >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("sampler"), json_value, g_json_doc_ptr->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<STexture>& _pData, const GLTFCharValue& _JsonValue)
    {
        std::shared_ptr<STexture> data_ptr = !!_pData ? _pData : std::make_shared<STexture>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const std::shared_ptr<STexture>& _pData, GLTFCharValue& _JsonValue)
    {
        if (!_pData) return false;
        return (*_pData >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<STexture>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<STexture>>(_vDatas, _JsonValue);
    }

    bool operator>>(const std::vector<std::shared_ptr<STexture>>& _vDatas, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<STexture>>(_vDatas, _JsonValue);
    }

    bool operator<<(SCameraOrthographic& _rData, const GLTFCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("xmag")) && _JsonValue[GLTFTEXT("xmag")].IsFloat())
        {
            _rData.xmag = _JsonValue[GLTFTEXT("xmag")].GetFloat();
        }
        if (_JsonValue.HasMember(GLTFTEXT("ymag")) && _JsonValue[GLTFTEXT("ymag")].IsFloat())
        {
            _rData.ymag = _JsonValue[GLTFTEXT("ymag")].GetFloat();
        }
        if (_JsonValue.HasMember(GLTFTEXT("zfar")) && _JsonValue[GLTFTEXT("zfar")].IsFloat())
        {
            _rData.zfar = _JsonValue[GLTFTEXT("zfar")].GetFloat();
        }
        if (_JsonValue.HasMember(GLTFTEXT("znear")) && _JsonValue[GLTFTEXT("znear")].IsFloat())
        {
            _rData.znear = _JsonValue[GLTFTEXT("znear")].GetFloat();
        }
        _rData.schemaType = GLTFTEXT("camera.orthographic.schema.json");
        return true;
    }

    bool operator>>(const SCameraOrthographic& _rData, GLTFCharValue& _JsonValue)
    {
        if (!g_json_doc_ptr) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr >> _JsonValue)) return false;
        }
        {
            GLTFCharValue json_value;
            if (!(_rData.xmag >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("xmag"), json_value, g_json_doc_ptr->GetAllocator());
        }
        {
            GLTFCharValue json_value;
            if (!(_rData.ymag >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("ymag"), json_value, g_json_doc_ptr->GetAllocator());
        }
        {
            GLTFCharValue json_value;
            if (!(_rData.zfar >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("zfar"), json_value, g_json_doc_ptr->GetAllocator());
        }
        {
            GLTFCharValue json_value;
            if (!(_rData.znear >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("znear"), json_value, g_json_doc_ptr->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SCameraOrthographic>& _pData, const GLTFCharValue& _JsonValue)
    {
        std::shared_ptr<SCameraOrthographic> data_ptr = !!_pData ? _pData : std::make_shared<SCameraOrthographic>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const std::shared_ptr<SCameraOrthographic>& _pData, GLTFCharValue& _JsonValue)
    {
        if (!_pData) return false;
        return (*_pData >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SCameraOrthographic>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SCameraOrthographic>>(_vDatas, _JsonValue);
    }

    bool operator>>(const std::vector<std::shared_ptr<SCameraOrthographic>>& _vDatas, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SCameraOrthographic>>(_vDatas, _JsonValue);
    }

    bool operator<<(SBuffer& _rData, const GLTFCharValue& _JsonValue)
    {
        {
            SGlTFChildofRootProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("byteLength")) && _JsonValue[GLTFTEXT("byteLength")].IsInt())
        {
            _rData.byteLength = _JsonValue[GLTFTEXT("byteLength")].GetInt();
        }
        if (_JsonValue.HasMember(GLTFTEXT("uri")) && _JsonValue[GLTFTEXT("uri")].IsString())
        {
            _rData.uri = _JsonValue[GLTFTEXT("uri")].GetString();
        }
        _rData.schemaType = GLTFTEXT("buffer.schema.json");
        return true;
    }

    bool operator>>(const SBuffer& _rData, GLTFCharValue& _JsonValue)
    {
        if (!g_json_doc_ptr) return false;
        _JsonValue.SetObject();
        {
            const SGlTFChildofRootProperty& super_ptr = _rData;
            if (!(super_ptr >> _JsonValue)) return false;
        }
        {
            GLTFCharValue json_value;
            if (!(_rData.byteLength >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("byteLength"), json_value, g_json_doc_ptr->GetAllocator());
        }
        {
            GLTFCharValue json_value;
            if (!(_rData.uri >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("uri"), json_value, g_json_doc_ptr->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SBuffer>& _pData, const GLTFCharValue& _JsonValue)
    {
        std::shared_ptr<SBuffer> data_ptr = !!_pData ? _pData : std::make_shared<SBuffer>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const std::shared_ptr<SBuffer>& _pData, GLTFCharValue& _JsonValue)
    {
        if (!_pData) return false;
        return (*_pData >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SBuffer>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SBuffer>>(_vDatas, _JsonValue);
    }

    bool operator>>(const std::vector<std::shared_ptr<SBuffer>>& _vDatas, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SBuffer>>(_vDatas, _JsonValue);
    }

    bool operator<<(SAccessor& _rData, const GLTFCharValue& _JsonValue)
    {
        {
            SGlTFChildofRootProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("count")) && _JsonValue[GLTFTEXT("count")].IsInt())
        {
            _rData.count = _JsonValue[GLTFTEXT("count")].GetInt();
        }
        if (_JsonValue.HasMember(GLTFTEXT("min")) && _JsonValue[GLTFTEXT("min")].IsArray())
        {
            if (!(_rData.min << _JsonValue[GLTFTEXT("min")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("max")) && _JsonValue[GLTFTEXT("max")].IsArray())
        {
            if (!(_rData.max << _JsonValue[GLTFTEXT("max")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("bufferView")) && _JsonValue[GLTFTEXT("bufferView")].IsInt())
        {
            if (!(_rData.bufferView << _JsonValue[GLTFTEXT("bufferView")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("componentType")) && _JsonValue[GLTFTEXT("componentType")].IsInt())
        {
            _rData.componentType = _JsonValue[GLTFTEXT("componentType")].GetInt();
        }
        if (_JsonValue.HasMember(GLTFTEXT("byteOffset")) && _JsonValue[GLTFTEXT("byteOffset")].IsInt())
        {
            _rData.byteOffset = _JsonValue[GLTFTEXT("byteOffset")].GetInt();
        }
        if (_JsonValue.HasMember(GLTFTEXT("sparse")) && _JsonValue[GLTFTEXT("sparse")].IsObject())
        {
            if (!(_rData.sparse << _JsonValue[GLTFTEXT("sparse")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("type")) && _JsonValue[GLTFTEXT("type")].IsString())
        {
            _rData.type = _JsonValue[GLTFTEXT("type")].GetString();
        }
        if (_JsonValue.HasMember(GLTFTEXT("normalized")) && _JsonValue[GLTFTEXT("normalized")].IsBool())
        {
            _rData.normalized = _JsonValue[GLTFTEXT("normalized")].GetBool();
        }
        _rData.schemaType = GLTFTEXT("accessor.schema.json");
        return true;
    }

    bool operator>>(const SAccessor& _rData, GLTFCharValue& _JsonValue)
    {
        if (!g_json_doc_ptr) return false;
        _JsonValue.SetObject();
        {
            const SGlTFChildofRootProperty& super_ptr = _rData;
            if (!(super_ptr >> _JsonValue)) return false;
        }
        {
            GLTFCharValue json_value;
            if (!(_rData.count >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("count"), json_value, g_json_doc_ptr->GetAllocator());
        }
        if (!_rData.min.empty())
        {
            GLTFCharValue json_value;
            if (!(_rData.min >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("min"), json_value, g_json_doc_ptr->GetAllocator());
        }
        if (!_rData.max.empty())
        {
            GLTFCharValue json_value;
            if (!(_rData.max >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("max"), json_value, g_json_doc_ptr->GetAllocator());
        }
        if (!!_rData.bufferView)
        {
            GLTFCharValue json_value;
            if (!(_rData.bufferView >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("bufferView"), json_value, g_json_doc_ptr->GetAllocator());
        }
        {
            GLTFCharValue json_value;
            if (!(_rData.componentType >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("componentType"), json_value, g_json_doc_ptr->GetAllocator());
        }
        {
            GLTFCharValue json_value;
            if (!(_rData.byteOffset >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("byteOffset"), json_value, g_json_doc_ptr->GetAllocator());
        }
        if (!!_rData.sparse)
        {
            GLTFCharValue json_value;
            if (!(_rData.sparse >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("sparse"), json_value, g_json_doc_ptr->GetAllocator());
        }
        {
            GLTFCharValue json_value;
            if (!(_rData.type >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("type"), json_value, g_json_doc_ptr->GetAllocator());
        }
        {
            GLTFCharValue json_value;
            if (!(_rData.normalized >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("normalized"), json_value, g_json_doc_ptr->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SAccessor>& _pData, const GLTFCharValue& _JsonValue)
    {
        std::shared_ptr<SAccessor> data_ptr = !!_pData ? _pData : std::make_shared<SAccessor>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const std::shared_ptr<SAccessor>& _pData, GLTFCharValue& _JsonValue)
    {
        if (!_pData) return false;
        return (*_pData >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SAccessor>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SAccessor>>(_vDatas, _JsonValue);
    }

    bool operator>>(const std::vector<std::shared_ptr<SAccessor>>& _vDatas, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SAccessor>>(_vDatas, _JsonValue);
    }

    bool operator<<(SGlTF& _rData, const GLTFCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("textures")) && _JsonValue[GLTFTEXT("textures")].IsArray())
        {
            if (!(_rData.textures << _JsonValue[GLTFTEXT("textures")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("cameras")) && _JsonValue[GLTFTEXT("cameras")].IsArray())
        {
            if (!(_rData.cameras << _JsonValue[GLTFTEXT("cameras")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("accessors")) && _JsonValue[GLTFTEXT("accessors")].IsArray())
        {
            if (!(_rData.accessors << _JsonValue[GLTFTEXT("accessors")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("extensionsUsed")) && _JsonValue[GLTFTEXT("extensionsUsed")].IsArray())
        {
            if (!(_rData.extensionsUsed << _JsonValue[GLTFTEXT("extensionsUsed")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("samplers")) && _JsonValue[GLTFTEXT("samplers")].IsArray())
        {
            if (!(_rData.samplers << _JsonValue[GLTFTEXT("samplers")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("scenes")) && _JsonValue[GLTFTEXT("scenes")].IsArray())
        {
            if (!(_rData.scenes << _JsonValue[GLTFTEXT("scenes")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("scene")) && _JsonValue[GLTFTEXT("scene")].IsInt())
        {
            if (!(_rData.scene << _JsonValue[GLTFTEXT("scene")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("extensionsRequired")) && _JsonValue[GLTFTEXT("extensionsRequired")].IsArray())
        {
            if (!(_rData.extensionsRequired << _JsonValue[GLTFTEXT("extensionsRequired")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("meshes")) && _JsonValue[GLTFTEXT("meshes")].IsArray())
        {
            if (!(_rData.meshes << _JsonValue[GLTFTEXT("meshes")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("animations")) && _JsonValue[GLTFTEXT("animations")].IsArray())
        {
            if (!(_rData.animations << _JsonValue[GLTFTEXT("animations")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("images")) && _JsonValue[GLTFTEXT("images")].IsArray())
        {
            if (!(_rData.images << _JsonValue[GLTFTEXT("images")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("nodes")) && _JsonValue[GLTFTEXT("nodes")].IsArray())
        {
            if (!(_rData.nodes << _JsonValue[GLTFTEXT("nodes")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("bufferViews")) && _JsonValue[GLTFTEXT("bufferViews")].IsArray())
        {
            if (!(_rData.bufferViews << _JsonValue[GLTFTEXT("bufferViews")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("skins")) && _JsonValue[GLTFTEXT("skins")].IsArray())
        {
            if (!(_rData.skins << _JsonValue[GLTFTEXT("skins")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("materials")) && _JsonValue[GLTFTEXT("materials")].IsArray())
        {
            if (!(_rData.materials << _JsonValue[GLTFTEXT("materials")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("buffers")) && _JsonValue[GLTFTEXT("buffers")].IsArray())
        {
            if (!(_rData.buffers << _JsonValue[GLTFTEXT("buffers")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("asset")) && _JsonValue[GLTFTEXT("asset")].IsObject())
        {
            if (!(_rData.asset << _JsonValue[GLTFTEXT("asset")])) return false;
        }
        _rData.schemaType = GLTFTEXT("glTF.schema.json");
        return true;
    }

    bool operator>>(const SGlTF& _rData, GLTFCharValue& _JsonValue)
    {
        if (!g_json_doc_ptr) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr >> _JsonValue)) return false;
        }
        if (!_rData.textures.empty())
        {
            GLTFCharValue json_value;
            if (!(_rData.textures >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("textures"), json_value, g_json_doc_ptr->GetAllocator());
        }
        if (!_rData.cameras.empty())
        {
            GLTFCharValue json_value;
            if (!(_rData.cameras >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("cameras"), json_value, g_json_doc_ptr->GetAllocator());
        }
        if (!_rData.accessors.empty())
        {
            GLTFCharValue json_value;
            if (!(_rData.accessors >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("accessors"), json_value, g_json_doc_ptr->GetAllocator());
        }
        if (!_rData.extensionsUsed.empty())
        {
            GLTFCharValue json_value;
            if (!(_rData.extensionsUsed >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("extensionsUsed"), json_value, g_json_doc_ptr->GetAllocator());
        }
        if (!_rData.samplers.empty())
        {
            GLTFCharValue json_value;
            if (!(_rData.samplers >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("samplers"), json_value, g_json_doc_ptr->GetAllocator());
        }
        if (!_rData.scenes.empty())
        {
            GLTFCharValue json_value;
            if (!(_rData.scenes >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("scenes"), json_value, g_json_doc_ptr->GetAllocator());
        }
        if (!!_rData.scene)
        {
            GLTFCharValue json_value;
            if (!(_rData.scene >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("scene"), json_value, g_json_doc_ptr->GetAllocator());
        }
        if (!_rData.extensionsRequired.empty())
        {
            GLTFCharValue json_value;
            if (!(_rData.extensionsRequired >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("extensionsRequired"), json_value, g_json_doc_ptr->GetAllocator());
        }
        if (!_rData.meshes.empty())
        {
            GLTFCharValue json_value;
            if (!(_rData.meshes >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("meshes"), json_value, g_json_doc_ptr->GetAllocator());
        }
        if (!_rData.animations.empty())
        {
            GLTFCharValue json_value;
            if (!(_rData.animations >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("animations"), json_value, g_json_doc_ptr->GetAllocator());
        }
        if (!_rData.images.empty())
        {
            GLTFCharValue json_value;
            if (!(_rData.images >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("images"), json_value, g_json_doc_ptr->GetAllocator());
        }
        if (!_rData.nodes.empty())
        {
            GLTFCharValue json_value;
            if (!(_rData.nodes >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("nodes"), json_value, g_json_doc_ptr->GetAllocator());
        }
        if (!_rData.bufferViews.empty())
        {
            GLTFCharValue json_value;
            if (!(_rData.bufferViews >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("bufferViews"), json_value, g_json_doc_ptr->GetAllocator());
        }
        if (!_rData.skins.empty())
        {
            GLTFCharValue json_value;
            if (!(_rData.skins >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("skins"), json_value, g_json_doc_ptr->GetAllocator());
        }
        if (!_rData.materials.empty())
        {
            GLTFCharValue json_value;
            if (!(_rData.materials >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("materials"), json_value, g_json_doc_ptr->GetAllocator());
        }
        if (!_rData.buffers.empty())
        {
            GLTFCharValue json_value;
            if (!(_rData.buffers >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("buffers"), json_value, g_json_doc_ptr->GetAllocator());
        }
        if (!!_rData.asset)
        {
            GLTFCharValue json_value;
            if (!(_rData.asset >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("asset"), json_value, g_json_doc_ptr->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SGlTF>& _pData, const GLTFCharValue& _JsonValue)
    {
        std::shared_ptr<SGlTF> data_ptr = !!_pData ? _pData : std::make_shared<SGlTF>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const std::shared_ptr<SGlTF>& _pData, GLTFCharValue& _JsonValue)
    {
        if (!_pData) return false;
        return (*_pData >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SGlTF>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SGlTF>>(_vDatas, _JsonValue);
    }

    bool operator>>(const std::vector<std::shared_ptr<SGlTF>>& _vDatas, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SGlTF>>(_vDatas, _JsonValue);
    }

    bool operator<<(STextureInfo& _rData, const GLTFCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("index")) && _JsonValue[GLTFTEXT("index")].IsInt())
        {
            if (!(_rData.index << _JsonValue[GLTFTEXT("index")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("texCoord")) && _JsonValue[GLTFTEXT("texCoord")].IsInt())
        {
            _rData.texCoord = _JsonValue[GLTFTEXT("texCoord")].GetInt();
        }
        _rData.schemaType = GLTFTEXT("textureInfo.schema.json");
        return true;
    }

    bool operator>>(const STextureInfo& _rData, GLTFCharValue& _JsonValue)
    {
        if (!g_json_doc_ptr) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr >> _JsonValue)) return false;
        }
        if (!!_rData.index)
        {
            GLTFCharValue json_value;
            if (!(_rData.index >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("index"), json_value, g_json_doc_ptr->GetAllocator());
        }
        {
            GLTFCharValue json_value;
            if (!(_rData.texCoord >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("texCoord"), json_value, g_json_doc_ptr->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<STextureInfo>& _pData, const GLTFCharValue& _JsonValue)
    {
        std::shared_ptr<STextureInfo> data_ptr = !!_pData ? _pData : std::make_shared<STextureInfo>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const std::shared_ptr<STextureInfo>& _pData, GLTFCharValue& _JsonValue)
    {
        if (!_pData) return false;
        return (*_pData >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<STextureInfo>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<STextureInfo>>(_vDatas, _JsonValue);
    }

    bool operator>>(const std::vector<std::shared_ptr<STextureInfo>>& _vDatas, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<STextureInfo>>(_vDatas, _JsonValue);
    }

}

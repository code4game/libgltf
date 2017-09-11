#include "libgltfpch.h"
#include "libgltfparser.h"
#include "libgltf.h"

namespace libgltf
{
    bool ParseByString(const std::wstring& _sContent, std::shared_ptr<SGlTF>& _pGlTF)
    {
        WCharDocument json_doc;
        json_doc.Parse(_sContent.c_str());
        if (!json_doc.IsObject()) return false;
        return (_pGlTF << json_doc.GetObject());
    }

    template<typename TData>
    bool operator<<(std::vector<TData>& _pDatas, const WCharValue& _JsonValue)
    {
        if (!_JsonValue.IsArray()) return false;
        std::vector<TData> datas;
        const WCharConstArray& json_array = _JsonValue.GetArray();
        size_t len = json_array.Size();
        if (len <= 0) return true;
        datas.resize(len);
        for (size_t i = 0; i < len; ++i) if (!(datas[i] << json_array[static_cast<rapidjson::SizeType>(i)])) return false;
        _pDatas = datas;
        return true;
    }

    template<>
    bool operator<<(std::vector<bool>& _pDatas, const WCharValue& _JsonValue)
    {
        if (!_JsonValue.IsArray()) return false;
        std::vector<bool> datas;
        const WCharConstArray& json_array = _JsonValue.GetArray();
        size_t len = json_array.Size();
        if (len <= 0) return true;
        datas.resize(len);
        for (size_t i = 0; i < len; ++i) datas[i] = json_array[static_cast<rapidjson::SizeType>(i)].GetBool();
        _pDatas = datas;
        return true;
    }

    template<>
    bool operator<<(std::vector<int32_t>& _pDatas, const WCharValue& _JsonValue)
    {
        if (!_JsonValue.IsArray()) return false;
        std::vector<int32_t> datas;
        const WCharConstArray& json_array = _JsonValue.GetArray();
        size_t len = json_array.Size();
        if (len <= 0) return true;
        datas.resize(len);
        for (size_t i = 0; i < len; ++i) datas[i] = json_array[static_cast<rapidjson::SizeType>(i)].GetInt();
        _pDatas = datas;
        return true;
    }

    template<>
    bool operator<<(std::vector<float>& _pDatas, const WCharValue& _JsonValue)
    {
        if (!_JsonValue.IsArray()) return false;
        std::vector<float> datas;
        const WCharConstArray& json_array = _JsonValue.GetArray();
        size_t len = json_array.Size();
        if (len <= 0) return true;
        datas.resize(len);
        for (size_t i = 0; i < len; ++i) datas[i] = json_array[static_cast<rapidjson::SizeType>(i)].GetFloat();
        _pDatas = datas;
        return true;
    }

    template<>
    bool operator<<(std::vector<std::wstring>& _pDatas, const WCharValue& _JsonValue)
    {
        if (!_JsonValue.IsArray()) return false;
        std::vector<std::wstring> datas;
        const WCharConstArray& json_array = _JsonValue.GetArray();
        size_t len = json_array.Size();
        if (len <= 0) return true;
        datas.resize(len);
        for (size_t i = 0; i < len; ++i) datas[i] = json_array[static_cast<rapidjson::SizeType>(i)].GetString();
        _pDatas = datas;
        return true;
    }

    bool operator<<(std::shared_ptr<SGlTFProperty>& _pData, const WCharValue& _JsonValue)
    {
        std::shared_ptr<SGlTFProperty> data_ptr = !!_pData ? _pData : std::make_shared<SGlTFProperty>();
        if (_JsonValue.HasMember(L"extras") && _JsonValue[L"extras"].IsObject())
        {
            if (!(data_ptr->extras << _JsonValue[L"extras"])) return false;
        }
        if (_JsonValue.HasMember(L"extensions") && _JsonValue[L"extensions"].IsObject())
        {
            if (!(data_ptr->extensions << _JsonValue[L"extensions"])) return false;
        }
        _pData = data_ptr;
        return true;
    }

    bool operator<<(std::vector<std::shared_ptr<SGlTFProperty>>& _pDatas, const WCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SGlTFProperty>>(_pDatas, _JsonValue);
    }

    bool operator<<(std::shared_ptr<SMaterial>& _pData, const WCharValue& _JsonValue)
    {
        std::shared_ptr<SMaterial> data_ptr = !!_pData ? _pData : std::make_shared<SMaterial>();
        if (_JsonValue.HasMember(L"alphaCutoff") && _JsonValue[L"alphaCutoff"].IsFloat())
        {
            data_ptr->alphaCutoff = _JsonValue[L"alphaCutoff"].GetFloat();
        }
        if (_JsonValue.HasMember(L"emissiveTexture") && _JsonValue[L"emissiveTexture"].IsObject())
        {
            if (!(data_ptr->emissiveTexture << _JsonValue[L"emissiveTexture"])) return false;
        }
        if (_JsonValue.HasMember(L"pbrMetallicRoughness") && _JsonValue[L"pbrMetallicRoughness"].IsObject())
        {
            if (!(data_ptr->pbrMetallicRoughness << _JsonValue[L"pbrMetallicRoughness"])) return false;
        }
        if (_JsonValue.HasMember(L"occlusionTexture") && _JsonValue[L"occlusionTexture"].IsObject())
        {
            if (!(data_ptr->occlusionTexture << _JsonValue[L"occlusionTexture"])) return false;
        }
        if (_JsonValue.HasMember(L"alphaMode") && _JsonValue[L"alphaMode"].IsString())
        {
            data_ptr->alphaMode = _JsonValue[L"alphaMode"].GetString();
        }
        if (_JsonValue.HasMember(L"doubleSided") && _JsonValue[L"doubleSided"].IsBool())
        {
            data_ptr->doubleSided = _JsonValue[L"doubleSided"].GetBool();
        }
        if (_JsonValue.HasMember(L"normalTexture") && _JsonValue[L"normalTexture"].IsObject())
        {
            if (!(data_ptr->normalTexture << _JsonValue[L"normalTexture"])) return false;
        }
        if (_JsonValue.HasMember(L"emissiveFactor") && _JsonValue[L"emissiveFactor"].IsArray())
        {
            if (!(data_ptr->emissiveFactor << _JsonValue[L"emissiveFactor"])) return false;
        }
        _pData = data_ptr;
        return true;
    }

    bool operator<<(std::vector<std::shared_ptr<SMaterial>>& _pDatas, const WCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SMaterial>>(_pDatas, _JsonValue);
    }

    bool operator<<(std::shared_ptr<SAsset>& _pData, const WCharValue& _JsonValue)
    {
        std::shared_ptr<SAsset> data_ptr = !!_pData ? _pData : std::make_shared<SAsset>();
        if (_JsonValue.HasMember(L"minVersion") && _JsonValue[L"minVersion"].IsString())
        {
            data_ptr->minVersion = _JsonValue[L"minVersion"].GetString();
        }
        if (_JsonValue.HasMember(L"version") && _JsonValue[L"version"].IsString())
        {
            data_ptr->version = _JsonValue[L"version"].GetString();
        }
        if (_JsonValue.HasMember(L"generator") && _JsonValue[L"generator"].IsString())
        {
            data_ptr->generator = _JsonValue[L"generator"].GetString();
        }
        if (_JsonValue.HasMember(L"copyright") && _JsonValue[L"copyright"].IsString())
        {
            data_ptr->copyright = _JsonValue[L"copyright"].GetString();
        }
        _pData = data_ptr;
        return true;
    }

    bool operator<<(std::vector<std::shared_ptr<SAsset>>& _pDatas, const WCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SAsset>>(_pDatas, _JsonValue);
    }

    bool operator<<(std::shared_ptr<SSampler>& _pData, const WCharValue& _JsonValue)
    {
        std::shared_ptr<SSampler> data_ptr = !!_pData ? _pData : std::make_shared<SSampler>();
        if (_JsonValue.HasMember(L"wrapS") && _JsonValue[L"wrapS"].IsInt())
        {
            data_ptr->wrapS = _JsonValue[L"wrapS"].GetInt();
        }
        if (_JsonValue.HasMember(L"minFilter") && _JsonValue[L"minFilter"].IsInt())
        {
            data_ptr->minFilter = _JsonValue[L"minFilter"].GetInt();
        }
        if (_JsonValue.HasMember(L"magFilter") && _JsonValue[L"magFilter"].IsInt())
        {
            data_ptr->magFilter = _JsonValue[L"magFilter"].GetInt();
        }
        if (_JsonValue.HasMember(L"wrapT") && _JsonValue[L"wrapT"].IsInt())
        {
            data_ptr->wrapT = _JsonValue[L"wrapT"].GetInt();
        }
        _pData = data_ptr;
        return true;
    }

    bool operator<<(std::vector<std::shared_ptr<SSampler>>& _pDatas, const WCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SSampler>>(_pDatas, _JsonValue);
    }

    bool operator<<(std::shared_ptr<SGlTFChildofRootProperty>& _pData, const WCharValue& _JsonValue)
    {
        std::shared_ptr<SGlTFChildofRootProperty> data_ptr = !!_pData ? _pData : std::make_shared<SGlTFChildofRootProperty>();
        if (_JsonValue.HasMember(L"name") && _JsonValue[L"name"].IsString())
        {
            data_ptr->name = _JsonValue[L"name"].GetString();
        }
        _pData = data_ptr;
        return true;
    }

    bool operator<<(std::vector<std::shared_ptr<SGlTFChildofRootProperty>>& _pDatas, const WCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SGlTFChildofRootProperty>>(_pDatas, _JsonValue);
    }

    bool operator<<(std::shared_ptr<SAnimationSampler>& _pData, const WCharValue& _JsonValue)
    {
        std::shared_ptr<SAnimationSampler> data_ptr = !!_pData ? _pData : std::make_shared<SAnimationSampler>();
        if (_JsonValue.HasMember(L"input") && _JsonValue[L"input"].IsInt())
        {
            data_ptr->input = _JsonValue[L"input"].GetInt();
        }
        if (_JsonValue.HasMember(L"output") && _JsonValue[L"output"].IsInt())
        {
            data_ptr->output = _JsonValue[L"output"].GetInt();
        }
        if (_JsonValue.HasMember(L"interpolation") && _JsonValue[L"interpolation"].IsString())
        {
            data_ptr->interpolation = _JsonValue[L"interpolation"].GetString();
        }
        _pData = data_ptr;
        return true;
    }

    bool operator<<(std::vector<std::shared_ptr<SAnimationSampler>>& _pDatas, const WCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SAnimationSampler>>(_pDatas, _JsonValue);
    }

    bool operator<<(std::shared_ptr<SExtras>& _pData, const WCharValue& _JsonValue)
    {
        std::shared_ptr<SExtras> data_ptr = !!_pData ? _pData : std::make_shared<SExtras>();
        _pData = data_ptr;
        return true;
    }

    bool operator<<(std::vector<std::shared_ptr<SExtras>>& _pDatas, const WCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SExtras>>(_pDatas, _JsonValue);
    }

    bool operator<<(std::shared_ptr<SScene>& _pData, const WCharValue& _JsonValue)
    {
        std::shared_ptr<SScene> data_ptr = !!_pData ? _pData : std::make_shared<SScene>();
        if (_JsonValue.HasMember(L"nodes") && _JsonValue[L"nodes"].IsArray())
        {
            if (!(data_ptr->nodes << _JsonValue[L"nodes"])) return false;
        }
        _pData = data_ptr;
        return true;
    }

    bool operator<<(std::vector<std::shared_ptr<SScene>>& _pDatas, const WCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SScene>>(_pDatas, _JsonValue);
    }

    bool operator<<(std::shared_ptr<SCameraPerspective>& _pData, const WCharValue& _JsonValue)
    {
        std::shared_ptr<SCameraPerspective> data_ptr = !!_pData ? _pData : std::make_shared<SCameraPerspective>();
        if (_JsonValue.HasMember(L"aspectRatio") && _JsonValue[L"aspectRatio"].IsFloat())
        {
            data_ptr->aspectRatio = _JsonValue[L"aspectRatio"].GetFloat();
        }
        if (_JsonValue.HasMember(L"yfov") && _JsonValue[L"yfov"].IsFloat())
        {
            data_ptr->yfov = _JsonValue[L"yfov"].GetFloat();
        }
        if (_JsonValue.HasMember(L"znear") && _JsonValue[L"znear"].IsFloat())
        {
            data_ptr->znear = _JsonValue[L"znear"].GetFloat();
        }
        if (_JsonValue.HasMember(L"zfar") && _JsonValue[L"zfar"].IsFloat())
        {
            data_ptr->zfar = _JsonValue[L"zfar"].GetFloat();
        }
        _pData = data_ptr;
        return true;
    }

    bool operator<<(std::vector<std::shared_ptr<SCameraPerspective>>& _pDatas, const WCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SCameraPerspective>>(_pDatas, _JsonValue);
    }

    bool operator<<(std::shared_ptr<SBufferView>& _pData, const WCharValue& _JsonValue)
    {
        std::shared_ptr<SBufferView> data_ptr = !!_pData ? _pData : std::make_shared<SBufferView>();
        if (_JsonValue.HasMember(L"byteLength") && _JsonValue[L"byteLength"].IsInt())
        {
            data_ptr->byteLength = _JsonValue[L"byteLength"].GetInt();
        }
        if (_JsonValue.HasMember(L"buffer") && _JsonValue[L"buffer"].IsInt())
        {
            data_ptr->buffer = _JsonValue[L"buffer"].GetInt();
        }
        if (_JsonValue.HasMember(L"byteOffset") && _JsonValue[L"byteOffset"].IsInt())
        {
            data_ptr->byteOffset = _JsonValue[L"byteOffset"].GetInt();
        }
        if (_JsonValue.HasMember(L"target") && _JsonValue[L"target"].IsInt())
        {
            data_ptr->target = _JsonValue[L"target"].GetInt();
        }
        if (_JsonValue.HasMember(L"byteStride") && _JsonValue[L"byteStride"].IsInt())
        {
            data_ptr->byteStride = _JsonValue[L"byteStride"].GetInt();
        }
        _pData = data_ptr;
        return true;
    }

    bool operator<<(std::vector<std::shared_ptr<SBufferView>>& _pDatas, const WCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SBufferView>>(_pDatas, _JsonValue);
    }

    bool operator<<(std::shared_ptr<SMaterialNormalTextureInfo>& _pData, const WCharValue& _JsonValue)
    {
        std::shared_ptr<SMaterialNormalTextureInfo> data_ptr = !!_pData ? _pData : std::make_shared<SMaterialNormalTextureInfo>();
        if (_JsonValue.HasMember(L"scale") && _JsonValue[L"scale"].IsFloat())
        {
            data_ptr->scale = _JsonValue[L"scale"].GetFloat();
        }
        _pData = data_ptr;
        return true;
    }

    bool operator<<(std::vector<std::shared_ptr<SMaterialNormalTextureInfo>>& _pDatas, const WCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SMaterialNormalTextureInfo>>(_pDatas, _JsonValue);
    }

    bool operator<<(std::shared_ptr<SMaterialOcclusionTextureInfo>& _pData, const WCharValue& _JsonValue)
    {
        std::shared_ptr<SMaterialOcclusionTextureInfo> data_ptr = !!_pData ? _pData : std::make_shared<SMaterialOcclusionTextureInfo>();
        if (_JsonValue.HasMember(L"strength") && _JsonValue[L"strength"].IsFloat())
        {
            data_ptr->strength = _JsonValue[L"strength"].GetFloat();
        }
        _pData = data_ptr;
        return true;
    }

    bool operator<<(std::vector<std::shared_ptr<SMaterialOcclusionTextureInfo>>& _pDatas, const WCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SMaterialOcclusionTextureInfo>>(_pDatas, _JsonValue);
    }

    bool operator<<(std::shared_ptr<SAccessorSparseValues>& _pData, const WCharValue& _JsonValue)
    {
        std::shared_ptr<SAccessorSparseValues> data_ptr = !!_pData ? _pData : std::make_shared<SAccessorSparseValues>();
        if (_JsonValue.HasMember(L"bufferView") && _JsonValue[L"bufferView"].IsInt())
        {
            data_ptr->bufferView = _JsonValue[L"bufferView"].GetInt();
        }
        if (_JsonValue.HasMember(L"byteOffset") && _JsonValue[L"byteOffset"].IsInt())
        {
            data_ptr->byteOffset = _JsonValue[L"byteOffset"].GetInt();
        }
        _pData = data_ptr;
        return true;
    }

    bool operator<<(std::vector<std::shared_ptr<SAccessorSparseValues>>& _pDatas, const WCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SAccessorSparseValues>>(_pDatas, _JsonValue);
    }

    bool operator<<(std::shared_ptr<SAnimationChannelTarget>& _pData, const WCharValue& _JsonValue)
    {
        std::shared_ptr<SAnimationChannelTarget> data_ptr = !!_pData ? _pData : std::make_shared<SAnimationChannelTarget>();
        if (_JsonValue.HasMember(L"node") && _JsonValue[L"node"].IsInt())
        {
            data_ptr->node = _JsonValue[L"node"].GetInt();
        }
        if (_JsonValue.HasMember(L"path") && _JsonValue[L"path"].IsString())
        {
            data_ptr->path = _JsonValue[L"path"].GetString();
        }
        _pData = data_ptr;
        return true;
    }

    bool operator<<(std::vector<std::shared_ptr<SAnimationChannelTarget>>& _pDatas, const WCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SAnimationChannelTarget>>(_pDatas, _JsonValue);
    }

    bool operator<<(std::shared_ptr<SMesh>& _pData, const WCharValue& _JsonValue)
    {
        std::shared_ptr<SMesh> data_ptr = !!_pData ? _pData : std::make_shared<SMesh>();
        if (_JsonValue.HasMember(L"primitives") && _JsonValue[L"primitives"].IsArray())
        {
            if (!(data_ptr->primitives << _JsonValue[L"primitives"])) return false;
        }
        if (_JsonValue.HasMember(L"weights") && _JsonValue[L"weights"].IsArray())
        {
            if (!(data_ptr->weights << _JsonValue[L"weights"])) return false;
        }
        _pData = data_ptr;
        return true;
    }

    bool operator<<(std::vector<std::shared_ptr<SMesh>>& _pDatas, const WCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SMesh>>(_pDatas, _JsonValue);
    }

    bool operator<<(std::shared_ptr<SAccessorSparse>& _pData, const WCharValue& _JsonValue)
    {
        std::shared_ptr<SAccessorSparse> data_ptr = !!_pData ? _pData : std::make_shared<SAccessorSparse>();
        if (_JsonValue.HasMember(L"count") && _JsonValue[L"count"].IsInt())
        {
            data_ptr->count = _JsonValue[L"count"].GetInt();
        }
        if (_JsonValue.HasMember(L"indices") && _JsonValue[L"indices"].IsObject())
        {
            if (!(data_ptr->indices << _JsonValue[L"indices"])) return false;
        }
        if (_JsonValue.HasMember(L"values") && _JsonValue[L"values"].IsObject())
        {
            if (!(data_ptr->values << _JsonValue[L"values"])) return false;
        }
        _pData = data_ptr;
        return true;
    }

    bool operator<<(std::vector<std::shared_ptr<SAccessorSparse>>& _pDatas, const WCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SAccessorSparse>>(_pDatas, _JsonValue);
    }

    bool operator<<(std::shared_ptr<SMeshPrimitive>& _pData, const WCharValue& _JsonValue)
    {
        std::shared_ptr<SMeshPrimitive> data_ptr = !!_pData ? _pData : std::make_shared<SMeshPrimitive>();
        if (_JsonValue.HasMember(L"indices") && _JsonValue[L"indices"].IsInt())
        {
            data_ptr->indices = _JsonValue[L"indices"].GetInt();
        }
        if (_JsonValue.HasMember(L"attributes") && _JsonValue[L"attributes"].IsInt())
        {
            data_ptr->attributes = _JsonValue[L"attributes"].GetInt();
        }
        if (_JsonValue.HasMember(L"material") && _JsonValue[L"material"].IsInt())
        {
            data_ptr->material = _JsonValue[L"material"].GetInt();
        }
        if (_JsonValue.HasMember(L"mode") && _JsonValue[L"mode"].IsInt())
        {
            data_ptr->mode = _JsonValue[L"mode"].GetInt();
        }
        if (_JsonValue.HasMember(L"targets") && _JsonValue[L"targets"].IsArray())
        {
            if (!(data_ptr->targets << _JsonValue[L"targets"])) return false;
        }
        _pData = data_ptr;
        return true;
    }

    bool operator<<(std::vector<std::shared_ptr<SMeshPrimitive>>& _pDatas, const WCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SMeshPrimitive>>(_pDatas, _JsonValue);
    }

    bool operator<<(std::shared_ptr<SExtension>& _pData, const WCharValue& _JsonValue)
    {
        std::shared_ptr<SExtension> data_ptr = !!_pData ? _pData : std::make_shared<SExtension>();
        _pData = data_ptr;
        return true;
    }

    bool operator<<(std::vector<std::shared_ptr<SExtension>>& _pDatas, const WCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SExtension>>(_pDatas, _JsonValue);
    }

    bool operator<<(std::shared_ptr<SAnimationChannel>& _pData, const WCharValue& _JsonValue)
    {
        std::shared_ptr<SAnimationChannel> data_ptr = !!_pData ? _pData : std::make_shared<SAnimationChannel>();
        if (_JsonValue.HasMember(L"target") && _JsonValue[L"target"].IsObject())
        {
            if (!(data_ptr->target << _JsonValue[L"target"])) return false;
        }
        if (_JsonValue.HasMember(L"sampler") && _JsonValue[L"sampler"].IsInt())
        {
            data_ptr->sampler = _JsonValue[L"sampler"].GetInt();
        }
        _pData = data_ptr;
        return true;
    }

    bool operator<<(std::vector<std::shared_ptr<SAnimationChannel>>& _pDatas, const WCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SAnimationChannel>>(_pDatas, _JsonValue);
    }

    bool operator<<(std::shared_ptr<SAccessorSparseIndices>& _pData, const WCharValue& _JsonValue)
    {
        std::shared_ptr<SAccessorSparseIndices> data_ptr = !!_pData ? _pData : std::make_shared<SAccessorSparseIndices>();
        if (_JsonValue.HasMember(L"componentType") && _JsonValue[L"componentType"].IsInt())
        {
            data_ptr->componentType = _JsonValue[L"componentType"].GetInt();
        }
        if (_JsonValue.HasMember(L"bufferView") && _JsonValue[L"bufferView"].IsInt())
        {
            data_ptr->bufferView = _JsonValue[L"bufferView"].GetInt();
        }
        if (_JsonValue.HasMember(L"byteOffset") && _JsonValue[L"byteOffset"].IsInt())
        {
            data_ptr->byteOffset = _JsonValue[L"byteOffset"].GetInt();
        }
        _pData = data_ptr;
        return true;
    }

    bool operator<<(std::vector<std::shared_ptr<SAccessorSparseIndices>>& _pDatas, const WCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SAccessorSparseIndices>>(_pDatas, _JsonValue);
    }

    bool operator<<(std::shared_ptr<SNode>& _pData, const WCharValue& _JsonValue)
    {
        std::shared_ptr<SNode> data_ptr = !!_pData ? _pData : std::make_shared<SNode>();
        if (_JsonValue.HasMember(L"scale") && _JsonValue[L"scale"].IsArray())
        {
            if (!(data_ptr->scale << _JsonValue[L"scale"])) return false;
        }
        if (_JsonValue.HasMember(L"rotation") && _JsonValue[L"rotation"].IsArray())
        {
            if (!(data_ptr->rotation << _JsonValue[L"rotation"])) return false;
        }
        if (_JsonValue.HasMember(L"matrix") && _JsonValue[L"matrix"].IsArray())
        {
            if (!(data_ptr->matrix << _JsonValue[L"matrix"])) return false;
        }
        if (_JsonValue.HasMember(L"mesh") && _JsonValue[L"mesh"].IsInt())
        {
            data_ptr->mesh = _JsonValue[L"mesh"].GetInt();
        }
        if (_JsonValue.HasMember(L"camera") && _JsonValue[L"camera"].IsInt())
        {
            data_ptr->camera = _JsonValue[L"camera"].GetInt();
        }
        if (_JsonValue.HasMember(L"weights") && _JsonValue[L"weights"].IsArray())
        {
            if (!(data_ptr->weights << _JsonValue[L"weights"])) return false;
        }
        if (_JsonValue.HasMember(L"skin") && _JsonValue[L"skin"].IsInt())
        {
            data_ptr->skin = _JsonValue[L"skin"].GetInt();
        }
        if (_JsonValue.HasMember(L"translation") && _JsonValue[L"translation"].IsArray())
        {
            if (!(data_ptr->translation << _JsonValue[L"translation"])) return false;
        }
        if (_JsonValue.HasMember(L"children") && _JsonValue[L"children"].IsArray())
        {
            if (!(data_ptr->children << _JsonValue[L"children"])) return false;
        }
        _pData = data_ptr;
        return true;
    }

    bool operator<<(std::vector<std::shared_ptr<SNode>>& _pDatas, const WCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SNode>>(_pDatas, _JsonValue);
    }

    bool operator<<(std::shared_ptr<SAnimation>& _pData, const WCharValue& _JsonValue)
    {
        std::shared_ptr<SAnimation> data_ptr = !!_pData ? _pData : std::make_shared<SAnimation>();
        if (_JsonValue.HasMember(L"channels") && _JsonValue[L"channels"].IsArray())
        {
            if (!(data_ptr->channels << _JsonValue[L"channels"])) return false;
        }
        if (_JsonValue.HasMember(L"samplers") && _JsonValue[L"samplers"].IsArray())
        {
            if (!(data_ptr->samplers << _JsonValue[L"samplers"])) return false;
        }
        _pData = data_ptr;
        return true;
    }

    bool operator<<(std::vector<std::shared_ptr<SAnimation>>& _pDatas, const WCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SAnimation>>(_pDatas, _JsonValue);
    }

    bool operator<<(std::shared_ptr<SSkin>& _pData, const WCharValue& _JsonValue)
    {
        std::shared_ptr<SSkin> data_ptr = !!_pData ? _pData : std::make_shared<SSkin>();
        if (_JsonValue.HasMember(L"joints") && _JsonValue[L"joints"].IsArray())
        {
            if (!(data_ptr->joints << _JsonValue[L"joints"])) return false;
        }
        if (_JsonValue.HasMember(L"inverseBindMatrices") && _JsonValue[L"inverseBindMatrices"].IsInt())
        {
            data_ptr->inverseBindMatrices = _JsonValue[L"inverseBindMatrices"].GetInt();
        }
        if (_JsonValue.HasMember(L"skeleton") && _JsonValue[L"skeleton"].IsInt())
        {
            data_ptr->skeleton = _JsonValue[L"skeleton"].GetInt();
        }
        _pData = data_ptr;
        return true;
    }

    bool operator<<(std::vector<std::shared_ptr<SSkin>>& _pDatas, const WCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SSkin>>(_pDatas, _JsonValue);
    }

    bool operator<<(std::shared_ptr<SMaterialPBRMetallicRoughness>& _pData, const WCharValue& _JsonValue)
    {
        std::shared_ptr<SMaterialPBRMetallicRoughness> data_ptr = !!_pData ? _pData : std::make_shared<SMaterialPBRMetallicRoughness>();
        if (_JsonValue.HasMember(L"roughnessFactor") && _JsonValue[L"roughnessFactor"].IsFloat())
        {
            data_ptr->roughnessFactor = _JsonValue[L"roughnessFactor"].GetFloat();
        }
        if (_JsonValue.HasMember(L"baseColorTexture") && _JsonValue[L"baseColorTexture"].IsObject())
        {
            if (!(data_ptr->baseColorTexture << _JsonValue[L"baseColorTexture"])) return false;
        }
        if (_JsonValue.HasMember(L"metallicFactor") && _JsonValue[L"metallicFactor"].IsFloat())
        {
            data_ptr->metallicFactor = _JsonValue[L"metallicFactor"].GetFloat();
        }
        if (_JsonValue.HasMember(L"baseColorFactor") && _JsonValue[L"baseColorFactor"].IsArray())
        {
            if (!(data_ptr->baseColorFactor << _JsonValue[L"baseColorFactor"])) return false;
        }
        if (_JsonValue.HasMember(L"metallicRoughnessTexture") && _JsonValue[L"metallicRoughnessTexture"].IsObject())
        {
            if (!(data_ptr->metallicRoughnessTexture << _JsonValue[L"metallicRoughnessTexture"])) return false;
        }
        _pData = data_ptr;
        return true;
    }

    bool operator<<(std::vector<std::shared_ptr<SMaterialPBRMetallicRoughness>>& _pDatas, const WCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SMaterialPBRMetallicRoughness>>(_pDatas, _JsonValue);
    }

    bool operator<<(std::shared_ptr<SCamera>& _pData, const WCharValue& _JsonValue)
    {
        std::shared_ptr<SCamera> data_ptr = !!_pData ? _pData : std::make_shared<SCamera>();
        if (_JsonValue.HasMember(L"type") && _JsonValue[L"type"].IsString())
        {
            data_ptr->type = _JsonValue[L"type"].GetString();
        }
        if (_JsonValue.HasMember(L"perspective") && _JsonValue[L"perspective"].IsObject())
        {
            if (!(data_ptr->perspective << _JsonValue[L"perspective"])) return false;
        }
        if (_JsonValue.HasMember(L"orthographic") && _JsonValue[L"orthographic"].IsObject())
        {
            if (!(data_ptr->orthographic << _JsonValue[L"orthographic"])) return false;
        }
        _pData = data_ptr;
        return true;
    }

    bool operator<<(std::vector<std::shared_ptr<SCamera>>& _pDatas, const WCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SCamera>>(_pDatas, _JsonValue);
    }

    bool operator<<(std::shared_ptr<SImage>& _pData, const WCharValue& _JsonValue)
    {
        std::shared_ptr<SImage> data_ptr = !!_pData ? _pData : std::make_shared<SImage>();
        if (_JsonValue.HasMember(L"mimeType") && _JsonValue[L"mimeType"].IsString())
        {
            data_ptr->mimeType = _JsonValue[L"mimeType"].GetString();
        }
        if (_JsonValue.HasMember(L"bufferView") && _JsonValue[L"bufferView"].IsInt())
        {
            data_ptr->bufferView = _JsonValue[L"bufferView"].GetInt();
        }
        if (_JsonValue.HasMember(L"uri") && _JsonValue[L"uri"].IsString())
        {
            data_ptr->uri = _JsonValue[L"uri"].GetString();
        }
        _pData = data_ptr;
        return true;
    }

    bool operator<<(std::vector<std::shared_ptr<SImage>>& _pDatas, const WCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SImage>>(_pDatas, _JsonValue);
    }

    bool operator<<(std::shared_ptr<STexture>& _pData, const WCharValue& _JsonValue)
    {
        std::shared_ptr<STexture> data_ptr = !!_pData ? _pData : std::make_shared<STexture>();
        if (_JsonValue.HasMember(L"source") && _JsonValue[L"source"].IsInt())
        {
            data_ptr->source = _JsonValue[L"source"].GetInt();
        }
        if (_JsonValue.HasMember(L"sampler") && _JsonValue[L"sampler"].IsInt())
        {
            data_ptr->sampler = _JsonValue[L"sampler"].GetInt();
        }
        _pData = data_ptr;
        return true;
    }

    bool operator<<(std::vector<std::shared_ptr<STexture>>& _pDatas, const WCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<STexture>>(_pDatas, _JsonValue);
    }

    bool operator<<(std::shared_ptr<SCameraOrthographic>& _pData, const WCharValue& _JsonValue)
    {
        std::shared_ptr<SCameraOrthographic> data_ptr = !!_pData ? _pData : std::make_shared<SCameraOrthographic>();
        if (_JsonValue.HasMember(L"xmag") && _JsonValue[L"xmag"].IsFloat())
        {
            data_ptr->xmag = _JsonValue[L"xmag"].GetFloat();
        }
        if (_JsonValue.HasMember(L"ymag") && _JsonValue[L"ymag"].IsFloat())
        {
            data_ptr->ymag = _JsonValue[L"ymag"].GetFloat();
        }
        if (_JsonValue.HasMember(L"zfar") && _JsonValue[L"zfar"].IsFloat())
        {
            data_ptr->zfar = _JsonValue[L"zfar"].GetFloat();
        }
        if (_JsonValue.HasMember(L"znear") && _JsonValue[L"znear"].IsFloat())
        {
            data_ptr->znear = _JsonValue[L"znear"].GetFloat();
        }
        _pData = data_ptr;
        return true;
    }

    bool operator<<(std::vector<std::shared_ptr<SCameraOrthographic>>& _pDatas, const WCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SCameraOrthographic>>(_pDatas, _JsonValue);
    }

    bool operator<<(std::shared_ptr<SBuffer>& _pData, const WCharValue& _JsonValue)
    {
        std::shared_ptr<SBuffer> data_ptr = !!_pData ? _pData : std::make_shared<SBuffer>();
        if (_JsonValue.HasMember(L"byteLength") && _JsonValue[L"byteLength"].IsInt())
        {
            data_ptr->byteLength = _JsonValue[L"byteLength"].GetInt();
        }
        if (_JsonValue.HasMember(L"uri") && _JsonValue[L"uri"].IsString())
        {
            data_ptr->uri = _JsonValue[L"uri"].GetString();
        }
        _pData = data_ptr;
        return true;
    }

    bool operator<<(std::vector<std::shared_ptr<SBuffer>>& _pDatas, const WCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SBuffer>>(_pDatas, _JsonValue);
    }

    bool operator<<(std::shared_ptr<SAccessor>& _pData, const WCharValue& _JsonValue)
    {
        std::shared_ptr<SAccessor> data_ptr = !!_pData ? _pData : std::make_shared<SAccessor>();
        if (_JsonValue.HasMember(L"count") && _JsonValue[L"count"].IsInt())
        {
            data_ptr->count = _JsonValue[L"count"].GetInt();
        }
        if (_JsonValue.HasMember(L"min") && _JsonValue[L"min"].IsArray())
        {
            if (!(data_ptr->min << _JsonValue[L"min"])) return false;
        }
        if (_JsonValue.HasMember(L"max") && _JsonValue[L"max"].IsArray())
        {
            if (!(data_ptr->max << _JsonValue[L"max"])) return false;
        }
        if (_JsonValue.HasMember(L"bufferView") && _JsonValue[L"bufferView"].IsInt())
        {
            data_ptr->bufferView = _JsonValue[L"bufferView"].GetInt();
        }
        if (_JsonValue.HasMember(L"componentType") && _JsonValue[L"componentType"].IsInt())
        {
            data_ptr->componentType = _JsonValue[L"componentType"].GetInt();
        }
        if (_JsonValue.HasMember(L"byteOffset") && _JsonValue[L"byteOffset"].IsInt())
        {
            data_ptr->byteOffset = _JsonValue[L"byteOffset"].GetInt();
        }
        if (_JsonValue.HasMember(L"sparse") && _JsonValue[L"sparse"].IsObject())
        {
            if (!(data_ptr->sparse << _JsonValue[L"sparse"])) return false;
        }
        if (_JsonValue.HasMember(L"type") && _JsonValue[L"type"].IsString())
        {
            data_ptr->type = _JsonValue[L"type"].GetString();
        }
        if (_JsonValue.HasMember(L"normalized") && _JsonValue[L"normalized"].IsBool())
        {
            data_ptr->normalized = _JsonValue[L"normalized"].GetBool();
        }
        _pData = data_ptr;
        return true;
    }

    bool operator<<(std::vector<std::shared_ptr<SAccessor>>& _pDatas, const WCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SAccessor>>(_pDatas, _JsonValue);
    }

    bool operator<<(std::shared_ptr<SGlTF>& _pData, const WCharValue& _JsonValue)
    {
        std::shared_ptr<SGlTF> data_ptr = !!_pData ? _pData : std::make_shared<SGlTF>();
        if (_JsonValue.HasMember(L"textures") && _JsonValue[L"textures"].IsArray())
        {
            if (!(data_ptr->textures << _JsonValue[L"textures"])) return false;
        }
        if (_JsonValue.HasMember(L"cameras") && _JsonValue[L"cameras"].IsArray())
        {
            if (!(data_ptr->cameras << _JsonValue[L"cameras"])) return false;
        }
        if (_JsonValue.HasMember(L"accessors") && _JsonValue[L"accessors"].IsArray())
        {
            if (!(data_ptr->accessors << _JsonValue[L"accessors"])) return false;
        }
        if (_JsonValue.HasMember(L"extensionsUsed") && _JsonValue[L"extensionsUsed"].IsArray())
        {
            if (!(data_ptr->extensionsUsed << _JsonValue[L"extensionsUsed"])) return false;
        }
        if (_JsonValue.HasMember(L"samplers") && _JsonValue[L"samplers"].IsArray())
        {
            if (!(data_ptr->samplers << _JsonValue[L"samplers"])) return false;
        }
        if (_JsonValue.HasMember(L"scenes") && _JsonValue[L"scenes"].IsArray())
        {
            if (!(data_ptr->scenes << _JsonValue[L"scenes"])) return false;
        }
        if (_JsonValue.HasMember(L"scene") && _JsonValue[L"scene"].IsInt())
        {
            data_ptr->scene = _JsonValue[L"scene"].GetInt();
        }
        if (_JsonValue.HasMember(L"extensionsRequired") && _JsonValue[L"extensionsRequired"].IsArray())
        {
            if (!(data_ptr->extensionsRequired << _JsonValue[L"extensionsRequired"])) return false;
        }
        if (_JsonValue.HasMember(L"meshes") && _JsonValue[L"meshes"].IsArray())
        {
            if (!(data_ptr->meshes << _JsonValue[L"meshes"])) return false;
        }
        if (_JsonValue.HasMember(L"animations") && _JsonValue[L"animations"].IsArray())
        {
            if (!(data_ptr->animations << _JsonValue[L"animations"])) return false;
        }
        if (_JsonValue.HasMember(L"images") && _JsonValue[L"images"].IsArray())
        {
            if (!(data_ptr->images << _JsonValue[L"images"])) return false;
        }
        if (_JsonValue.HasMember(L"nodes") && _JsonValue[L"nodes"].IsArray())
        {
            if (!(data_ptr->nodes << _JsonValue[L"nodes"])) return false;
        }
        if (_JsonValue.HasMember(L"bufferViews") && _JsonValue[L"bufferViews"].IsArray())
        {
            if (!(data_ptr->bufferViews << _JsonValue[L"bufferViews"])) return false;
        }
        if (_JsonValue.HasMember(L"skins") && _JsonValue[L"skins"].IsArray())
        {
            if (!(data_ptr->skins << _JsonValue[L"skins"])) return false;
        }
        if (_JsonValue.HasMember(L"materials") && _JsonValue[L"materials"].IsArray())
        {
            if (!(data_ptr->materials << _JsonValue[L"materials"])) return false;
        }
        if (_JsonValue.HasMember(L"buffers") && _JsonValue[L"buffers"].IsArray())
        {
            if (!(data_ptr->buffers << _JsonValue[L"buffers"])) return false;
        }
        if (_JsonValue.HasMember(L"asset") && _JsonValue[L"asset"].IsObject())
        {
            if (!(data_ptr->asset << _JsonValue[L"asset"])) return false;
        }
        _pData = data_ptr;
        return true;
    }

    bool operator<<(std::vector<std::shared_ptr<SGlTF>>& _pDatas, const WCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SGlTF>>(_pDatas, _JsonValue);
    }

    bool operator<<(std::shared_ptr<STextureInfo>& _pData, const WCharValue& _JsonValue)
    {
        std::shared_ptr<STextureInfo> data_ptr = !!_pData ? _pData : std::make_shared<STextureInfo>();
        if (_JsonValue.HasMember(L"index") && _JsonValue[L"index"].IsInt())
        {
            data_ptr->index = _JsonValue[L"index"].GetInt();
        }
        if (_JsonValue.HasMember(L"texCoord") && _JsonValue[L"texCoord"].IsInt())
        {
            data_ptr->texCoord = _JsonValue[L"texCoord"].GetInt();
        }
        _pData = data_ptr;
        return true;
    }

    bool operator<<(std::vector<std::shared_ptr<STextureInfo>>& _pDatas, const WCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<STextureInfo>>(_pDatas, _JsonValue);
    }

}

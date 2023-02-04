/*
 * This software is released under the MIT license.
 * 
 * Copyright (c) 2017-2023 Code 4 Game, Org. All Rights Reserved.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "libgltfparser.h"

namespace libgltf
{
    bool operator<<(SGlTF& _gltf, const std::string& _sContent)
    {
        JSONCharDocument json_doc;
        json_doc.Parse(_sContent.c_str());
        if (!json_doc.IsObject()) return false;
        return (_gltf << json_doc.GetObject());
    }

    bool operator>>(const SGlTF& _gltf, std::string& _sContent)
    {
        JSONCharDocument json_doc;
        if (!(TDataDoc<SGlTF>(_gltf, &json_doc) >> json_doc)) return false;
        JSONStringBuffer json_string_buffer;
        JSONWriter json_writer(json_string_buffer);
        if (!json_doc.Accept(json_writer)) return false;
        _sContent = json_string_buffer.GetString();
        return true;
    }

    bool operator<<(bool& _rData, const JSONCharValue& _JsonValue)
    {
        if (!_JsonValue.IsBool()) return false;
        _rData = _JsonValue.GetBool();
        return true;
    }

    bool operator>>(const TDataDoc<bool>& _rData, JSONCharValue& _JsonValue)
    {
        _JsonValue.SetBool(_rData.data);
        return true;
    }

    bool operator<<(int32_t& _rData, const JSONCharValue& _JsonValue)
    {
        if (!_JsonValue.IsInt()) return false;
        _rData = _JsonValue.GetInt();
        return true;
    }

    bool operator>>(const TDataDoc<int32_t>& _rData, JSONCharValue& _JsonValue)
    {
        _JsonValue.SetInt(_rData.data);
        return true;
    }

    bool operator<<(float& _rData, const JSONCharValue& _JsonValue)
    {
        if (_JsonValue.IsNumber())
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

    bool operator>>(const TDataDoc<float>& _rData, JSONCharValue& _JsonValue)
    {
        _JsonValue.SetFloat(_rData.data);
        return true;
    }

    bool operator<<(std::string& _rData, const JSONCharValue& _JsonValue)
    {
        if (!_JsonValue.IsString()) return false;
        _rData = _JsonValue.GetString();
        return true;
    }

    bool operator>>(const TDataDoc<std::string>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetString(_rData.data.c_str(), _rData.doc->GetAllocator());
        return true;
    }

    template<typename TData>
    bool operator<<(std::vector<TData>& _vDatas, const JSONCharValue& _JsonValue)
    {
        if (!_JsonValue.IsArray()) return false;
        std::vector<TData> datas;
        const JSONCharConstArray& json_array = _JsonValue.GetArray();
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
    bool operator>>(const TDataDoc<std::vector<TData>>& _rData, JSONCharValue& _JsonValue)
    {
        if (_rData.data.empty() || !_rData.doc) return false;
        JSONCharValue& json_array = _JsonValue.SetArray();
        for (const TData& data : _rData.data)
        {
            JSONCharValue json_value;
            if (!(TDataDoc<TData>(data, _rData.doc) >> json_value)) return false;
            json_array.PushBack(json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    template<typename TData>
    bool operator<<(std::map<std::string, TData>& _mDatas, const JSONCharValue& _JsonValue)
    {
        if (!_JsonValue.IsObject()) return false;
        std::map<std::string, TData> datas;
        const JSONCharConstObject& json_object = _JsonValue.GetObject();
        for (JSONCharConstObject::ConstMemberIterator cit = json_object.MemberBegin(); cit != json_object.MemberEnd(); ++cit)
        {
            TData data;
            if (!(data << cit->value)) return false;
            datas.insert(std::make_pair(cit->name.GetString(), data));
        }
        _mDatas = datas;
        return true;
    }

    template<typename TData>
    bool operator>>(const TDataDoc<std::map<std::string, TData>>& _rData, JSONCharValue& _JsonValue)
    {
        if (_rData.data.empty() || !_rData.doc) return false;
        _JsonValue.SetObject();
        for (const std::pair<std::string, TData>& data : _rData.data)
        {
            JSONCharValue json_value;
            if (!(TDataDoc<TData>(data.second, _rData.doc) >> json_value)) return false;
            JSONCharValue json_key(data.first.c_str(), _rData.doc->GetAllocator());
            _JsonValue.AddMember(json_key, json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(SAccessor& _rData, const JSONCharValue& _JsonValue)
    {
        {
            SGlTFChildofRootProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember("bufferView") && _JsonValue["bufferView"].IsInt())
        {
            if (!(_rData.bufferView << _JsonValue["bufferView"])) return false;
        }
        if (_JsonValue.HasMember("byteOffset") && _JsonValue["byteOffset"].IsInt())
        {
            _rData.byteOffset = _JsonValue["byteOffset"].GetInt();
        }
        if (_JsonValue.HasMember("componentType") && _JsonValue["componentType"].IsInt())
        {
            _rData.componentType = _JsonValue["componentType"].GetInt();
        }
        if (_JsonValue.HasMember("normalized") && _JsonValue["normalized"].IsBool())
        {
            _rData.normalized = _JsonValue["normalized"].GetBool();
        }
        if (_JsonValue.HasMember("count") && _JsonValue["count"].IsInt())
        {
            _rData.count = _JsonValue["count"].GetInt();
        }
        if (_JsonValue.HasMember("type") && _JsonValue["type"].IsString())
        {
            _rData.type = _JsonValue["type"].GetString();
        }
        if (_JsonValue.HasMember("max") && _JsonValue["max"].IsArray())
        {
            if (!(_rData.max << _JsonValue["max"])) return false;
        }
        if (_JsonValue.HasMember("min") && _JsonValue["min"].IsArray())
        {
            if (!(_rData.min << _JsonValue["min"])) return false;
        }
        if (_JsonValue.HasMember("sparse") && _JsonValue["sparse"].IsObject())
        {
            if (!(_rData.sparse << _JsonValue["sparse"])) return false;
        }
        _rData.schemaType = "accessor.schema.json";
        return true;
    }

    bool operator>>(const TDataDoc<SAccessor>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFChildofRootProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFChildofRootProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        if (!!_rData.data.bufferView)
        {
            JSONCharValue json_value;
            if (!(TDataDoc<SGlTFId>(*_rData.data.bufferView, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("bufferView", json_value, _rData.doc->GetAllocator());
        }
        {
            JSONCharValue json_value;
            if (!(TDataDoc<int32_t>(_rData.data.byteOffset, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("byteOffset", json_value, _rData.doc->GetAllocator());
        }
        {
            JSONCharValue json_value;
            if (!(TDataDoc<int32_t>(_rData.data.componentType, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("componentType", json_value, _rData.doc->GetAllocator());
        }
        {
            JSONCharValue json_value;
            if (!(TDataDoc<bool>(_rData.data.normalized, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("normalized", json_value, _rData.doc->GetAllocator());
        }
        {
            JSONCharValue json_value;
            if (!(TDataDoc<int32_t>(_rData.data.count, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("count", json_value, _rData.doc->GetAllocator());
        }
        {
            JSONCharValue json_value;
            if (!(TDataDoc<std::string>(_rData.data.type, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("type", json_value, _rData.doc->GetAllocator());
        }
        if (!_rData.data.max.empty())
        {
            JSONCharValue json_value;
            if (!(TDataDoc<std::vector<float>>(_rData.data.max, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("max", json_value, _rData.doc->GetAllocator());
        }
        if (!_rData.data.min.empty())
        {
            JSONCharValue json_value;
            if (!(TDataDoc<std::vector<float>>(_rData.data.min, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("min", json_value, _rData.doc->GetAllocator());
        }
        if (!!_rData.data.sparse)
        {
            JSONCharValue json_value;
            if (!(TDataDoc<SAccessorSparse>(*_rData.data.sparse, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("sparse", json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SAccessor>& _pData, const JSONCharValue& _JsonValue)
    {
        std::shared_ptr<SAccessor> data_ptr = !!_pData ? _pData : std::make_shared<SAccessor>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SAccessor>>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SAccessor>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SAccessor>>& _vDatas, const JSONCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SAccessor>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SAccessor>>>& _rData, JSONCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SAccessor>>(_rData, _JsonValue);
    }

    bool operator<<(SAccessorSparseIndices& _rData, const JSONCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember("bufferView") && _JsonValue["bufferView"].IsInt())
        {
            if (!(_rData.bufferView << _JsonValue["bufferView"])) return false;
        }
        if (_JsonValue.HasMember("byteOffset") && _JsonValue["byteOffset"].IsInt())
        {
            _rData.byteOffset = _JsonValue["byteOffset"].GetInt();
        }
        if (_JsonValue.HasMember("componentType") && _JsonValue["componentType"].IsInt())
        {
            _rData.componentType = _JsonValue["componentType"].GetInt();
        }
        _rData.schemaType = "accessor.sparse.indices.schema.json";
        return true;
    }

    bool operator>>(const TDataDoc<SAccessorSparseIndices>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        if (!!_rData.data.bufferView)
        {
            JSONCharValue json_value;
            if (!(TDataDoc<SGlTFId>(*_rData.data.bufferView, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("bufferView", json_value, _rData.doc->GetAllocator());
        }
        {
            JSONCharValue json_value;
            if (!(TDataDoc<int32_t>(_rData.data.byteOffset, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("byteOffset", json_value, _rData.doc->GetAllocator());
        }
        {
            JSONCharValue json_value;
            if (!(TDataDoc<int32_t>(_rData.data.componentType, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("componentType", json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SAccessorSparseIndices>& _pData, const JSONCharValue& _JsonValue)
    {
        std::shared_ptr<SAccessorSparseIndices> data_ptr = !!_pData ? _pData : std::make_shared<SAccessorSparseIndices>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SAccessorSparseIndices>>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SAccessorSparseIndices>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SAccessorSparseIndices>>& _vDatas, const JSONCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SAccessorSparseIndices>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SAccessorSparseIndices>>>& _rData, JSONCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SAccessorSparseIndices>>(_rData, _JsonValue);
    }

    bool operator<<(SAccessorSparse& _rData, const JSONCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember("count") && _JsonValue["count"].IsInt())
        {
            _rData.count = _JsonValue["count"].GetInt();
        }
        if (_JsonValue.HasMember("indices") && _JsonValue["indices"].IsObject())
        {
            if (!(_rData.indices << _JsonValue["indices"])) return false;
        }
        if (_JsonValue.HasMember("values") && _JsonValue["values"].IsObject())
        {
            if (!(_rData.values << _JsonValue["values"])) return false;
        }
        _rData.schemaType = "accessor.sparse.schema.json";
        return true;
    }

    bool operator>>(const TDataDoc<SAccessorSparse>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        {
            JSONCharValue json_value;
            if (!(TDataDoc<int32_t>(_rData.data.count, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("count", json_value, _rData.doc->GetAllocator());
        }
        if (!!_rData.data.indices)
        {
            JSONCharValue json_value;
            if (!(TDataDoc<SAccessorSparseIndices>(*_rData.data.indices, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("indices", json_value, _rData.doc->GetAllocator());
        }
        if (!!_rData.data.values)
        {
            JSONCharValue json_value;
            if (!(TDataDoc<SAccessorSparseValues>(*_rData.data.values, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("values", json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SAccessorSparse>& _pData, const JSONCharValue& _JsonValue)
    {
        std::shared_ptr<SAccessorSparse> data_ptr = !!_pData ? _pData : std::make_shared<SAccessorSparse>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SAccessorSparse>>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SAccessorSparse>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SAccessorSparse>>& _vDatas, const JSONCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SAccessorSparse>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SAccessorSparse>>>& _rData, JSONCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SAccessorSparse>>(_rData, _JsonValue);
    }

    bool operator<<(SAccessorSparseValues& _rData, const JSONCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember("bufferView") && _JsonValue["bufferView"].IsInt())
        {
            if (!(_rData.bufferView << _JsonValue["bufferView"])) return false;
        }
        if (_JsonValue.HasMember("byteOffset") && _JsonValue["byteOffset"].IsInt())
        {
            _rData.byteOffset = _JsonValue["byteOffset"].GetInt();
        }
        _rData.schemaType = "accessor.sparse.values.schema.json";
        return true;
    }

    bool operator>>(const TDataDoc<SAccessorSparseValues>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        if (!!_rData.data.bufferView)
        {
            JSONCharValue json_value;
            if (!(TDataDoc<SGlTFId>(*_rData.data.bufferView, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("bufferView", json_value, _rData.doc->GetAllocator());
        }
        {
            JSONCharValue json_value;
            if (!(TDataDoc<int32_t>(_rData.data.byteOffset, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("byteOffset", json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SAccessorSparseValues>& _pData, const JSONCharValue& _JsonValue)
    {
        std::shared_ptr<SAccessorSparseValues> data_ptr = !!_pData ? _pData : std::make_shared<SAccessorSparseValues>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SAccessorSparseValues>>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SAccessorSparseValues>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SAccessorSparseValues>>& _vDatas, const JSONCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SAccessorSparseValues>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SAccessorSparseValues>>>& _rData, JSONCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SAccessorSparseValues>>(_rData, _JsonValue);
    }

    bool operator<<(SAnimationChannel& _rData, const JSONCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember("sampler") && _JsonValue["sampler"].IsInt())
        {
            if (!(_rData.sampler << _JsonValue["sampler"])) return false;
        }
        if (_JsonValue.HasMember("target") && _JsonValue["target"].IsObject())
        {
            if (!(_rData.target << _JsonValue["target"])) return false;
        }
        _rData.schemaType = "animation.channel.schema.json";
        return true;
    }

    bool operator>>(const TDataDoc<SAnimationChannel>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        if (!!_rData.data.sampler)
        {
            JSONCharValue json_value;
            if (!(TDataDoc<SGlTFId>(*_rData.data.sampler, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("sampler", json_value, _rData.doc->GetAllocator());
        }
        if (!!_rData.data.target)
        {
            JSONCharValue json_value;
            if (!(TDataDoc<SAnimationChannelTarget>(*_rData.data.target, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("target", json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SAnimationChannel>& _pData, const JSONCharValue& _JsonValue)
    {
        std::shared_ptr<SAnimationChannel> data_ptr = !!_pData ? _pData : std::make_shared<SAnimationChannel>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SAnimationChannel>>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SAnimationChannel>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SAnimationChannel>>& _vDatas, const JSONCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SAnimationChannel>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SAnimationChannel>>>& _rData, JSONCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SAnimationChannel>>(_rData, _JsonValue);
    }

    bool operator<<(SAnimationChannelTarget& _rData, const JSONCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember("node") && _JsonValue["node"].IsInt())
        {
            if (!(_rData.node << _JsonValue["node"])) return false;
        }
        if (_JsonValue.HasMember("path") && _JsonValue["path"].IsString())
        {
            _rData.path = _JsonValue["path"].GetString();
        }
        _rData.schemaType = "animation.channel.target.schema.json";
        return true;
    }

    bool operator>>(const TDataDoc<SAnimationChannelTarget>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        if (!!_rData.data.node)
        {
            JSONCharValue json_value;
            if (!(TDataDoc<SGlTFId>(*_rData.data.node, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("node", json_value, _rData.doc->GetAllocator());
        }
        {
            JSONCharValue json_value;
            if (!(TDataDoc<std::string>(_rData.data.path, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("path", json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SAnimationChannelTarget>& _pData, const JSONCharValue& _JsonValue)
    {
        std::shared_ptr<SAnimationChannelTarget> data_ptr = !!_pData ? _pData : std::make_shared<SAnimationChannelTarget>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SAnimationChannelTarget>>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SAnimationChannelTarget>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SAnimationChannelTarget>>& _vDatas, const JSONCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SAnimationChannelTarget>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SAnimationChannelTarget>>>& _rData, JSONCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SAnimationChannelTarget>>(_rData, _JsonValue);
    }

    bool operator<<(SAnimationSampler& _rData, const JSONCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember("input") && _JsonValue["input"].IsInt())
        {
            if (!(_rData.input << _JsonValue["input"])) return false;
        }
        if (_JsonValue.HasMember("interpolation") && _JsonValue["interpolation"].IsString())
        {
            _rData.interpolation = _JsonValue["interpolation"].GetString();
        }
        if (_JsonValue.HasMember("output") && _JsonValue["output"].IsInt())
        {
            if (!(_rData.output << _JsonValue["output"])) return false;
        }
        _rData.schemaType = "animation.sampler.schema.json";
        return true;
    }

    bool operator>>(const TDataDoc<SAnimationSampler>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        if (!!_rData.data.input)
        {
            JSONCharValue json_value;
            if (!(TDataDoc<SGlTFId>(*_rData.data.input, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("input", json_value, _rData.doc->GetAllocator());
        }
        {
            JSONCharValue json_value;
            if (!(TDataDoc<std::string>(_rData.data.interpolation, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("interpolation", json_value, _rData.doc->GetAllocator());
        }
        if (!!_rData.data.output)
        {
            JSONCharValue json_value;
            if (!(TDataDoc<SGlTFId>(*_rData.data.output, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("output", json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SAnimationSampler>& _pData, const JSONCharValue& _JsonValue)
    {
        std::shared_ptr<SAnimationSampler> data_ptr = !!_pData ? _pData : std::make_shared<SAnimationSampler>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SAnimationSampler>>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SAnimationSampler>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SAnimationSampler>>& _vDatas, const JSONCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SAnimationSampler>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SAnimationSampler>>>& _rData, JSONCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SAnimationSampler>>(_rData, _JsonValue);
    }

    bool operator<<(SAnimation& _rData, const JSONCharValue& _JsonValue)
    {
        {
            SGlTFChildofRootProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember("channels") && _JsonValue["channels"].IsArray())
        {
            if (!(_rData.channels << _JsonValue["channels"])) return false;
        }
        if (_JsonValue.HasMember("samplers") && _JsonValue["samplers"].IsArray())
        {
            if (!(_rData.samplers << _JsonValue["samplers"])) return false;
        }
        _rData.schemaType = "animation.schema.json";
        return true;
    }

    bool operator>>(const TDataDoc<SAnimation>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFChildofRootProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFChildofRootProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        if (!_rData.data.channels.empty())
        {
            JSONCharValue json_value;
            if (!(TDataDoc<std::vector<std::shared_ptr<SAnimationChannel>>>(_rData.data.channels, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("channels", json_value, _rData.doc->GetAllocator());
        }
        if (!_rData.data.samplers.empty())
        {
            JSONCharValue json_value;
            if (!(TDataDoc<std::vector<std::shared_ptr<SAnimationSampler>>>(_rData.data.samplers, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("samplers", json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SAnimation>& _pData, const JSONCharValue& _JsonValue)
    {
        std::shared_ptr<SAnimation> data_ptr = !!_pData ? _pData : std::make_shared<SAnimation>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SAnimation>>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SAnimation>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SAnimation>>& _vDatas, const JSONCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SAnimation>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SAnimation>>>& _rData, JSONCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SAnimation>>(_rData, _JsonValue);
    }

    bool operator<<(SAsset& _rData, const JSONCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember("copyright") && _JsonValue["copyright"].IsString())
        {
            _rData.copyright = _JsonValue["copyright"].GetString();
        }
        if (_JsonValue.HasMember("generator") && _JsonValue["generator"].IsString())
        {
            _rData.generator = _JsonValue["generator"].GetString();
        }
        if (_JsonValue.HasMember("version") && _JsonValue["version"].IsString())
        {
            _rData.version = _JsonValue["version"].GetString();
        }
        if (_JsonValue.HasMember("minVersion") && _JsonValue["minVersion"].IsString())
        {
            _rData.minVersion = _JsonValue["minVersion"].GetString();
        }
        _rData.schemaType = "asset.schema.json";
        return true;
    }

    bool operator>>(const TDataDoc<SAsset>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        {
            JSONCharValue json_value;
            if (!(TDataDoc<std::string>(_rData.data.copyright, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("copyright", json_value, _rData.doc->GetAllocator());
        }
        {
            JSONCharValue json_value;
            if (!(TDataDoc<std::string>(_rData.data.generator, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("generator", json_value, _rData.doc->GetAllocator());
        }
        {
            JSONCharValue json_value;
            if (!(TDataDoc<std::string>(_rData.data.version, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("version", json_value, _rData.doc->GetAllocator());
        }
        {
            JSONCharValue json_value;
            if (!(TDataDoc<std::string>(_rData.data.minVersion, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("minVersion", json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SAsset>& _pData, const JSONCharValue& _JsonValue)
    {
        std::shared_ptr<SAsset> data_ptr = !!_pData ? _pData : std::make_shared<SAsset>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SAsset>>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SAsset>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SAsset>>& _vDatas, const JSONCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SAsset>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SAsset>>>& _rData, JSONCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SAsset>>(_rData, _JsonValue);
    }

    bool operator<<(SBuffer& _rData, const JSONCharValue& _JsonValue)
    {
        {
            SGlTFChildofRootProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember("uri") && _JsonValue["uri"].IsString())
        {
            _rData.uri = _JsonValue["uri"].GetString();
        }
        if (_JsonValue.HasMember("byteLength") && _JsonValue["byteLength"].IsInt())
        {
            _rData.byteLength = _JsonValue["byteLength"].GetInt();
        }
        _rData.schemaType = "buffer.schema.json";
        return true;
    }

    bool operator>>(const TDataDoc<SBuffer>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFChildofRootProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFChildofRootProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        {
            JSONCharValue json_value;
            if (!(TDataDoc<std::string>(_rData.data.uri, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("uri", json_value, _rData.doc->GetAllocator());
        }
        {
            JSONCharValue json_value;
            if (!(TDataDoc<int32_t>(_rData.data.byteLength, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("byteLength", json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SBuffer>& _pData, const JSONCharValue& _JsonValue)
    {
        std::shared_ptr<SBuffer> data_ptr = !!_pData ? _pData : std::make_shared<SBuffer>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SBuffer>>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SBuffer>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SBuffer>>& _vDatas, const JSONCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SBuffer>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SBuffer>>>& _rData, JSONCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SBuffer>>(_rData, _JsonValue);
    }

    bool operator<<(SBufferView& _rData, const JSONCharValue& _JsonValue)
    {
        {
            SGlTFChildofRootProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember("buffer") && _JsonValue["buffer"].IsInt())
        {
            if (!(_rData.buffer << _JsonValue["buffer"])) return false;
        }
        if (_JsonValue.HasMember("byteOffset") && _JsonValue["byteOffset"].IsInt())
        {
            _rData.byteOffset = _JsonValue["byteOffset"].GetInt();
        }
        if (_JsonValue.HasMember("byteLength") && _JsonValue["byteLength"].IsInt())
        {
            _rData.byteLength = _JsonValue["byteLength"].GetInt();
        }
        if (_JsonValue.HasMember("byteStride") && _JsonValue["byteStride"].IsInt())
        {
            _rData.byteStride = _JsonValue["byteStride"].GetInt();
        }
        if (_JsonValue.HasMember("target") && _JsonValue["target"].IsInt())
        {
            _rData.target = _JsonValue["target"].GetInt();
        }
        _rData.schemaType = "bufferView.schema.json";
        return true;
    }

    bool operator>>(const TDataDoc<SBufferView>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFChildofRootProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFChildofRootProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        if (!!_rData.data.buffer)
        {
            JSONCharValue json_value;
            if (!(TDataDoc<SGlTFId>(*_rData.data.buffer, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("buffer", json_value, _rData.doc->GetAllocator());
        }
        {
            JSONCharValue json_value;
            if (!(TDataDoc<int32_t>(_rData.data.byteOffset, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("byteOffset", json_value, _rData.doc->GetAllocator());
        }
        {
            JSONCharValue json_value;
            if (!(TDataDoc<int32_t>(_rData.data.byteLength, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("byteLength", json_value, _rData.doc->GetAllocator());
        }
        {
            JSONCharValue json_value;
            if (!(TDataDoc<int32_t>(_rData.data.byteStride, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("byteStride", json_value, _rData.doc->GetAllocator());
        }
        {
            JSONCharValue json_value;
            if (!(TDataDoc<int32_t>(_rData.data.target, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("target", json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SBufferView>& _pData, const JSONCharValue& _JsonValue)
    {
        std::shared_ptr<SBufferView> data_ptr = !!_pData ? _pData : std::make_shared<SBufferView>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SBufferView>>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SBufferView>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SBufferView>>& _vDatas, const JSONCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SBufferView>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SBufferView>>>& _rData, JSONCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SBufferView>>(_rData, _JsonValue);
    }

    bool operator<<(SCameraOrthographic& _rData, const JSONCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember("xmag") && _JsonValue["xmag"].IsNumber())
        {
            _rData.xmag = _JsonValue["xmag"].GetFloat();
        }
        if (_JsonValue.HasMember("ymag") && _JsonValue["ymag"].IsNumber())
        {
            _rData.ymag = _JsonValue["ymag"].GetFloat();
        }
        if (_JsonValue.HasMember("zfar") && _JsonValue["zfar"].IsNumber())
        {
            _rData.zfar = _JsonValue["zfar"].GetFloat();
        }
        if (_JsonValue.HasMember("znear") && _JsonValue["znear"].IsNumber())
        {
            _rData.znear = _JsonValue["znear"].GetFloat();
        }
        _rData.schemaType = "camera.orthographic.schema.json";
        return true;
    }

    bool operator>>(const TDataDoc<SCameraOrthographic>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        {
            JSONCharValue json_value;
            if (!(TDataDoc<float>(_rData.data.xmag, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("xmag", json_value, _rData.doc->GetAllocator());
        }
        {
            JSONCharValue json_value;
            if (!(TDataDoc<float>(_rData.data.ymag, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("ymag", json_value, _rData.doc->GetAllocator());
        }
        {
            JSONCharValue json_value;
            if (!(TDataDoc<float>(_rData.data.zfar, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("zfar", json_value, _rData.doc->GetAllocator());
        }
        {
            JSONCharValue json_value;
            if (!(TDataDoc<float>(_rData.data.znear, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("znear", json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SCameraOrthographic>& _pData, const JSONCharValue& _JsonValue)
    {
        std::shared_ptr<SCameraOrthographic> data_ptr = !!_pData ? _pData : std::make_shared<SCameraOrthographic>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SCameraOrthographic>>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SCameraOrthographic>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SCameraOrthographic>>& _vDatas, const JSONCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SCameraOrthographic>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SCameraOrthographic>>>& _rData, JSONCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SCameraOrthographic>>(_rData, _JsonValue);
    }

    bool operator<<(SCameraPerspective& _rData, const JSONCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember("aspectRatio") && _JsonValue["aspectRatio"].IsNumber())
        {
            _rData.aspectRatio = _JsonValue["aspectRatio"].GetFloat();
        }
        if (_JsonValue.HasMember("yfov") && _JsonValue["yfov"].IsNumber())
        {
            _rData.yfov = _JsonValue["yfov"].GetFloat();
        }
        if (_JsonValue.HasMember("zfar") && _JsonValue["zfar"].IsNumber())
        {
            _rData.zfar = _JsonValue["zfar"].GetFloat();
        }
        if (_JsonValue.HasMember("znear") && _JsonValue["znear"].IsNumber())
        {
            _rData.znear = _JsonValue["znear"].GetFloat();
        }
        _rData.schemaType = "camera.perspective.schema.json";
        return true;
    }

    bool operator>>(const TDataDoc<SCameraPerspective>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        {
            JSONCharValue json_value;
            if (!(TDataDoc<float>(_rData.data.aspectRatio, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("aspectRatio", json_value, _rData.doc->GetAllocator());
        }
        {
            JSONCharValue json_value;
            if (!(TDataDoc<float>(_rData.data.yfov, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("yfov", json_value, _rData.doc->GetAllocator());
        }
        {
            JSONCharValue json_value;
            if (!(TDataDoc<float>(_rData.data.zfar, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("zfar", json_value, _rData.doc->GetAllocator());
        }
        {
            JSONCharValue json_value;
            if (!(TDataDoc<float>(_rData.data.znear, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("znear", json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SCameraPerspective>& _pData, const JSONCharValue& _JsonValue)
    {
        std::shared_ptr<SCameraPerspective> data_ptr = !!_pData ? _pData : std::make_shared<SCameraPerspective>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SCameraPerspective>>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SCameraPerspective>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SCameraPerspective>>& _vDatas, const JSONCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SCameraPerspective>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SCameraPerspective>>>& _rData, JSONCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SCameraPerspective>>(_rData, _JsonValue);
    }

    bool operator<<(SCamera& _rData, const JSONCharValue& _JsonValue)
    {
        {
            SGlTFChildofRootProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember("orthographic") && _JsonValue["orthographic"].IsObject())
        {
            if (!(_rData.orthographic << _JsonValue["orthographic"])) return false;
        }
        if (_JsonValue.HasMember("perspective") && _JsonValue["perspective"].IsObject())
        {
            if (!(_rData.perspective << _JsonValue["perspective"])) return false;
        }
        if (_JsonValue.HasMember("type") && _JsonValue["type"].IsString())
        {
            _rData.type = _JsonValue["type"].GetString();
        }
        _rData.schemaType = "camera.schema.json";
        return true;
    }

    bool operator>>(const TDataDoc<SCamera>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFChildofRootProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFChildofRootProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        if (!!_rData.data.orthographic)
        {
            JSONCharValue json_value;
            if (!(TDataDoc<SCameraOrthographic>(*_rData.data.orthographic, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("orthographic", json_value, _rData.doc->GetAllocator());
        }
        if (!!_rData.data.perspective)
        {
            JSONCharValue json_value;
            if (!(TDataDoc<SCameraPerspective>(*_rData.data.perspective, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("perspective", json_value, _rData.doc->GetAllocator());
        }
        {
            JSONCharValue json_value;
            if (!(TDataDoc<std::string>(_rData.data.type, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("type", json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SCamera>& _pData, const JSONCharValue& _JsonValue)
    {
        std::shared_ptr<SCamera> data_ptr = !!_pData ? _pData : std::make_shared<SCamera>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SCamera>>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SCamera>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SCamera>>& _vDatas, const JSONCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SCamera>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SCamera>>>& _rData, JSONCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SCamera>>(_rData, _JsonValue);
    }

    bool operator<<(SExtension& _rData, const JSONCharValue& _JsonValue)
    {
        // Manual code lines
        if (_JsonValue.HasMember("KHR_draco_mesh_compression") && _JsonValue["KHR_draco_mesh_compression"].IsObject())
        {
            std::shared_ptr<SKHR_draco_mesh_compressionextension> extension;
            if (!(extension << _JsonValue["KHR_draco_mesh_compression"])) return false;
            _rData.properties.insert(std::make_pair("KHR_draco_mesh_compression", extension));
        }
        if (_JsonValue.HasMember("KHR_lights_punctual") && _JsonValue["KHR_lights_punctual"].IsObject())
        {
            const JSONCharValue& json_extension = _JsonValue["KHR_lights_punctual"];
            if (json_extension.HasMember("lights"))
            {
                std::shared_ptr<SKHR_lights_punctualglTFextension> extension;
                if (extension << json_extension)
                {
                    _rData.properties.insert(std::make_pair("KHR_lights_punctual", extension));
                }
            }
            if (json_extension.HasMember("light"))
            {
                std::shared_ptr<SKHR_lights_punctualnodeextension> extension;
                if (extension << json_extension)
                {
                    _rData.properties.insert(std::make_pair("KHR_lights_punctual", extension));
                }
            }
        }
        if (_JsonValue.HasMember("KHR_materials_clearcoat") && _JsonValue["KHR_materials_clearcoat"].IsObject())
        {
            std::shared_ptr<SKHR_materials_clearcoatglTFextension> extension;
            if (!(extension << _JsonValue["KHR_materials_clearcoat"])) return false;
            _rData.properties.insert(std::make_pair("KHR_materials_clearcoat", extension));
        }
        if (_JsonValue.HasMember("KHR_materials_emissive_strength") && _JsonValue["KHR_materials_emissive_strength"].IsObject())
        {
            std::shared_ptr<SKHR_materials_emissive_strengthglTFextension> extension;
            if (!(extension << _JsonValue["KHR_materials_emissive_strength"])) return false;
            _rData.properties.insert(std::make_pair("KHR_materials_emissive_strength", extension));
        }
        if (_JsonValue.HasMember("KHR_materials_ior") && _JsonValue["KHR_materials_ior"].IsObject())
        {
            std::shared_ptr<SKHR_materials_iorglTFextension> extension;
            if (!(extension << _JsonValue["KHR_materials_ior"])) return false;
            _rData.properties.insert(std::make_pair("KHR_materials_ior", extension));
        }
        if (_JsonValue.HasMember("KHR_materials_iridescence") && _JsonValue["KHR_materials_iridescence"].IsObject())
        {
            std::shared_ptr<SKHR_materials_iridescenceglTFextension> extension;
            if (!(extension << _JsonValue["KHR_materials_iridescence"])) return false;
            _rData.properties.insert(std::make_pair("KHR_materials_iridescence", extension));
        }
        if (_JsonValue.HasMember("KHR_materials_sheen") && _JsonValue["KHR_materials_sheen"].IsObject())
        {
            std::shared_ptr<SKHR_materials_sheenglTFextension> extension;
            if (!(extension << _JsonValue["KHR_materials_sheen"])) return false;
            _rData.properties.insert(std::make_pair("KHR_materials_sheen", extension));
        }
        if (_JsonValue.HasMember("KHR_materials_specular") && _JsonValue["KHR_materials_specular"].IsObject())
        {
            std::shared_ptr<SKHR_materials_specularglTFextension> extension;
            if (!(extension << _JsonValue["KHR_materials_specular"])) return false;
            _rData.properties.insert(std::make_pair("KHR_materials_specular", extension));
        }
        if (_JsonValue.HasMember("KHR_materials_transmission") && _JsonValue["KHR_materials_transmission"].IsObject())
        {
            std::shared_ptr<SKHR_materials_transmissionglTFextension> extension;
            if (!(extension << _JsonValue["KHR_materials_transmission"])) return false;
            _rData.properties.insert(std::make_pair("KHR_materials_transmission", extension));
        }
        if (_JsonValue.HasMember("KHR_materials_unlit") && _JsonValue["KHR_materials_unlit"].IsObject())
        {
            std::shared_ptr<SKHR_materials_unlitglTFextension> extension;
            if (!(extension << _JsonValue["KHR_materials_unlit"])) return false;
            _rData.properties.insert(std::make_pair("KHR_materials_unlit", extension));
        }
        if (_JsonValue.HasMember("KHR_materials_variants") && _JsonValue["KHR_materials_variants"].IsObject())
        {
            std::shared_ptr<SKHR_materials_variantsglTFextension> extension;
            if (!(extension << _JsonValue["KHR_materials_variants"])) return false;
            _rData.properties.insert(std::make_pair("KHR_materials_variants", extension));
        }
        if (_JsonValue.HasMember("KHR_materials_volume") && _JsonValue["KHR_materials_volume"].IsObject())
        {
            std::shared_ptr<SKHR_materials_volumeglTFextension> extension;
            if (!(extension << _JsonValue["KHR_materials_volume"])) return false;
            _rData.properties.insert(std::make_pair("KHR_materials_volume", extension));
        }
        if (_JsonValue.HasMember("KHR_texture_transform") && _JsonValue["KHR_texture_transform"].IsObject())
        {
            std::shared_ptr<SKHR_texture_transformtextureInfoextension> extension;
            if (!(extension << _JsonValue["KHR_texture_transform"])) return false;
            _rData.properties.insert(std::make_pair("KHR_texture_transform", extension));
        }
        if (_JsonValue.HasMember("ADOBE_materials_thin_transparency") && _JsonValue["ADOBE_materials_thin_transparency"].IsObject())
        {
            std::shared_ptr<SADOBE_materials_thin_transparencyglTFextension> extension;
            if (!(extension << _JsonValue["ADOBE_materials_thin_transparency"])) return false;
            _rData.properties.insert(std::make_pair("ADOBE_materials_thin_transparency", extension));
        }
        if (_JsonValue.HasMember("AGI_articulations") && _JsonValue["AGI_articulations"].IsObject())
        {
            std::shared_ptr<SAGI_articulationsglTFextension> extension;
            if (!(extension << _JsonValue["AGI_articulations"])) return false;
            _rData.properties.insert(std::make_pair("AGI_articulations", extension));
        }
        if (_JsonValue.HasMember("AGI_stk_metadata") && _JsonValue["AGI_stk_metadata"].IsObject())
        {
            std::shared_ptr<SAGI_stk_metadataglTFNodeextension> extension;
            if (!(extension << _JsonValue["AGI_stk_metadata"])) return false;
            _rData.properties.insert(std::make_pair("AGI_stk_metadata", extension));
        }
        if (_JsonValue.HasMember("CESIUM_primitive_outline") && _JsonValue["CESIUM_primitive_outline"].IsObject())
        {
            std::shared_ptr<SCESIUM_primitive_outlineglTFprimitiveextension> extension;
            if (!(extension << _JsonValue["CESIUM_primitive_outline"])) return false;
            _rData.properties.insert(std::make_pair("CESIUM_primitive_outline", extension));
        }
        if (_JsonValue.HasMember("EXT_lights_ies") && _JsonValue["EXT_lights_ies"].IsObject())
        {
            std::shared_ptr<SEXT_lights_iesglTFextension> extension;
            if (!(extension << _JsonValue["EXT_lights_ies"])) return false;
            _rData.properties.insert(std::make_pair("EXT_lights_ies", extension));
        }
        if (_JsonValue.HasMember("EXT_mesh_gpu_instancing") && _JsonValue["EXT_mesh_gpu_instancing"].IsObject())
        {
            std::shared_ptr<SEXT_mesh_gpu_instancingglTFextension> extension;
            if (!(extension << _JsonValue["EXT_mesh_gpu_instancing"])) return false;
            _rData.properties.insert(std::make_pair("EXT_mesh_gpu_instancing", extension));
        }
        if (_JsonValue.HasMember("EXT_texture_webp") && _JsonValue["EXT_texture_webp"].IsObject())
        {
            std::shared_ptr<SEXT_texture_webpglTFextension> extension;
            if (!(extension << _JsonValue["EXT_texture_webp"])) return false;
            _rData.properties.insert(std::make_pair("EXT_texture_webp", extension));
        }
        if (_JsonValue.HasMember("FB_geometry_metadata") && _JsonValue["FB_geometry_metadata"].IsObject())
        {
            std::shared_ptr<SFB_geometry_metadatasceneextension> extension;
            if (!(extension << _JsonValue["FB_geometry_metadata"])) return false;
            _rData.properties.insert(std::make_pair("FB_geometry_metadata", extension));
        }
        if (_JsonValue.HasMember("MSFT_lod") && _JsonValue["MSFT_lod"].IsObject())
        {
            std::shared_ptr<SMSFT_lodglTFextension> extension;
            if (!(extension << _JsonValue["MSFT_lod"])) return false;
            _rData.properties.insert(std::make_pair("MSFT_lod", extension));
        }
        if (_JsonValue.HasMember("MSFT_texture_dds") && _JsonValue["MSFT_texture_dds"].IsObject())
        {
            std::shared_ptr<SMSFT_texture_ddsextension> extension;
            if (!(extension << _JsonValue["MSFT_texture_dds"])) return false;
            _rData.properties.insert(std::make_pair("MSFT_texture_dds", extension));
        }
        _rData.schemaType = "extension.schema.json";
        return true;
    }

    bool operator>>(const TDataDoc<SExtension>& _rData, JSONCharValue& _JsonValue)
    {
        // Manual code lines
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        for (const std::pair<std::string, std::shared_ptr<SObject>>& extensionProperty : _rData.data.properties)
        {
            const std::shared_ptr<SObject>& extensionValue = extensionProperty.second;
            if (!extensionValue) return false;
            if (extensionValue->schemaType == "node.KHR_draco_mesh_compression.schema.json")
            {
                const SKHR_draco_mesh_compressionextension& extension = *((const SKHR_draco_mesh_compressionextension*)extensionValue.get());
                JSONCharValue json_value;
                if (!(TDataDoc<SKHR_draco_mesh_compressionextension>(extension, _rData.doc) >> json_value)) return false;
                _JsonValue.AddMember("KHR_draco_mesh_compression", json_value, _rData.doc->GetAllocator());
            }
            else if (extensionValue->schemaType == "node.KHR_lights_punctual.schema.json")
            {
                const SKHR_lights_punctualnodeextension& extension = *((const SKHR_lights_punctualnodeextension*)extensionValue.get());
                JSONCharValue json_value;
                if (!(TDataDoc<SKHR_lights_punctualnodeextension>(extension, _rData.doc) >> json_value)) return false;
                _JsonValue.AddMember("KHR_lights_punctual", json_value, _rData.doc->GetAllocator());
            }
            else if (extensionValue->schemaType == "glTF.KHR_lights_punctual.schema.json")
            {
                const SKHR_lights_punctualglTFextension& extension = *((const SKHR_lights_punctualglTFextension*)extensionValue.get());
                JSONCharValue json_value;
                if (!(TDataDoc<SKHR_lights_punctualglTFextension>(extension, _rData.doc) >> json_value)) return false;
                _JsonValue.AddMember("KHR_lights_punctual", json_value, _rData.doc->GetAllocator());
            }
            else if (extensionValue->schemaType == "glTF.KHR_materials_clearcoat.schema.json")
            {
                const SKHR_materials_clearcoatglTFextension& extension = *((const SKHR_materials_clearcoatglTFextension*)extensionValue.get());
                JSONCharValue json_value;
                if (!(TDataDoc<SKHR_materials_clearcoatglTFextension>(extension, _rData.doc) >> json_value)) return false;
                _JsonValue.AddMember("KHR_materials_clearcoat", json_value, _rData.doc->GetAllocator());
            }
            else if (extensionValue->schemaType == "glTF.KHR_materials_emissive_strength.schema.json")
            {
                const SKHR_materials_emissive_strengthglTFextension& extension = *((const SKHR_materials_emissive_strengthglTFextension*)extensionValue.get());
                JSONCharValue json_value;
                if (!(TDataDoc<SKHR_materials_emissive_strengthglTFextension>(extension, _rData.doc) >> json_value)) return false;
                _JsonValue.AddMember("KHR_materials_emissive_strength", json_value, _rData.doc->GetAllocator());
            }
            else if (extensionValue->schemaType == "glTF.KHR_materials_ior.schema.json")
            {
                const SKHR_materials_iorglTFextension& extension = *((const SKHR_materials_iorglTFextension*)extensionValue.get());
                JSONCharValue json_value;
                if (!(TDataDoc<SKHR_materials_iorglTFextension>(extension, _rData.doc) >> json_value)) return false;
                _JsonValue.AddMember("KHR_materials_ior", json_value, _rData.doc->GetAllocator());
            }
            else if (extensionValue->schemaType == "glTF.KHR_materials_iridescence.schema.json")
            {
                const SKHR_materials_iridescenceglTFextension& extension = *((const SKHR_materials_iridescenceglTFextension*)extensionValue.get());
                JSONCharValue json_value;
                if (!(TDataDoc<SKHR_materials_iridescenceglTFextension>(extension, _rData.doc) >> json_value)) return false;
                _JsonValue.AddMember("KHR_materials_iridescence", json_value, _rData.doc->GetAllocator());
            }
            else if (extensionValue->schemaType == "glTF.KHR_materials_sheen.schema.json")
            {
                const SKHR_materials_sheenglTFextension& extension = *((const SKHR_materials_sheenglTFextension*)extensionValue.get());
                JSONCharValue json_value;
                if (!(TDataDoc<SKHR_materials_sheenglTFextension>(extension, _rData.doc) >> json_value)) return false;
                _JsonValue.AddMember("KHR_materials_sheen", json_value, _rData.doc->GetAllocator());
            }
            else if (extensionValue->schemaType == "glTF.KHR_materials_specular.schema.json")
            {
                const SKHR_materials_specularglTFextension& extension = *((const SKHR_materials_specularglTFextension*)extensionValue.get());
                JSONCharValue json_value;
                if (!(TDataDoc<SKHR_materials_specularglTFextension>(extension, _rData.doc) >> json_value)) return false;
                _JsonValue.AddMember("KHR_materials_specular", json_value, _rData.doc->GetAllocator());
            }
            else if (extensionValue->schemaType == "glTF.KHR_materials_transmission.schema.json")
            {
                const SKHR_materials_transmissionglTFextension& extension = *((const SKHR_materials_transmissionglTFextension*)extensionValue.get());
                JSONCharValue json_value;
                if (!(TDataDoc<SKHR_materials_transmissionglTFextension>(extension, _rData.doc) >> json_value)) return false;
                _JsonValue.AddMember("KHR_materials_transmission", json_value, _rData.doc->GetAllocator());
            }
            else if (extensionValue->schemaType == "glTF.KHR_materials_unlit.schema.json")
            {
                const SKHR_materials_unlitglTFextension& extension = *((const SKHR_materials_unlitglTFextension*)extensionValue.get());
                JSONCharValue json_value;
                if (!(TDataDoc<SKHR_materials_unlitglTFextension>(extension, _rData.doc) >> json_value)) return false;
                _JsonValue.AddMember("KHR_materials_unlit", json_value, _rData.doc->GetAllocator());
            }
            else if (extensionValue->schemaType == "glTF.KHR_materials_variants.schema.json")
            {
                const SKHR_materials_variantsglTFextension& extension = *((const SKHR_materials_variantsglTFextension*)extensionValue.get());
                JSONCharValue json_value;
                if (!(TDataDoc<SKHR_materials_variantsglTFextension>(extension, _rData.doc) >> json_value)) return false;
                _JsonValue.AddMember("KHR_materials_variants", json_value, _rData.doc->GetAllocator());
            }
            else if (extensionValue->schemaType == "glTF.KHR_materials_volume.schema.json")
            {
                const SKHR_materials_volumeglTFextension& extension = *((const SKHR_materials_volumeglTFextension*)extensionValue.get());
                JSONCharValue json_value;
                if (!(TDataDoc<SKHR_materials_volumeglTFextension>(extension, _rData.doc) >> json_value)) return false;
                _JsonValue.AddMember("KHR_materials_volume", json_value, _rData.doc->GetAllocator());
            }
            else if (extensionValue->schemaType == "glTF.KHR_texture_transform.schema.json")
            {
                const SKHR_texture_transformtextureInfoextension& extension = *((const SKHR_texture_transformtextureInfoextension*)extensionValue.get());
                JSONCharValue json_value;
                if (!(TDataDoc<SKHR_texture_transformtextureInfoextension>(extension, _rData.doc) >> json_value)) return false;
                _JsonValue.AddMember("KHR_texture_transform", json_value, _rData.doc->GetAllocator());
            }
            else if (extensionValue->schemaType == "glTF.ADOBE_materials_thin_transparency.schema.json")
            {
                const SADOBE_materials_thin_transparencyglTFextension& extension = *((const SADOBE_materials_thin_transparencyglTFextension*)extensionValue.get());
                JSONCharValue json_value;
                if (!(TDataDoc<SADOBE_materials_thin_transparencyglTFextension>(extension, _rData.doc) >> json_value)) return false;
                _JsonValue.AddMember("ADOBE_materials_thin_transparency", json_value, _rData.doc->GetAllocator());
            }
            else if (extensionValue->schemaType == "glTF.AGI_articulations.schema.json")
            {
                const SAGI_articulationsglTFextension& extension = *((const SAGI_articulationsglTFextension*)extensionValue.get());
                JSONCharValue json_value;
                if (!(TDataDoc<SAGI_articulationsglTFextension>(extension, _rData.doc) >> json_value)) return false;
                _JsonValue.AddMember("AGI_articulations", json_value, _rData.doc->GetAllocator());
            }
            else if (extensionValue->schemaType == "glTF.AGI_stk_metadata.schema.json")
            {
                const SAGI_stk_metadataglTFextension& extension = *((const SAGI_stk_metadataglTFextension*)extensionValue.get());
                JSONCharValue json_value;
                if (!(TDataDoc<SAGI_stk_metadataglTFextension>(extension, _rData.doc) >> json_value)) return false;
                _JsonValue.AddMember("AGI_stk_metadata", json_value, _rData.doc->GetAllocator());
            }
            else if (extensionValue->schemaType == "glTF.CESIUM_primitive_outline.schema.json")
            {
                const SCESIUM_primitive_outlineglTFprimitiveextension& extension = *((const SCESIUM_primitive_outlineglTFprimitiveextension*)extensionValue.get());
                JSONCharValue json_value;
                if (!(TDataDoc<SCESIUM_primitive_outlineglTFprimitiveextension>(extension, _rData.doc) >> json_value)) return false;
                _JsonValue.AddMember("CESIUM_primitive_outline", json_value, _rData.doc->GetAllocator());
            }
            else if (extensionValue->schemaType == "glTF.EXT_lights_ies.schema.json")
            {
                const SEXT_lights_iesglTFextension& extension = *((const SEXT_lights_iesglTFextension*)extensionValue.get());
                JSONCharValue json_value;
                if (!(TDataDoc<SEXT_lights_iesglTFextension>(extension, _rData.doc) >> json_value)) return false;
                _JsonValue.AddMember("EXT_lights_ies", json_value, _rData.doc->GetAllocator());
            }
            else if (extensionValue->schemaType == "glTF.EXT_mesh_gpu_instancing.schema.json")
            {
                const SEXT_mesh_gpu_instancingglTFextension& extension = *((const SEXT_mesh_gpu_instancingglTFextension*)extensionValue.get());
                JSONCharValue json_value;
                if (!(TDataDoc<SEXT_mesh_gpu_instancingglTFextension>(extension, _rData.doc) >> json_value)) return false;
                _JsonValue.AddMember("EXT_mesh_gpu_instancing", json_value, _rData.doc->GetAllocator());
            }
            else if (extensionValue->schemaType == "glTF.EXT_texture_webp.schema.json")
            {
                const SEXT_texture_webpglTFextension& extension = *((const SEXT_texture_webpglTFextension*)extensionValue.get());
                JSONCharValue json_value;
                if (!(TDataDoc<SEXT_texture_webpglTFextension>(extension, _rData.doc) >> json_value)) return false;
                _JsonValue.AddMember("EXT_texture_webp", json_value, _rData.doc->GetAllocator());
            }
            else if (extensionValue->schemaType == "glTF.FB_geometry_metadata.schema.json")
            {
                const SFB_geometry_metadatasceneextension& extension = *((const SFB_geometry_metadatasceneextension*)extensionValue.get());
                JSONCharValue json_value;
                if (!(TDataDoc<SFB_geometry_metadatasceneextension>(extension, _rData.doc) >> json_value)) return false;
                _JsonValue.AddMember("FB_geometry_metadata", json_value, _rData.doc->GetAllocator());
            }
            else if (extensionValue->schemaType == "glTF.MSFT_lod.schema.json")
            {
                const SMSFT_lodglTFextension& extension = *((const SMSFT_lodglTFextension*)extensionValue.get());
                JSONCharValue json_value;
                if (!(TDataDoc<SMSFT_lodglTFextension>(extension, _rData.doc) >> json_value)) return false;
                _JsonValue.AddMember("MSFT_lod", json_value, _rData.doc->GetAllocator());
            }
            else if (extensionValue->schemaType == "glTF.MSFT_texture_dds.schema.json")
            {
                const SMSFT_texture_ddsextension& extension = *((const SMSFT_texture_ddsextension*)extensionValue.get());
                JSONCharValue json_value;
                if (!(TDataDoc<SMSFT_texture_ddsextension>(extension, _rData.doc) >> json_value)) return false;
                _JsonValue.AddMember("MSFT_texture_dds", json_value, _rData.doc->GetAllocator());
            }
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SExtension>& _pData, const JSONCharValue& _JsonValue)
    {
        std::shared_ptr<SExtension> data_ptr = !!_pData ? _pData : std::make_shared<SExtension>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SExtension>>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SExtension>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SExtension>>& _vDatas, const JSONCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SExtension>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SExtension>>>& _rData, JSONCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SExtension>>(_rData, _JsonValue);
    }

    bool operator<<(SExtras& _rData, const JSONCharValue& _JsonValue)
    {
        _rData.schemaType = "extras.schema.json";
        return true;
    }

    bool operator>>(const TDataDoc<SExtras>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        return true;
    }

    bool operator<<(std::shared_ptr<SExtras>& _pData, const JSONCharValue& _JsonValue)
    {
        std::shared_ptr<SExtras> data_ptr = !!_pData ? _pData : std::make_shared<SExtras>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SExtras>>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SExtras>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SExtras>>& _vDatas, const JSONCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SExtras>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SExtras>>>& _rData, JSONCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SExtras>>(_rData, _JsonValue);
    }

    bool operator<<(SGlTF& _rData, const JSONCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember("extensionsUsed") && _JsonValue["extensionsUsed"].IsArray())
        {
            if (!(_rData.extensionsUsed << _JsonValue["extensionsUsed"])) return false;
        }
        if (_JsonValue.HasMember("extensionsRequired") && _JsonValue["extensionsRequired"].IsArray())
        {
            if (!(_rData.extensionsRequired << _JsonValue["extensionsRequired"])) return false;
        }
        if (_JsonValue.HasMember("accessors") && _JsonValue["accessors"].IsArray())
        {
            if (!(_rData.accessors << _JsonValue["accessors"])) return false;
        }
        if (_JsonValue.HasMember("animations") && _JsonValue["animations"].IsArray())
        {
            if (!(_rData.animations << _JsonValue["animations"])) return false;
        }
        if (_JsonValue.HasMember("asset") && _JsonValue["asset"].IsObject())
        {
            if (!(_rData.asset << _JsonValue["asset"])) return false;
        }
        if (_JsonValue.HasMember("buffers") && _JsonValue["buffers"].IsArray())
        {
            if (!(_rData.buffers << _JsonValue["buffers"])) return false;
        }
        if (_JsonValue.HasMember("bufferViews") && _JsonValue["bufferViews"].IsArray())
        {
            if (!(_rData.bufferViews << _JsonValue["bufferViews"])) return false;
        }
        if (_JsonValue.HasMember("cameras") && _JsonValue["cameras"].IsArray())
        {
            if (!(_rData.cameras << _JsonValue["cameras"])) return false;
        }
        if (_JsonValue.HasMember("images") && _JsonValue["images"].IsArray())
        {
            if (!(_rData.images << _JsonValue["images"])) return false;
        }
        if (_JsonValue.HasMember("materials") && _JsonValue["materials"].IsArray())
        {
            if (!(_rData.materials << _JsonValue["materials"])) return false;
        }
        if (_JsonValue.HasMember("meshes") && _JsonValue["meshes"].IsArray())
        {
            if (!(_rData.meshes << _JsonValue["meshes"])) return false;
        }
        if (_JsonValue.HasMember("nodes") && _JsonValue["nodes"].IsArray())
        {
            if (!(_rData.nodes << _JsonValue["nodes"])) return false;
        }
        if (_JsonValue.HasMember("samplers") && _JsonValue["samplers"].IsArray())
        {
            if (!(_rData.samplers << _JsonValue["samplers"])) return false;
        }
        if (_JsonValue.HasMember("scene") && _JsonValue["scene"].IsInt())
        {
            if (!(_rData.scene << _JsonValue["scene"])) return false;
        }
        if (_JsonValue.HasMember("scenes") && _JsonValue["scenes"].IsArray())
        {
            if (!(_rData.scenes << _JsonValue["scenes"])) return false;
        }
        if (_JsonValue.HasMember("skins") && _JsonValue["skins"].IsArray())
        {
            if (!(_rData.skins << _JsonValue["skins"])) return false;
        }
        if (_JsonValue.HasMember("textures") && _JsonValue["textures"].IsArray())
        {
            if (!(_rData.textures << _JsonValue["textures"])) return false;
        }
        _rData.schemaType = "glTF.schema.json";
        return true;
    }

    bool operator>>(const TDataDoc<SGlTF>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        if (!_rData.data.extensionsUsed.empty())
        {
            JSONCharValue json_value;
            if (!(TDataDoc<std::vector<std::string>>(_rData.data.extensionsUsed, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("extensionsUsed", json_value, _rData.doc->GetAllocator());
        }
        if (!_rData.data.extensionsRequired.empty())
        {
            JSONCharValue json_value;
            if (!(TDataDoc<std::vector<std::string>>(_rData.data.extensionsRequired, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("extensionsRequired", json_value, _rData.doc->GetAllocator());
        }
        if (!_rData.data.accessors.empty())
        {
            JSONCharValue json_value;
            if (!(TDataDoc<std::vector<std::shared_ptr<SAccessor>>>(_rData.data.accessors, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("accessors", json_value, _rData.doc->GetAllocator());
        }
        if (!_rData.data.animations.empty())
        {
            JSONCharValue json_value;
            if (!(TDataDoc<std::vector<std::shared_ptr<SAnimation>>>(_rData.data.animations, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("animations", json_value, _rData.doc->GetAllocator());
        }
        if (!!_rData.data.asset)
        {
            JSONCharValue json_value;
            if (!(TDataDoc<SAsset>(*_rData.data.asset, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("asset", json_value, _rData.doc->GetAllocator());
        }
        if (!_rData.data.buffers.empty())
        {
            JSONCharValue json_value;
            if (!(TDataDoc<std::vector<std::shared_ptr<SBuffer>>>(_rData.data.buffers, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("buffers", json_value, _rData.doc->GetAllocator());
        }
        if (!_rData.data.bufferViews.empty())
        {
            JSONCharValue json_value;
            if (!(TDataDoc<std::vector<std::shared_ptr<SBufferView>>>(_rData.data.bufferViews, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("bufferViews", json_value, _rData.doc->GetAllocator());
        }
        if (!_rData.data.cameras.empty())
        {
            JSONCharValue json_value;
            if (!(TDataDoc<std::vector<std::shared_ptr<SCamera>>>(_rData.data.cameras, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("cameras", json_value, _rData.doc->GetAllocator());
        }
        if (!_rData.data.images.empty())
        {
            JSONCharValue json_value;
            if (!(TDataDoc<std::vector<std::shared_ptr<SImage>>>(_rData.data.images, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("images", json_value, _rData.doc->GetAllocator());
        }
        if (!_rData.data.materials.empty())
        {
            JSONCharValue json_value;
            if (!(TDataDoc<std::vector<std::shared_ptr<SMaterial>>>(_rData.data.materials, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("materials", json_value, _rData.doc->GetAllocator());
        }
        if (!_rData.data.meshes.empty())
        {
            JSONCharValue json_value;
            if (!(TDataDoc<std::vector<std::shared_ptr<SMesh>>>(_rData.data.meshes, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("meshes", json_value, _rData.doc->GetAllocator());
        }
        if (!_rData.data.nodes.empty())
        {
            JSONCharValue json_value;
            if (!(TDataDoc<std::vector<std::shared_ptr<SNode>>>(_rData.data.nodes, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("nodes", json_value, _rData.doc->GetAllocator());
        }
        if (!_rData.data.samplers.empty())
        {
            JSONCharValue json_value;
            if (!(TDataDoc<std::vector<std::shared_ptr<SSampler>>>(_rData.data.samplers, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("samplers", json_value, _rData.doc->GetAllocator());
        }
        if (!!_rData.data.scene)
        {
            JSONCharValue json_value;
            if (!(TDataDoc<SGlTFId>(*_rData.data.scene, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("scene", json_value, _rData.doc->GetAllocator());
        }
        if (!_rData.data.scenes.empty())
        {
            JSONCharValue json_value;
            if (!(TDataDoc<std::vector<std::shared_ptr<SScene>>>(_rData.data.scenes, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("scenes", json_value, _rData.doc->GetAllocator());
        }
        if (!_rData.data.skins.empty())
        {
            JSONCharValue json_value;
            if (!(TDataDoc<std::vector<std::shared_ptr<SSkin>>>(_rData.data.skins, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("skins", json_value, _rData.doc->GetAllocator());
        }
        if (!_rData.data.textures.empty())
        {
            JSONCharValue json_value;
            if (!(TDataDoc<std::vector<std::shared_ptr<STexture>>>(_rData.data.textures, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("textures", json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SGlTF>& _pData, const JSONCharValue& _JsonValue)
    {
        std::shared_ptr<SGlTF> data_ptr = !!_pData ? _pData : std::make_shared<SGlTF>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SGlTF>>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SGlTF>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SGlTF>>& _vDatas, const JSONCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SGlTF>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SGlTF>>>& _rData, JSONCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SGlTF>>(_rData, _JsonValue);
    }

    bool operator<<(SGlTFChildofRootProperty& _rData, const JSONCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember("name") && _JsonValue["name"].IsString())
        {
            _rData.name = _JsonValue["name"].GetString();
        }
        _rData.schemaType = "glTFChildOfRootProperty.schema.json";
        return true;
    }

    bool operator>>(const TDataDoc<SGlTFChildofRootProperty>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        {
            JSONCharValue json_value;
            if (!(TDataDoc<std::string>(_rData.data.name, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("name", json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SGlTFChildofRootProperty>& _pData, const JSONCharValue& _JsonValue)
    {
        std::shared_ptr<SGlTFChildofRootProperty> data_ptr = !!_pData ? _pData : std::make_shared<SGlTFChildofRootProperty>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SGlTFChildofRootProperty>>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SGlTFChildofRootProperty>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SGlTFChildofRootProperty>>& _vDatas, const JSONCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SGlTFChildofRootProperty>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SGlTFChildofRootProperty>>>& _rData, JSONCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SGlTFChildofRootProperty>>(_rData, _JsonValue);
    }

    bool operator<<(SGlTFId& _rData, const JSONCharValue& _JsonValue)
    {
        if (!(_rData.int32_tValue << _JsonValue)) return false;
        _rData.schemaType = "glTFid.schema.json";
        return true;
    }

    bool operator>>(const TDataDoc<SGlTFId>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        if (!(TDataDoc<int32_t>(_rData.data.int32_tValue, _rData.doc) >> _JsonValue)) return false;
        return true;
    }

    bool operator<<(std::shared_ptr<SGlTFId>& _pData, const JSONCharValue& _JsonValue)
    {
        std::shared_ptr<SGlTFId> data_ptr = !!_pData ? _pData : std::make_shared<SGlTFId>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SGlTFId>>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SGlTFId>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SGlTFId>>& _vDatas, const JSONCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SGlTFId>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SGlTFId>>>& _rData, JSONCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SGlTFId>>(_rData, _JsonValue);
    }

    bool operator<<(SGlTFProperty& _rData, const JSONCharValue& _JsonValue)
    {
        if (_JsonValue.HasMember("extensions") && _JsonValue["extensions"].IsObject())
        {
            if (!(_rData.extensions << _JsonValue["extensions"])) return false;
        }
        if (_JsonValue.HasMember("extras") && _JsonValue["extras"].IsObject())
        {
            if (!(_rData.extras << _JsonValue["extras"])) return false;
        }
        _rData.schemaType = "glTFProperty.schema.json";
        return true;
    }

    bool operator>>(const TDataDoc<SGlTFProperty>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        if (!!_rData.data.extensions)
        {
            JSONCharValue json_value;
            if (!(TDataDoc<SExtension>(*_rData.data.extensions, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("extensions", json_value, _rData.doc->GetAllocator());
        }
        if (!!_rData.data.extras)
        {
            JSONCharValue json_value;
            if (!(TDataDoc<SExtras>(*_rData.data.extras, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("extras", json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SGlTFProperty>& _pData, const JSONCharValue& _JsonValue)
    {
        std::shared_ptr<SGlTFProperty> data_ptr = !!_pData ? _pData : std::make_shared<SGlTFProperty>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SGlTFProperty>>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SGlTFProperty>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SGlTFProperty>>& _vDatas, const JSONCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SGlTFProperty>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SGlTFProperty>>>& _rData, JSONCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SGlTFProperty>>(_rData, _JsonValue);
    }

    bool operator<<(SImage& _rData, const JSONCharValue& _JsonValue)
    {
        {
            SGlTFChildofRootProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember("uri") && _JsonValue["uri"].IsString())
        {
            _rData.uri = _JsonValue["uri"].GetString();
        }
        if (_JsonValue.HasMember("mimeType") && _JsonValue["mimeType"].IsString())
        {
            _rData.mimeType = _JsonValue["mimeType"].GetString();
        }
        if (_JsonValue.HasMember("bufferView") && _JsonValue["bufferView"].IsInt())
        {
            if (!(_rData.bufferView << _JsonValue["bufferView"])) return false;
        }
        _rData.schemaType = "image.schema.json";
        return true;
    }

    bool operator>>(const TDataDoc<SImage>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFChildofRootProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFChildofRootProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        {
            JSONCharValue json_value;
            if (!(TDataDoc<std::string>(_rData.data.uri, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("uri", json_value, _rData.doc->GetAllocator());
        }
        {
            JSONCharValue json_value;
            if (!(TDataDoc<std::string>(_rData.data.mimeType, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("mimeType", json_value, _rData.doc->GetAllocator());
        }
        if (!!_rData.data.bufferView)
        {
            JSONCharValue json_value;
            if (!(TDataDoc<SGlTFId>(*_rData.data.bufferView, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("bufferView", json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SImage>& _pData, const JSONCharValue& _JsonValue)
    {
        std::shared_ptr<SImage> data_ptr = !!_pData ? _pData : std::make_shared<SImage>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SImage>>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SImage>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SImage>>& _vDatas, const JSONCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SImage>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SImage>>>& _rData, JSONCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SImage>>(_rData, _JsonValue);
    }

    bool operator<<(SMaterialNormalTextureInfo& _rData, const JSONCharValue& _JsonValue)
    {
        {
            STextureInfo& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember("scale") && _JsonValue["scale"].IsNumber())
        {
            _rData.scale = _JsonValue["scale"].GetFloat();
        }
        _rData.schemaType = "material.normalTextureInfo.schema.json";
        return true;
    }

    bool operator>>(const TDataDoc<SMaterialNormalTextureInfo>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const STextureInfo& super_ptr = _rData.data;
            if (!(TDataDoc<STextureInfo>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        {
            JSONCharValue json_value;
            if (!(TDataDoc<float>(_rData.data.scale, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("scale", json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SMaterialNormalTextureInfo>& _pData, const JSONCharValue& _JsonValue)
    {
        std::shared_ptr<SMaterialNormalTextureInfo> data_ptr = !!_pData ? _pData : std::make_shared<SMaterialNormalTextureInfo>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SMaterialNormalTextureInfo>>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SMaterialNormalTextureInfo>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SMaterialNormalTextureInfo>>& _vDatas, const JSONCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SMaterialNormalTextureInfo>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SMaterialNormalTextureInfo>>>& _rData, JSONCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SMaterialNormalTextureInfo>>(_rData, _JsonValue);
    }

    bool operator<<(SMaterialOcclusionTextureInfo& _rData, const JSONCharValue& _JsonValue)
    {
        {
            STextureInfo& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember("strength") && _JsonValue["strength"].IsNumber())
        {
            _rData.strength = _JsonValue["strength"].GetFloat();
        }
        _rData.schemaType = "material.occlusionTextureInfo.schema.json";
        return true;
    }

    bool operator>>(const TDataDoc<SMaterialOcclusionTextureInfo>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const STextureInfo& super_ptr = _rData.data;
            if (!(TDataDoc<STextureInfo>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        {
            JSONCharValue json_value;
            if (!(TDataDoc<float>(_rData.data.strength, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("strength", json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SMaterialOcclusionTextureInfo>& _pData, const JSONCharValue& _JsonValue)
    {
        std::shared_ptr<SMaterialOcclusionTextureInfo> data_ptr = !!_pData ? _pData : std::make_shared<SMaterialOcclusionTextureInfo>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SMaterialOcclusionTextureInfo>>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SMaterialOcclusionTextureInfo>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SMaterialOcclusionTextureInfo>>& _vDatas, const JSONCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SMaterialOcclusionTextureInfo>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SMaterialOcclusionTextureInfo>>>& _rData, JSONCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SMaterialOcclusionTextureInfo>>(_rData, _JsonValue);
    }

    bool operator<<(SMaterialPBRMetallicRoughness& _rData, const JSONCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember("baseColorFactor") && _JsonValue["baseColorFactor"].IsArray())
        {
            if (!(_rData.baseColorFactor << _JsonValue["baseColorFactor"])) return false;
        }
        if (_JsonValue.HasMember("baseColorTexture") && _JsonValue["baseColorTexture"].IsObject())
        {
            if (!(_rData.baseColorTexture << _JsonValue["baseColorTexture"])) return false;
        }
        if (_JsonValue.HasMember("metallicFactor") && _JsonValue["metallicFactor"].IsNumber())
        {
            _rData.metallicFactor = _JsonValue["metallicFactor"].GetFloat();
        }
        if (_JsonValue.HasMember("roughnessFactor") && _JsonValue["roughnessFactor"].IsNumber())
        {
            _rData.roughnessFactor = _JsonValue["roughnessFactor"].GetFloat();
        }
        if (_JsonValue.HasMember("metallicRoughnessTexture") && _JsonValue["metallicRoughnessTexture"].IsObject())
        {
            if (!(_rData.metallicRoughnessTexture << _JsonValue["metallicRoughnessTexture"])) return false;
        }
        _rData.schemaType = "material.pbrMetallicRoughness.schema.json";
        return true;
    }

    bool operator>>(const TDataDoc<SMaterialPBRMetallicRoughness>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        if (!_rData.data.baseColorFactor.empty())
        {
            JSONCharValue json_value;
            if (!(TDataDoc<std::vector<float>>(_rData.data.baseColorFactor, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("baseColorFactor", json_value, _rData.doc->GetAllocator());
        }
        if (!!_rData.data.baseColorTexture)
        {
            JSONCharValue json_value;
            if (!(TDataDoc<STextureInfo>(*_rData.data.baseColorTexture, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("baseColorTexture", json_value, _rData.doc->GetAllocator());
        }
        {
            JSONCharValue json_value;
            if (!(TDataDoc<float>(_rData.data.metallicFactor, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("metallicFactor", json_value, _rData.doc->GetAllocator());
        }
        {
            JSONCharValue json_value;
            if (!(TDataDoc<float>(_rData.data.roughnessFactor, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("roughnessFactor", json_value, _rData.doc->GetAllocator());
        }
        if (!!_rData.data.metallicRoughnessTexture)
        {
            JSONCharValue json_value;
            if (!(TDataDoc<STextureInfo>(*_rData.data.metallicRoughnessTexture, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("metallicRoughnessTexture", json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SMaterialPBRMetallicRoughness>& _pData, const JSONCharValue& _JsonValue)
    {
        std::shared_ptr<SMaterialPBRMetallicRoughness> data_ptr = !!_pData ? _pData : std::make_shared<SMaterialPBRMetallicRoughness>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SMaterialPBRMetallicRoughness>>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SMaterialPBRMetallicRoughness>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SMaterialPBRMetallicRoughness>>& _vDatas, const JSONCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SMaterialPBRMetallicRoughness>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SMaterialPBRMetallicRoughness>>>& _rData, JSONCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SMaterialPBRMetallicRoughness>>(_rData, _JsonValue);
    }

    bool operator<<(SMaterial& _rData, const JSONCharValue& _JsonValue)
    {
        {
            SGlTFChildofRootProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember("pbrMetallicRoughness") && _JsonValue["pbrMetallicRoughness"].IsObject())
        {
            if (!(_rData.pbrMetallicRoughness << _JsonValue["pbrMetallicRoughness"])) return false;
        }
        if (_JsonValue.HasMember("normalTexture") && _JsonValue["normalTexture"].IsObject())
        {
            if (!(_rData.normalTexture << _JsonValue["normalTexture"])) return false;
        }
        if (_JsonValue.HasMember("occlusionTexture") && _JsonValue["occlusionTexture"].IsObject())
        {
            if (!(_rData.occlusionTexture << _JsonValue["occlusionTexture"])) return false;
        }
        if (_JsonValue.HasMember("emissiveTexture") && _JsonValue["emissiveTexture"].IsObject())
        {
            if (!(_rData.emissiveTexture << _JsonValue["emissiveTexture"])) return false;
        }
        if (_JsonValue.HasMember("emissiveFactor") && _JsonValue["emissiveFactor"].IsArray())
        {
            if (!(_rData.emissiveFactor << _JsonValue["emissiveFactor"])) return false;
        }
        if (_JsonValue.HasMember("alphaMode") && _JsonValue["alphaMode"].IsString())
        {
            _rData.alphaMode = _JsonValue["alphaMode"].GetString();
        }
        if (_JsonValue.HasMember("alphaCutoff") && _JsonValue["alphaCutoff"].IsNumber())
        {
            _rData.alphaCutoff = _JsonValue["alphaCutoff"].GetFloat();
        }
        if (_JsonValue.HasMember("doubleSided") && _JsonValue["doubleSided"].IsBool())
        {
            _rData.doubleSided = _JsonValue["doubleSided"].GetBool();
        }
        _rData.schemaType = "material.schema.json";
        return true;
    }

    bool operator>>(const TDataDoc<SMaterial>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFChildofRootProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFChildofRootProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        if (!!_rData.data.pbrMetallicRoughness)
        {
            JSONCharValue json_value;
            if (!(TDataDoc<SMaterialPBRMetallicRoughness>(*_rData.data.pbrMetallicRoughness, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("pbrMetallicRoughness", json_value, _rData.doc->GetAllocator());
        }
        if (!!_rData.data.normalTexture)
        {
            JSONCharValue json_value;
            if (!(TDataDoc<SMaterialNormalTextureInfo>(*_rData.data.normalTexture, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("normalTexture", json_value, _rData.doc->GetAllocator());
        }
        if (!!_rData.data.occlusionTexture)
        {
            JSONCharValue json_value;
            if (!(TDataDoc<SMaterialOcclusionTextureInfo>(*_rData.data.occlusionTexture, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("occlusionTexture", json_value, _rData.doc->GetAllocator());
        }
        if (!!_rData.data.emissiveTexture)
        {
            JSONCharValue json_value;
            if (!(TDataDoc<STextureInfo>(*_rData.data.emissiveTexture, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("emissiveTexture", json_value, _rData.doc->GetAllocator());
        }
        if (!_rData.data.emissiveFactor.empty())
        {
            JSONCharValue json_value;
            if (!(TDataDoc<std::vector<float>>(_rData.data.emissiveFactor, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("emissiveFactor", json_value, _rData.doc->GetAllocator());
        }
        {
            JSONCharValue json_value;
            if (!(TDataDoc<std::string>(_rData.data.alphaMode, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("alphaMode", json_value, _rData.doc->GetAllocator());
        }
        {
            JSONCharValue json_value;
            if (!(TDataDoc<float>(_rData.data.alphaCutoff, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("alphaCutoff", json_value, _rData.doc->GetAllocator());
        }
        {
            JSONCharValue json_value;
            if (!(TDataDoc<bool>(_rData.data.doubleSided, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("doubleSided", json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SMaterial>& _pData, const JSONCharValue& _JsonValue)
    {
        std::shared_ptr<SMaterial> data_ptr = !!_pData ? _pData : std::make_shared<SMaterial>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SMaterial>>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SMaterial>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SMaterial>>& _vDatas, const JSONCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SMaterial>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SMaterial>>>& _rData, JSONCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SMaterial>>(_rData, _JsonValue);
    }

    bool operator<<(SMeshPrimitive& _rData, const JSONCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember("attributes") && _JsonValue["attributes"].IsObject())
        {
            if (!(_rData.attributes << _JsonValue["attributes"])) return false;
        }
        if (_JsonValue.HasMember("indices") && _JsonValue["indices"].IsInt())
        {
            if (!(_rData.indices << _JsonValue["indices"])) return false;
        }
        if (_JsonValue.HasMember("material") && _JsonValue["material"].IsInt())
        {
            if (!(_rData.material << _JsonValue["material"])) return false;
        }
        if (_JsonValue.HasMember("mode") && _JsonValue["mode"].IsInt())
        {
            _rData.mode = _JsonValue["mode"].GetInt();
        }
        if (_JsonValue.HasMember("targets") && _JsonValue["targets"].IsArray())
        {
            if (!(_rData.targets << _JsonValue["targets"])) return false;
        }
        _rData.schemaType = "mesh.primitive.schema.json";
        return true;
    }

    bool operator>>(const TDataDoc<SMeshPrimitive>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        if (!_rData.data.attributes.empty())
        {
            JSONCharValue json_value;
            if (!(TDataDoc<std::map<std::string, std::shared_ptr<SGlTFId>>>(_rData.data.attributes, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("attributes", json_value, _rData.doc->GetAllocator());
        }
        if (!!_rData.data.indices)
        {
            JSONCharValue json_value;
            if (!(TDataDoc<SGlTFId>(*_rData.data.indices, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("indices", json_value, _rData.doc->GetAllocator());
        }
        if (!!_rData.data.material)
        {
            JSONCharValue json_value;
            if (!(TDataDoc<SGlTFId>(*_rData.data.material, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("material", json_value, _rData.doc->GetAllocator());
        }
        {
            JSONCharValue json_value;
            if (!(TDataDoc<int32_t>(_rData.data.mode, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("mode", json_value, _rData.doc->GetAllocator());
        }
        if (!_rData.data.targets.empty())
        {
            JSONCharValue json_value;
            if (!(TDataDoc<std::vector<std::map<std::string, std::shared_ptr<SGlTFId>>>>(_rData.data.targets, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("targets", json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SMeshPrimitive>& _pData, const JSONCharValue& _JsonValue)
    {
        std::shared_ptr<SMeshPrimitive> data_ptr = !!_pData ? _pData : std::make_shared<SMeshPrimitive>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SMeshPrimitive>>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SMeshPrimitive>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SMeshPrimitive>>& _vDatas, const JSONCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SMeshPrimitive>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SMeshPrimitive>>>& _rData, JSONCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SMeshPrimitive>>(_rData, _JsonValue);
    }

    bool operator<<(SMesh& _rData, const JSONCharValue& _JsonValue)
    {
        {
            SGlTFChildofRootProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember("primitives") && _JsonValue["primitives"].IsArray())
        {
            if (!(_rData.primitives << _JsonValue["primitives"])) return false;
        }
        if (_JsonValue.HasMember("weights") && _JsonValue["weights"].IsArray())
        {
            if (!(_rData.weights << _JsonValue["weights"])) return false;
        }
        _rData.schemaType = "mesh.schema.json";
        return true;
    }

    bool operator>>(const TDataDoc<SMesh>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFChildofRootProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFChildofRootProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        if (!_rData.data.primitives.empty())
        {
            JSONCharValue json_value;
            if (!(TDataDoc<std::vector<std::shared_ptr<SMeshPrimitive>>>(_rData.data.primitives, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("primitives", json_value, _rData.doc->GetAllocator());
        }
        if (!_rData.data.weights.empty())
        {
            JSONCharValue json_value;
            if (!(TDataDoc<std::vector<float>>(_rData.data.weights, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("weights", json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SMesh>& _pData, const JSONCharValue& _JsonValue)
    {
        std::shared_ptr<SMesh> data_ptr = !!_pData ? _pData : std::make_shared<SMesh>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SMesh>>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SMesh>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SMesh>>& _vDatas, const JSONCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SMesh>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SMesh>>>& _rData, JSONCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SMesh>>(_rData, _JsonValue);
    }

    bool operator<<(SNode& _rData, const JSONCharValue& _JsonValue)
    {
        {
            SGlTFChildofRootProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember("camera") && _JsonValue["camera"].IsInt())
        {
            if (!(_rData.camera << _JsonValue["camera"])) return false;
        }
        if (_JsonValue.HasMember("children") && _JsonValue["children"].IsArray())
        {
            if (!(_rData.children << _JsonValue["children"])) return false;
        }
        if (_JsonValue.HasMember("skin") && _JsonValue["skin"].IsInt())
        {
            if (!(_rData.skin << _JsonValue["skin"])) return false;
        }
        if (_JsonValue.HasMember("matrix") && _JsonValue["matrix"].IsArray())
        {
            if (!(_rData.matrix << _JsonValue["matrix"])) return false;
        }
        if (_JsonValue.HasMember("mesh") && _JsonValue["mesh"].IsInt())
        {
            if (!(_rData.mesh << _JsonValue["mesh"])) return false;
        }
        if (_JsonValue.HasMember("rotation") && _JsonValue["rotation"].IsArray())
        {
            if (!(_rData.rotation << _JsonValue["rotation"])) return false;
        }
        if (_JsonValue.HasMember("scale") && _JsonValue["scale"].IsArray())
        {
            if (!(_rData.scale << _JsonValue["scale"])) return false;
        }
        if (_JsonValue.HasMember("translation") && _JsonValue["translation"].IsArray())
        {
            if (!(_rData.translation << _JsonValue["translation"])) return false;
        }
        if (_JsonValue.HasMember("weights") && _JsonValue["weights"].IsArray())
        {
            if (!(_rData.weights << _JsonValue["weights"])) return false;
        }
        _rData.schemaType = "node.schema.json";
        return true;
    }

    bool operator>>(const TDataDoc<SNode>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFChildofRootProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFChildofRootProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        if (!!_rData.data.camera)
        {
            JSONCharValue json_value;
            if (!(TDataDoc<SGlTFId>(*_rData.data.camera, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("camera", json_value, _rData.doc->GetAllocator());
        }
        if (!_rData.data.children.empty())
        {
            JSONCharValue json_value;
            if (!(TDataDoc<std::vector<std::shared_ptr<SGlTFId>>>(_rData.data.children, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("children", json_value, _rData.doc->GetAllocator());
        }
        if (!!_rData.data.skin)
        {
            JSONCharValue json_value;
            if (!(TDataDoc<SGlTFId>(*_rData.data.skin, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("skin", json_value, _rData.doc->GetAllocator());
        }
        if (!_rData.data.matrix.empty())
        {
            JSONCharValue json_value;
            if (!(TDataDoc<std::vector<float>>(_rData.data.matrix, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("matrix", json_value, _rData.doc->GetAllocator());
        }
        if (!!_rData.data.mesh)
        {
            JSONCharValue json_value;
            if (!(TDataDoc<SGlTFId>(*_rData.data.mesh, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("mesh", json_value, _rData.doc->GetAllocator());
        }
        if (!_rData.data.rotation.empty())
        {
            JSONCharValue json_value;
            if (!(TDataDoc<std::vector<float>>(_rData.data.rotation, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("rotation", json_value, _rData.doc->GetAllocator());
        }
        if (!_rData.data.scale.empty())
        {
            JSONCharValue json_value;
            if (!(TDataDoc<std::vector<float>>(_rData.data.scale, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("scale", json_value, _rData.doc->GetAllocator());
        }
        if (!_rData.data.translation.empty())
        {
            JSONCharValue json_value;
            if (!(TDataDoc<std::vector<float>>(_rData.data.translation, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("translation", json_value, _rData.doc->GetAllocator());
        }
        if (!_rData.data.weights.empty())
        {
            JSONCharValue json_value;
            if (!(TDataDoc<std::vector<float>>(_rData.data.weights, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("weights", json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SNode>& _pData, const JSONCharValue& _JsonValue)
    {
        std::shared_ptr<SNode> data_ptr = !!_pData ? _pData : std::make_shared<SNode>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SNode>>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SNode>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SNode>>& _vDatas, const JSONCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SNode>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SNode>>>& _rData, JSONCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SNode>>(_rData, _JsonValue);
    }

    bool operator<<(SSampler& _rData, const JSONCharValue& _JsonValue)
    {
        {
            SGlTFChildofRootProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember("magFilter") && _JsonValue["magFilter"].IsInt())
        {
            _rData.magFilter = _JsonValue["magFilter"].GetInt();
        }
        if (_JsonValue.HasMember("minFilter") && _JsonValue["minFilter"].IsInt())
        {
            _rData.minFilter = _JsonValue["minFilter"].GetInt();
        }
        if (_JsonValue.HasMember("wrapS") && _JsonValue["wrapS"].IsInt())
        {
            _rData.wrapS = _JsonValue["wrapS"].GetInt();
        }
        if (_JsonValue.HasMember("wrapT") && _JsonValue["wrapT"].IsInt())
        {
            _rData.wrapT = _JsonValue["wrapT"].GetInt();
        }
        _rData.schemaType = "sampler.schema.json";
        return true;
    }

    bool operator>>(const TDataDoc<SSampler>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFChildofRootProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFChildofRootProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        {
            JSONCharValue json_value;
            if (!(TDataDoc<int32_t>(_rData.data.magFilter, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("magFilter", json_value, _rData.doc->GetAllocator());
        }
        {
            JSONCharValue json_value;
            if (!(TDataDoc<int32_t>(_rData.data.minFilter, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("minFilter", json_value, _rData.doc->GetAllocator());
        }
        {
            JSONCharValue json_value;
            if (!(TDataDoc<int32_t>(_rData.data.wrapS, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("wrapS", json_value, _rData.doc->GetAllocator());
        }
        {
            JSONCharValue json_value;
            if (!(TDataDoc<int32_t>(_rData.data.wrapT, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("wrapT", json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SSampler>& _pData, const JSONCharValue& _JsonValue)
    {
        std::shared_ptr<SSampler> data_ptr = !!_pData ? _pData : std::make_shared<SSampler>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SSampler>>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SSampler>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SSampler>>& _vDatas, const JSONCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SSampler>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SSampler>>>& _rData, JSONCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SSampler>>(_rData, _JsonValue);
    }

    bool operator<<(SScene& _rData, const JSONCharValue& _JsonValue)
    {
        {
            SGlTFChildofRootProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember("nodes") && _JsonValue["nodes"].IsArray())
        {
            if (!(_rData.nodes << _JsonValue["nodes"])) return false;
        }
        _rData.schemaType = "scene.schema.json";
        return true;
    }

    bool operator>>(const TDataDoc<SScene>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFChildofRootProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFChildofRootProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        if (!_rData.data.nodes.empty())
        {
            JSONCharValue json_value;
            if (!(TDataDoc<std::vector<std::shared_ptr<SGlTFId>>>(_rData.data.nodes, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("nodes", json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SScene>& _pData, const JSONCharValue& _JsonValue)
    {
        std::shared_ptr<SScene> data_ptr = !!_pData ? _pData : std::make_shared<SScene>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SScene>>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SScene>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SScene>>& _vDatas, const JSONCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SScene>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SScene>>>& _rData, JSONCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SScene>>(_rData, _JsonValue);
    }

    bool operator<<(SSkin& _rData, const JSONCharValue& _JsonValue)
    {
        {
            SGlTFChildofRootProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember("inverseBindMatrices") && _JsonValue["inverseBindMatrices"].IsInt())
        {
            if (!(_rData.inverseBindMatrices << _JsonValue["inverseBindMatrices"])) return false;
        }
        if (_JsonValue.HasMember("skeleton") && _JsonValue["skeleton"].IsInt())
        {
            if (!(_rData.skeleton << _JsonValue["skeleton"])) return false;
        }
        if (_JsonValue.HasMember("joints") && _JsonValue["joints"].IsArray())
        {
            if (!(_rData.joints << _JsonValue["joints"])) return false;
        }
        _rData.schemaType = "skin.schema.json";
        return true;
    }

    bool operator>>(const TDataDoc<SSkin>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFChildofRootProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFChildofRootProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        if (!!_rData.data.inverseBindMatrices)
        {
            JSONCharValue json_value;
            if (!(TDataDoc<SGlTFId>(*_rData.data.inverseBindMatrices, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("inverseBindMatrices", json_value, _rData.doc->GetAllocator());
        }
        if (!!_rData.data.skeleton)
        {
            JSONCharValue json_value;
            if (!(TDataDoc<SGlTFId>(*_rData.data.skeleton, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("skeleton", json_value, _rData.doc->GetAllocator());
        }
        if (!_rData.data.joints.empty())
        {
            JSONCharValue json_value;
            if (!(TDataDoc<std::vector<std::shared_ptr<SGlTFId>>>(_rData.data.joints, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("joints", json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SSkin>& _pData, const JSONCharValue& _JsonValue)
    {
        std::shared_ptr<SSkin> data_ptr = !!_pData ? _pData : std::make_shared<SSkin>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SSkin>>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SSkin>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SSkin>>& _vDatas, const JSONCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SSkin>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SSkin>>>& _rData, JSONCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SSkin>>(_rData, _JsonValue);
    }

    bool operator<<(STexture& _rData, const JSONCharValue& _JsonValue)
    {
        {
            SGlTFChildofRootProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember("sampler") && _JsonValue["sampler"].IsInt())
        {
            if (!(_rData.sampler << _JsonValue["sampler"])) return false;
        }
        if (_JsonValue.HasMember("source") && _JsonValue["source"].IsInt())
        {
            if (!(_rData.source << _JsonValue["source"])) return false;
        }
        _rData.schemaType = "texture.schema.json";
        return true;
    }

    bool operator>>(const TDataDoc<STexture>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFChildofRootProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFChildofRootProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        if (!!_rData.data.sampler)
        {
            JSONCharValue json_value;
            if (!(TDataDoc<SGlTFId>(*_rData.data.sampler, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("sampler", json_value, _rData.doc->GetAllocator());
        }
        if (!!_rData.data.source)
        {
            JSONCharValue json_value;
            if (!(TDataDoc<SGlTFId>(*_rData.data.source, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("source", json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<STexture>& _pData, const JSONCharValue& _JsonValue)
    {
        std::shared_ptr<STexture> data_ptr = !!_pData ? _pData : std::make_shared<STexture>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<STexture>>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<STexture>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<STexture>>& _vDatas, const JSONCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<STexture>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<STexture>>>& _rData, JSONCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<STexture>>(_rData, _JsonValue);
    }

    bool operator<<(STextureInfo& _rData, const JSONCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember("index") && _JsonValue["index"].IsInt())
        {
            if (!(_rData.index << _JsonValue["index"])) return false;
        }
        if (_JsonValue.HasMember("texCoord") && _JsonValue["texCoord"].IsInt())
        {
            _rData.texCoord = _JsonValue["texCoord"].GetInt();
        }
        _rData.schemaType = "textureInfo.schema.json";
        return true;
    }

    bool operator>>(const TDataDoc<STextureInfo>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        if (!!_rData.data.index)
        {
            JSONCharValue json_value;
            if (!(TDataDoc<SGlTFId>(*_rData.data.index, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("index", json_value, _rData.doc->GetAllocator());
        }
        {
            JSONCharValue json_value;
            if (!(TDataDoc<int32_t>(_rData.data.texCoord, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("texCoord", json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<STextureInfo>& _pData, const JSONCharValue& _JsonValue)
    {
        std::shared_ptr<STextureInfo> data_ptr = !!_pData ? _pData : std::make_shared<STextureInfo>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<STextureInfo>>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<STextureInfo>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<STextureInfo>>& _vDatas, const JSONCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<STextureInfo>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<STextureInfo>>>& _rData, JSONCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<STextureInfo>>(_rData, _JsonValue);
    }

    bool operator<<(SKHR_draco_mesh_compressionextension& _rData, const JSONCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember("bufferView") && _JsonValue["bufferView"].IsInt())
        {
            if (!(_rData.bufferView << _JsonValue["bufferView"])) return false;
        }
        if (_JsonValue.HasMember("attributes") && _JsonValue["attributes"].IsObject())
        {
            if (!(_rData.attributes << _JsonValue["attributes"])) return false;
        }
        _rData.schemaType = "mesh.primitive.KHR_draco_mesh_compression.schema.json";
        return true;
    }

    bool operator>>(const TDataDoc<SKHR_draco_mesh_compressionextension>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        if (!!_rData.data.bufferView)
        {
            JSONCharValue json_value;
            if (!(TDataDoc<SGlTFId>(*_rData.data.bufferView, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("bufferView", json_value, _rData.doc->GetAllocator());
        }
        if (!_rData.data.attributes.empty())
        {
            JSONCharValue json_value;
            if (!(TDataDoc<std::map<std::string, std::shared_ptr<SGlTFId>>>(_rData.data.attributes, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("attributes", json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SKHR_draco_mesh_compressionextension>& _pData, const JSONCharValue& _JsonValue)
    {
        std::shared_ptr<SKHR_draco_mesh_compressionextension> data_ptr = !!_pData ? _pData : std::make_shared<SKHR_draco_mesh_compressionextension>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SKHR_draco_mesh_compressionextension>>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SKHR_draco_mesh_compressionextension>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SKHR_draco_mesh_compressionextension>>& _vDatas, const JSONCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SKHR_draco_mesh_compressionextension>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SKHR_draco_mesh_compressionextension>>>& _rData, JSONCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SKHR_draco_mesh_compressionextension>>(_rData, _JsonValue);
    }

    bool operator<<(SKHR_lights_punctualglTFextension& _rData, const JSONCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember("lights") && _JsonValue["lights"].IsArray())
        {
            if (!(_rData.lights << _JsonValue["lights"])) return false;
        }
        _rData.schemaType = "glTF.KHR_lights_punctual.schema.json";
        return true;
    }

    bool operator>>(const TDataDoc<SKHR_lights_punctualglTFextension>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        if (!_rData.data.lights.empty())
        {
            JSONCharValue json_value;
            if (!(TDataDoc<std::vector<std::shared_ptr<SLight>>>(_rData.data.lights, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("lights", json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SKHR_lights_punctualglTFextension>& _pData, const JSONCharValue& _JsonValue)
    {
        std::shared_ptr<SKHR_lights_punctualglTFextension> data_ptr = !!_pData ? _pData : std::make_shared<SKHR_lights_punctualglTFextension>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SKHR_lights_punctualglTFextension>>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SKHR_lights_punctualglTFextension>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SKHR_lights_punctualglTFextension>>& _vDatas, const JSONCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SKHR_lights_punctualglTFextension>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SKHR_lights_punctualglTFextension>>>& _rData, JSONCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SKHR_lights_punctualglTFextension>>(_rData, _JsonValue);
    }

    bool operator<<(SLight& _rData, const JSONCharValue& _JsonValue)
    {
        {
            SGlTFChildofRootProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember("color") && _JsonValue["color"].IsArray())
        {
            if (!(_rData.color << _JsonValue["color"])) return false;
        }
        if (_JsonValue.HasMember("intensity") && _JsonValue["intensity"].IsNumber())
        {
            _rData.intensity = _JsonValue["intensity"].GetFloat();
        }
        if (_JsonValue.HasMember("spot") && _JsonValue["spot"].IsObject())
        {
            if (!(_rData.spot << _JsonValue["spot"])) return false;
        }
        if (_JsonValue.HasMember("type") && _JsonValue["type"].IsString())
        {
            _rData.type = _JsonValue["type"].GetString();
        }
        if (_JsonValue.HasMember("range") && _JsonValue["range"].IsNumber())
        {
            _rData.range = _JsonValue["range"].GetFloat();
        }
        _rData.schemaType = "light.schema.json";
        return true;
    }

    bool operator>>(const TDataDoc<SLight>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFChildofRootProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFChildofRootProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        if (!_rData.data.color.empty())
        {
            JSONCharValue json_value;
            if (!(TDataDoc<std::vector<float>>(_rData.data.color, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("color", json_value, _rData.doc->GetAllocator());
        }
        {
            JSONCharValue json_value;
            if (!(TDataDoc<float>(_rData.data.intensity, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("intensity", json_value, _rData.doc->GetAllocator());
        }
        if (!!_rData.data.spot)
        {
            JSONCharValue json_value;
            if (!(TDataDoc<SLightspot>(*_rData.data.spot, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("spot", json_value, _rData.doc->GetAllocator());
        }
        {
            JSONCharValue json_value;
            if (!(TDataDoc<std::string>(_rData.data.type, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("type", json_value, _rData.doc->GetAllocator());
        }
        {
            JSONCharValue json_value;
            if (!(TDataDoc<float>(_rData.data.range, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("range", json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SLight>& _pData, const JSONCharValue& _JsonValue)
    {
        std::shared_ptr<SLight> data_ptr = !!_pData ? _pData : std::make_shared<SLight>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SLight>>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SLight>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SLight>>& _vDatas, const JSONCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SLight>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SLight>>>& _rData, JSONCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SLight>>(_rData, _JsonValue);
    }

    bool operator<<(SLightspot& _rData, const JSONCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember("innerConeAngle") && _JsonValue["innerConeAngle"].IsNumber())
        {
            _rData.innerConeAngle = _JsonValue["innerConeAngle"].GetFloat();
        }
        if (_JsonValue.HasMember("outerConeAngle") && _JsonValue["outerConeAngle"].IsNumber())
        {
            _rData.outerConeAngle = _JsonValue["outerConeAngle"].GetFloat();
        }
        _rData.schemaType = "light.spot.schema.json";
        return true;
    }

    bool operator>>(const TDataDoc<SLightspot>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        {
            JSONCharValue json_value;
            if (!(TDataDoc<float>(_rData.data.innerConeAngle, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("innerConeAngle", json_value, _rData.doc->GetAllocator());
        }
        {
            JSONCharValue json_value;
            if (!(TDataDoc<float>(_rData.data.outerConeAngle, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("outerConeAngle", json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SLightspot>& _pData, const JSONCharValue& _JsonValue)
    {
        std::shared_ptr<SLightspot> data_ptr = !!_pData ? _pData : std::make_shared<SLightspot>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SLightspot>>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SLightspot>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SLightspot>>& _vDatas, const JSONCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SLightspot>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SLightspot>>>& _rData, JSONCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SLightspot>>(_rData, _JsonValue);
    }

    bool operator<<(SKHR_lights_punctualnodeextension& _rData, const JSONCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember("light") && _JsonValue["light"].IsInt())
        {
            if (!(_rData.light << _JsonValue["light"])) return false;
        }
        _rData.schemaType = "node.KHR_lights_punctual.schema.json";
        return true;
    }

    bool operator>>(const TDataDoc<SKHR_lights_punctualnodeextension>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        if (!!_rData.data.light)
        {
            JSONCharValue json_value;
            if (!(TDataDoc<SGlTFId>(*_rData.data.light, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("light", json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SKHR_lights_punctualnodeextension>& _pData, const JSONCharValue& _JsonValue)
    {
        std::shared_ptr<SKHR_lights_punctualnodeextension> data_ptr = !!_pData ? _pData : std::make_shared<SKHR_lights_punctualnodeextension>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SKHR_lights_punctualnodeextension>>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SKHR_lights_punctualnodeextension>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SKHR_lights_punctualnodeextension>>& _vDatas, const JSONCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SKHR_lights_punctualnodeextension>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SKHR_lights_punctualnodeextension>>>& _rData, JSONCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SKHR_lights_punctualnodeextension>>(_rData, _JsonValue);
    }

    bool operator<<(SKHR_materials_clearcoatglTFextension& _rData, const JSONCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember("clearcoatFactor") && _JsonValue["clearcoatFactor"].IsNumber())
        {
            _rData.clearcoatFactor = _JsonValue["clearcoatFactor"].GetFloat();
        }
        if (_JsonValue.HasMember("clearcoatTexture") && _JsonValue["clearcoatTexture"].IsObject())
        {
            if (!(_rData.clearcoatTexture << _JsonValue["clearcoatTexture"])) return false;
        }
        if (_JsonValue.HasMember("clearcoatRoughnessFactor") && _JsonValue["clearcoatRoughnessFactor"].IsNumber())
        {
            _rData.clearcoatRoughnessFactor = _JsonValue["clearcoatRoughnessFactor"].GetFloat();
        }
        if (_JsonValue.HasMember("clearcoatRoughnessTexture") && _JsonValue["clearcoatRoughnessTexture"].IsObject())
        {
            if (!(_rData.clearcoatRoughnessTexture << _JsonValue["clearcoatRoughnessTexture"])) return false;
        }
        if (_JsonValue.HasMember("clearcoatNormalTexture") && _JsonValue["clearcoatNormalTexture"].IsObject())
        {
            if (!(_rData.clearcoatNormalTexture << _JsonValue["clearcoatNormalTexture"])) return false;
        }
        _rData.schemaType = "glTF.KHR_materials_clearcoat.schema.json";
        return true;
    }

    bool operator>>(const TDataDoc<SKHR_materials_clearcoatglTFextension>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        {
            JSONCharValue json_value;
            if (!(TDataDoc<float>(_rData.data.clearcoatFactor, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("clearcoatFactor", json_value, _rData.doc->GetAllocator());
        }
        if (!!_rData.data.clearcoatTexture)
        {
            JSONCharValue json_value;
            if (!(TDataDoc<STextureInfo>(*_rData.data.clearcoatTexture, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("clearcoatTexture", json_value, _rData.doc->GetAllocator());
        }
        {
            JSONCharValue json_value;
            if (!(TDataDoc<float>(_rData.data.clearcoatRoughnessFactor, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("clearcoatRoughnessFactor", json_value, _rData.doc->GetAllocator());
        }
        if (!!_rData.data.clearcoatRoughnessTexture)
        {
            JSONCharValue json_value;
            if (!(TDataDoc<STextureInfo>(*_rData.data.clearcoatRoughnessTexture, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("clearcoatRoughnessTexture", json_value, _rData.doc->GetAllocator());
        }
        if (!!_rData.data.clearcoatNormalTexture)
        {
            JSONCharValue json_value;
            if (!(TDataDoc<SMaterialNormalTextureInfo>(*_rData.data.clearcoatNormalTexture, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("clearcoatNormalTexture", json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SKHR_materials_clearcoatglTFextension>& _pData, const JSONCharValue& _JsonValue)
    {
        std::shared_ptr<SKHR_materials_clearcoatglTFextension> data_ptr = !!_pData ? _pData : std::make_shared<SKHR_materials_clearcoatglTFextension>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SKHR_materials_clearcoatglTFextension>>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SKHR_materials_clearcoatglTFextension>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SKHR_materials_clearcoatglTFextension>>& _vDatas, const JSONCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SKHR_materials_clearcoatglTFextension>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SKHR_materials_clearcoatglTFextension>>>& _rData, JSONCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SKHR_materials_clearcoatglTFextension>>(_rData, _JsonValue);
    }

    bool operator<<(SKHR_materials_emissive_strengthglTFextension& _rData, const JSONCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember("emissiveStrength") && _JsonValue["emissiveStrength"].IsNumber())
        {
            _rData.emissiveStrength = _JsonValue["emissiveStrength"].GetFloat();
        }
        _rData.schemaType = "glTF.KHR_materials_emissive_strength.schema.json";
        return true;
    }

    bool operator>>(const TDataDoc<SKHR_materials_emissive_strengthglTFextension>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        {
            JSONCharValue json_value;
            if (!(TDataDoc<float>(_rData.data.emissiveStrength, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("emissiveStrength", json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SKHR_materials_emissive_strengthglTFextension>& _pData, const JSONCharValue& _JsonValue)
    {
        std::shared_ptr<SKHR_materials_emissive_strengthglTFextension> data_ptr = !!_pData ? _pData : std::make_shared<SKHR_materials_emissive_strengthglTFextension>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SKHR_materials_emissive_strengthglTFextension>>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SKHR_materials_emissive_strengthglTFextension>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SKHR_materials_emissive_strengthglTFextension>>& _vDatas, const JSONCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SKHR_materials_emissive_strengthglTFextension>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SKHR_materials_emissive_strengthglTFextension>>>& _rData, JSONCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SKHR_materials_emissive_strengthglTFextension>>(_rData, _JsonValue);
    }

    bool operator<<(SKHR_materials_iorglTFextension& _rData, const JSONCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember("ior") && _JsonValue["ior"].IsNumber())
        {
            _rData.ior = _JsonValue["ior"].GetFloat();
        }
        _rData.schemaType = "glTF.KHR_materials_ior.schema.json";
        return true;
    }

    bool operator>>(const TDataDoc<SKHR_materials_iorglTFextension>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        {
            JSONCharValue json_value;
            if (!(TDataDoc<float>(_rData.data.ior, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("ior", json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SKHR_materials_iorglTFextension>& _pData, const JSONCharValue& _JsonValue)
    {
        std::shared_ptr<SKHR_materials_iorglTFextension> data_ptr = !!_pData ? _pData : std::make_shared<SKHR_materials_iorglTFextension>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SKHR_materials_iorglTFextension>>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SKHR_materials_iorglTFextension>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SKHR_materials_iorglTFextension>>& _vDatas, const JSONCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SKHR_materials_iorglTFextension>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SKHR_materials_iorglTFextension>>>& _rData, JSONCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SKHR_materials_iorglTFextension>>(_rData, _JsonValue);
    }

    bool operator<<(SKHR_materials_iridescenceglTFextension& _rData, const JSONCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember("iridescenceFactor") && _JsonValue["iridescenceFactor"].IsNumber())
        {
            _rData.iridescenceFactor = _JsonValue["iridescenceFactor"].GetFloat();
        }
        if (_JsonValue.HasMember("iridescenceTexture") && _JsonValue["iridescenceTexture"].IsObject())
        {
            if (!(_rData.iridescenceTexture << _JsonValue["iridescenceTexture"])) return false;
        }
        if (_JsonValue.HasMember("iridescenceIor") && _JsonValue["iridescenceIor"].IsNumber())
        {
            _rData.iridescenceIor = _JsonValue["iridescenceIor"].GetFloat();
        }
        if (_JsonValue.HasMember("iridescenceThicknessMinimum") && _JsonValue["iridescenceThicknessMinimum"].IsNumber())
        {
            _rData.iridescenceThicknessMinimum = _JsonValue["iridescenceThicknessMinimum"].GetFloat();
        }
        if (_JsonValue.HasMember("iridescenceThicknessMaximum") && _JsonValue["iridescenceThicknessMaximum"].IsNumber())
        {
            _rData.iridescenceThicknessMaximum = _JsonValue["iridescenceThicknessMaximum"].GetFloat();
        }
        if (_JsonValue.HasMember("iridescenceThicknessTexture") && _JsonValue["iridescenceThicknessTexture"].IsObject())
        {
            if (!(_rData.iridescenceThicknessTexture << _JsonValue["iridescenceThicknessTexture"])) return false;
        }
        _rData.schemaType = "glTF.KHR_materials_iridescence.schema.json";
        return true;
    }

    bool operator>>(const TDataDoc<SKHR_materials_iridescenceglTFextension>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        {
            JSONCharValue json_value;
            if (!(TDataDoc<float>(_rData.data.iridescenceFactor, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("iridescenceFactor", json_value, _rData.doc->GetAllocator());
        }
        if (!!_rData.data.iridescenceTexture)
        {
            JSONCharValue json_value;
            if (!(TDataDoc<STextureInfo>(*_rData.data.iridescenceTexture, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("iridescenceTexture", json_value, _rData.doc->GetAllocator());
        }
        {
            JSONCharValue json_value;
            if (!(TDataDoc<float>(_rData.data.iridescenceIor, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("iridescenceIor", json_value, _rData.doc->GetAllocator());
        }
        {
            JSONCharValue json_value;
            if (!(TDataDoc<float>(_rData.data.iridescenceThicknessMinimum, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("iridescenceThicknessMinimum", json_value, _rData.doc->GetAllocator());
        }
        {
            JSONCharValue json_value;
            if (!(TDataDoc<float>(_rData.data.iridescenceThicknessMaximum, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("iridescenceThicknessMaximum", json_value, _rData.doc->GetAllocator());
        }
        if (!!_rData.data.iridescenceThicknessTexture)
        {
            JSONCharValue json_value;
            if (!(TDataDoc<STextureInfo>(*_rData.data.iridescenceThicknessTexture, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("iridescenceThicknessTexture", json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SKHR_materials_iridescenceglTFextension>& _pData, const JSONCharValue& _JsonValue)
    {
        std::shared_ptr<SKHR_materials_iridescenceglTFextension> data_ptr = !!_pData ? _pData : std::make_shared<SKHR_materials_iridescenceglTFextension>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SKHR_materials_iridescenceglTFextension>>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SKHR_materials_iridescenceglTFextension>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SKHR_materials_iridescenceglTFextension>>& _vDatas, const JSONCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SKHR_materials_iridescenceglTFextension>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SKHR_materials_iridescenceglTFextension>>>& _rData, JSONCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SKHR_materials_iridescenceglTFextension>>(_rData, _JsonValue);
    }

    bool operator<<(SKHR_materials_sheenglTFextension& _rData, const JSONCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember("sheenColorFactor") && _JsonValue["sheenColorFactor"].IsArray())
        {
            if (!(_rData.sheenColorFactor << _JsonValue["sheenColorFactor"])) return false;
        }
        if (_JsonValue.HasMember("sheenColorTexture") && _JsonValue["sheenColorTexture"].IsObject())
        {
            if (!(_rData.sheenColorTexture << _JsonValue["sheenColorTexture"])) return false;
        }
        if (_JsonValue.HasMember("sheenRoughnessFactor") && _JsonValue["sheenRoughnessFactor"].IsNumber())
        {
            _rData.sheenRoughnessFactor = _JsonValue["sheenRoughnessFactor"].GetFloat();
        }
        if (_JsonValue.HasMember("sheenRoughnessTexture") && _JsonValue["sheenRoughnessTexture"].IsObject())
        {
            if (!(_rData.sheenRoughnessTexture << _JsonValue["sheenRoughnessTexture"])) return false;
        }
        _rData.schemaType = "glTF.KHR_materials_sheen.schema.json";
        return true;
    }

    bool operator>>(const TDataDoc<SKHR_materials_sheenglTFextension>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        if (!_rData.data.sheenColorFactor.empty())
        {
            JSONCharValue json_value;
            if (!(TDataDoc<std::vector<float>>(_rData.data.sheenColorFactor, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("sheenColorFactor", json_value, _rData.doc->GetAllocator());
        }
        if (!!_rData.data.sheenColorTexture)
        {
            JSONCharValue json_value;
            if (!(TDataDoc<STextureInfo>(*_rData.data.sheenColorTexture, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("sheenColorTexture", json_value, _rData.doc->GetAllocator());
        }
        {
            JSONCharValue json_value;
            if (!(TDataDoc<float>(_rData.data.sheenRoughnessFactor, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("sheenRoughnessFactor", json_value, _rData.doc->GetAllocator());
        }
        if (!!_rData.data.sheenRoughnessTexture)
        {
            JSONCharValue json_value;
            if (!(TDataDoc<STextureInfo>(*_rData.data.sheenRoughnessTexture, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("sheenRoughnessTexture", json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SKHR_materials_sheenglTFextension>& _pData, const JSONCharValue& _JsonValue)
    {
        std::shared_ptr<SKHR_materials_sheenglTFextension> data_ptr = !!_pData ? _pData : std::make_shared<SKHR_materials_sheenglTFextension>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SKHR_materials_sheenglTFextension>>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SKHR_materials_sheenglTFextension>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SKHR_materials_sheenglTFextension>>& _vDatas, const JSONCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SKHR_materials_sheenglTFextension>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SKHR_materials_sheenglTFextension>>>& _rData, JSONCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SKHR_materials_sheenglTFextension>>(_rData, _JsonValue);
    }

    bool operator<<(SKHR_materials_specularglTFextension& _rData, const JSONCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember("specularFactor") && _JsonValue["specularFactor"].IsNumber())
        {
            _rData.specularFactor = _JsonValue["specularFactor"].GetFloat();
        }
        if (_JsonValue.HasMember("specularTexture") && _JsonValue["specularTexture"].IsObject())
        {
            if (!(_rData.specularTexture << _JsonValue["specularTexture"])) return false;
        }
        if (_JsonValue.HasMember("specularColorFactor") && _JsonValue["specularColorFactor"].IsArray())
        {
            if (!(_rData.specularColorFactor << _JsonValue["specularColorFactor"])) return false;
        }
        if (_JsonValue.HasMember("specularColorTexture") && _JsonValue["specularColorTexture"].IsObject())
        {
            if (!(_rData.specularColorTexture << _JsonValue["specularColorTexture"])) return false;
        }
        _rData.schemaType = "glTF.KHR_materials_specular.schema.json";
        return true;
    }

    bool operator>>(const TDataDoc<SKHR_materials_specularglTFextension>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        {
            JSONCharValue json_value;
            if (!(TDataDoc<float>(_rData.data.specularFactor, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("specularFactor", json_value, _rData.doc->GetAllocator());
        }
        if (!!_rData.data.specularTexture)
        {
            JSONCharValue json_value;
            if (!(TDataDoc<STextureInfo>(*_rData.data.specularTexture, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("specularTexture", json_value, _rData.doc->GetAllocator());
        }
        if (!_rData.data.specularColorFactor.empty())
        {
            JSONCharValue json_value;
            if (!(TDataDoc<std::vector<float>>(_rData.data.specularColorFactor, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("specularColorFactor", json_value, _rData.doc->GetAllocator());
        }
        if (!!_rData.data.specularColorTexture)
        {
            JSONCharValue json_value;
            if (!(TDataDoc<STextureInfo>(*_rData.data.specularColorTexture, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("specularColorTexture", json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SKHR_materials_specularglTFextension>& _pData, const JSONCharValue& _JsonValue)
    {
        std::shared_ptr<SKHR_materials_specularglTFextension> data_ptr = !!_pData ? _pData : std::make_shared<SKHR_materials_specularglTFextension>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SKHR_materials_specularglTFextension>>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SKHR_materials_specularglTFextension>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SKHR_materials_specularglTFextension>>& _vDatas, const JSONCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SKHR_materials_specularglTFextension>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SKHR_materials_specularglTFextension>>>& _rData, JSONCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SKHR_materials_specularglTFextension>>(_rData, _JsonValue);
    }

    bool operator<<(SKHR_materials_transmissionglTFextension& _rData, const JSONCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember("transmissionFactor") && _JsonValue["transmissionFactor"].IsNumber())
        {
            _rData.transmissionFactor = _JsonValue["transmissionFactor"].GetFloat();
        }
        if (_JsonValue.HasMember("transmissionTexture") && _JsonValue["transmissionTexture"].IsObject())
        {
            if (!(_rData.transmissionTexture << _JsonValue["transmissionTexture"])) return false;
        }
        _rData.schemaType = "glTF.KHR_materials_transmission.schema.json";
        return true;
    }

    bool operator>>(const TDataDoc<SKHR_materials_transmissionglTFextension>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        {
            JSONCharValue json_value;
            if (!(TDataDoc<float>(_rData.data.transmissionFactor, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("transmissionFactor", json_value, _rData.doc->GetAllocator());
        }
        if (!!_rData.data.transmissionTexture)
        {
            JSONCharValue json_value;
            if (!(TDataDoc<STextureInfo>(*_rData.data.transmissionTexture, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("transmissionTexture", json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SKHR_materials_transmissionglTFextension>& _pData, const JSONCharValue& _JsonValue)
    {
        std::shared_ptr<SKHR_materials_transmissionglTFextension> data_ptr = !!_pData ? _pData : std::make_shared<SKHR_materials_transmissionglTFextension>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SKHR_materials_transmissionglTFextension>>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SKHR_materials_transmissionglTFextension>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SKHR_materials_transmissionglTFextension>>& _vDatas, const JSONCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SKHR_materials_transmissionglTFextension>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SKHR_materials_transmissionglTFextension>>>& _rData, JSONCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SKHR_materials_transmissionglTFextension>>(_rData, _JsonValue);
    }

    bool operator<<(SKHR_materials_unlitglTFextension& _rData, const JSONCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        _rData.schemaType = "glTF.KHR_materials_unlit.schema.json";
        return true;
    }

    bool operator>>(const TDataDoc<SKHR_materials_unlitglTFextension>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SKHR_materials_unlitglTFextension>& _pData, const JSONCharValue& _JsonValue)
    {
        std::shared_ptr<SKHR_materials_unlitglTFextension> data_ptr = !!_pData ? _pData : std::make_shared<SKHR_materials_unlitglTFextension>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SKHR_materials_unlitglTFextension>>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SKHR_materials_unlitglTFextension>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SKHR_materials_unlitglTFextension>>& _vDatas, const JSONCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SKHR_materials_unlitglTFextension>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SKHR_materials_unlitglTFextension>>>& _rData, JSONCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SKHR_materials_unlitglTFextension>>(_rData, _JsonValue);
    }

    bool operator<<(SKHR_materials_variantsglTFextension& _rData, const JSONCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember("variants") && _JsonValue["variants"].IsArray())
        {
            if (!(_rData.variants << _JsonValue["variants"])) return false;
        }
        _rData.schemaType = "glTF.KHR_materials_variants.schema.json";
        return true;
    }

    bool operator>>(const TDataDoc<SKHR_materials_variantsglTFextension>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        if (!_rData.data.variants.empty())
        {
            JSONCharValue json_value;
            if (!(TDataDoc<std::vector<std::shared_ptr<SGlTFChildofRootProperty>>>(_rData.data.variants, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("variants", json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SKHR_materials_variantsglTFextension>& _pData, const JSONCharValue& _JsonValue)
    {
        std::shared_ptr<SKHR_materials_variantsglTFextension> data_ptr = !!_pData ? _pData : std::make_shared<SKHR_materials_variantsglTFextension>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SKHR_materials_variantsglTFextension>>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SKHR_materials_variantsglTFextension>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SKHR_materials_variantsglTFextension>>& _vDatas, const JSONCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SKHR_materials_variantsglTFextension>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SKHR_materials_variantsglTFextension>>>& _rData, JSONCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SKHR_materials_variantsglTFextension>>(_rData, _JsonValue);
    }

    bool operator<<(SKHR_materials_variantsmeshprimitiveextension& _rData, const JSONCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember("mappings") && _JsonValue["mappings"].IsArray())
        {
            if (!(_rData.mappings << _JsonValue["mappings"])) return false;
        }
        _rData.schemaType = "mesh.primitive.KHR_materials_variants.schema.json";
        return true;
    }

    bool operator>>(const TDataDoc<SKHR_materials_variantsmeshprimitiveextension>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        if (!_rData.data.mappings.empty())
        {
            JSONCharValue json_value;
            if (!(TDataDoc<std::vector<std::shared_ptr<SGlTFProperty>>>(_rData.data.mappings, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("mappings", json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SKHR_materials_variantsmeshprimitiveextension>& _pData, const JSONCharValue& _JsonValue)
    {
        std::shared_ptr<SKHR_materials_variantsmeshprimitiveextension> data_ptr = !!_pData ? _pData : std::make_shared<SKHR_materials_variantsmeshprimitiveextension>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SKHR_materials_variantsmeshprimitiveextension>>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SKHR_materials_variantsmeshprimitiveextension>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SKHR_materials_variantsmeshprimitiveextension>>& _vDatas, const JSONCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SKHR_materials_variantsmeshprimitiveextension>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SKHR_materials_variantsmeshprimitiveextension>>>& _rData, JSONCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SKHR_materials_variantsmeshprimitiveextension>>(_rData, _JsonValue);
    }

    bool operator<<(SKHR_materials_volumeglTFextension& _rData, const JSONCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember("thicknessFactor") && _JsonValue["thicknessFactor"].IsNumber())
        {
            _rData.thicknessFactor = _JsonValue["thicknessFactor"].GetFloat();
        }
        if (_JsonValue.HasMember("thicknessTexture") && _JsonValue["thicknessTexture"].IsObject())
        {
            if (!(_rData.thicknessTexture << _JsonValue["thicknessTexture"])) return false;
        }
        if (_JsonValue.HasMember("attenuationDistance") && _JsonValue["attenuationDistance"].IsNumber())
        {
            _rData.attenuationDistance = _JsonValue["attenuationDistance"].GetFloat();
        }
        if (_JsonValue.HasMember("attenuationColor") && _JsonValue["attenuationColor"].IsArray())
        {
            if (!(_rData.attenuationColor << _JsonValue["attenuationColor"])) return false;
        }
        _rData.schemaType = "glTF.KHR_materials_volume.schema.json";
        return true;
    }

    bool operator>>(const TDataDoc<SKHR_materials_volumeglTFextension>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        {
            JSONCharValue json_value;
            if (!(TDataDoc<float>(_rData.data.thicknessFactor, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("thicknessFactor", json_value, _rData.doc->GetAllocator());
        }
        if (!!_rData.data.thicknessTexture)
        {
            JSONCharValue json_value;
            if (!(TDataDoc<STextureInfo>(*_rData.data.thicknessTexture, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("thicknessTexture", json_value, _rData.doc->GetAllocator());
        }
        {
            JSONCharValue json_value;
            if (!(TDataDoc<float>(_rData.data.attenuationDistance, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("attenuationDistance", json_value, _rData.doc->GetAllocator());
        }
        if (!_rData.data.attenuationColor.empty())
        {
            JSONCharValue json_value;
            if (!(TDataDoc<std::vector<float>>(_rData.data.attenuationColor, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("attenuationColor", json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SKHR_materials_volumeglTFextension>& _pData, const JSONCharValue& _JsonValue)
    {
        std::shared_ptr<SKHR_materials_volumeglTFextension> data_ptr = !!_pData ? _pData : std::make_shared<SKHR_materials_volumeglTFextension>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SKHR_materials_volumeglTFextension>>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SKHR_materials_volumeglTFextension>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SKHR_materials_volumeglTFextension>>& _vDatas, const JSONCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SKHR_materials_volumeglTFextension>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SKHR_materials_volumeglTFextension>>>& _rData, JSONCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SKHR_materials_volumeglTFextension>>(_rData, _JsonValue);
    }

    bool operator<<(SKHR_texture_transformtextureInfoextension& _rData, const JSONCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember("offset") && _JsonValue["offset"].IsArray())
        {
            if (!(_rData.offset << _JsonValue["offset"])) return false;
        }
        if (_JsonValue.HasMember("rotation") && _JsonValue["rotation"].IsNumber())
        {
            _rData.rotation = _JsonValue["rotation"].GetFloat();
        }
        if (_JsonValue.HasMember("scale") && _JsonValue["scale"].IsArray())
        {
            if (!(_rData.scale << _JsonValue["scale"])) return false;
        }
        if (_JsonValue.HasMember("texCoord") && _JsonValue["texCoord"].IsInt())
        {
            _rData.texCoord = _JsonValue["texCoord"].GetInt();
        }
        _rData.schemaType = "KHR_texture_transform.textureInfo.schema.json";
        return true;
    }

    bool operator>>(const TDataDoc<SKHR_texture_transformtextureInfoextension>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        if (!_rData.data.offset.empty())
        {
            JSONCharValue json_value;
            if (!(TDataDoc<std::vector<float>>(_rData.data.offset, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("offset", json_value, _rData.doc->GetAllocator());
        }
        {
            JSONCharValue json_value;
            if (!(TDataDoc<float>(_rData.data.rotation, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("rotation", json_value, _rData.doc->GetAllocator());
        }
        if (!_rData.data.scale.empty())
        {
            JSONCharValue json_value;
            if (!(TDataDoc<std::vector<float>>(_rData.data.scale, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("scale", json_value, _rData.doc->GetAllocator());
        }
        {
            JSONCharValue json_value;
            if (!(TDataDoc<int32_t>(_rData.data.texCoord, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("texCoord", json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SKHR_texture_transformtextureInfoextension>& _pData, const JSONCharValue& _JsonValue)
    {
        std::shared_ptr<SKHR_texture_transformtextureInfoextension> data_ptr = !!_pData ? _pData : std::make_shared<SKHR_texture_transformtextureInfoextension>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SKHR_texture_transformtextureInfoextension>>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SKHR_texture_transformtextureInfoextension>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SKHR_texture_transformtextureInfoextension>>& _vDatas, const JSONCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SKHR_texture_transformtextureInfoextension>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SKHR_texture_transformtextureInfoextension>>>& _rData, JSONCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SKHR_texture_transformtextureInfoextension>>(_rData, _JsonValue);
    }

    bool operator<<(SADOBE_materials_thin_transparencyglTFextension& _rData, const JSONCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember("transmissionFactor") && _JsonValue["transmissionFactor"].IsNumber())
        {
            _rData.transmissionFactor = _JsonValue["transmissionFactor"].GetFloat();
        }
        if (_JsonValue.HasMember("transmissionTexture") && _JsonValue["transmissionTexture"].IsObject())
        {
            if (!(_rData.transmissionTexture << _JsonValue["transmissionTexture"])) return false;
        }
        if (_JsonValue.HasMember("ior") && _JsonValue["ior"].IsNumber())
        {
            _rData.ior = _JsonValue["ior"].GetFloat();
        }
        _rData.schemaType = "glTF.ADOBE_materials_thin_transparency.schema.json";
        return true;
    }

    bool operator>>(const TDataDoc<SADOBE_materials_thin_transparencyglTFextension>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        {
            JSONCharValue json_value;
            if (!(TDataDoc<float>(_rData.data.transmissionFactor, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("transmissionFactor", json_value, _rData.doc->GetAllocator());
        }
        if (!!_rData.data.transmissionTexture)
        {
            JSONCharValue json_value;
            if (!(TDataDoc<STextureInfo>(*_rData.data.transmissionTexture, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("transmissionTexture", json_value, _rData.doc->GetAllocator());
        }
        {
            JSONCharValue json_value;
            if (!(TDataDoc<float>(_rData.data.ior, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("ior", json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SADOBE_materials_thin_transparencyglTFextension>& _pData, const JSONCharValue& _JsonValue)
    {
        std::shared_ptr<SADOBE_materials_thin_transparencyglTFextension> data_ptr = !!_pData ? _pData : std::make_shared<SADOBE_materials_thin_transparencyglTFextension>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SADOBE_materials_thin_transparencyglTFextension>>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SADOBE_materials_thin_transparencyglTFextension>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SADOBE_materials_thin_transparencyglTFextension>>& _vDatas, const JSONCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SADOBE_materials_thin_transparencyglTFextension>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SADOBE_materials_thin_transparencyglTFextension>>>& _rData, JSONCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SADOBE_materials_thin_transparencyglTFextension>>(_rData, _JsonValue);
    }

    bool operator<<(SArticulation& _rData, const JSONCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember("name") && _JsonValue["name"].IsString())
        {
            _rData.name = _JsonValue["name"].GetString();
        }
        if (_JsonValue.HasMember("stages") && _JsonValue["stages"].IsArray())
        {
            if (!(_rData.stages << _JsonValue["stages"])) return false;
        }
        if (_JsonValue.HasMember("pointingVector") && _JsonValue["pointingVector"].IsArray())
        {
            if (!(_rData.pointingVector << _JsonValue["pointingVector"])) return false;
        }
        _rData.schemaType = "articulation.schema.json";
        return true;
    }

    bool operator>>(const TDataDoc<SArticulation>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        {
            JSONCharValue json_value;
            if (!(TDataDoc<std::string>(_rData.data.name, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("name", json_value, _rData.doc->GetAllocator());
        }
        if (!_rData.data.stages.empty())
        {
            JSONCharValue json_value;
            if (!(TDataDoc<std::vector<std::shared_ptr<SArticulationStage>>>(_rData.data.stages, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("stages", json_value, _rData.doc->GetAllocator());
        }
        if (!_rData.data.pointingVector.empty())
        {
            JSONCharValue json_value;
            if (!(TDataDoc<std::vector<float>>(_rData.data.pointingVector, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("pointingVector", json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SArticulation>& _pData, const JSONCharValue& _JsonValue)
    {
        std::shared_ptr<SArticulation> data_ptr = !!_pData ? _pData : std::make_shared<SArticulation>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SArticulation>>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SArticulation>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SArticulation>>& _vDatas, const JSONCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SArticulation>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SArticulation>>>& _rData, JSONCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SArticulation>>(_rData, _JsonValue);
    }

    bool operator<<(SArticulationStage& _rData, const JSONCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember("name") && _JsonValue["name"].IsString())
        {
            _rData.name = _JsonValue["name"].GetString();
        }
        if (_JsonValue.HasMember("type") && _JsonValue["type"].IsString())
        {
            _rData.type = _JsonValue["type"].GetString();
        }
        if (_JsonValue.HasMember("minimumValue") && _JsonValue["minimumValue"].IsNumber())
        {
            _rData.minimumValue = _JsonValue["minimumValue"].GetFloat();
        }
        if (_JsonValue.HasMember("maximumValue") && _JsonValue["maximumValue"].IsNumber())
        {
            _rData.maximumValue = _JsonValue["maximumValue"].GetFloat();
        }
        if (_JsonValue.HasMember("initialValue") && _JsonValue["initialValue"].IsNumber())
        {
            _rData.initialValue = _JsonValue["initialValue"].GetFloat();
        }
        _rData.schemaType = "articulation.stage.schema.json";
        return true;
    }

    bool operator>>(const TDataDoc<SArticulationStage>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        {
            JSONCharValue json_value;
            if (!(TDataDoc<std::string>(_rData.data.name, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("name", json_value, _rData.doc->GetAllocator());
        }
        {
            JSONCharValue json_value;
            if (!(TDataDoc<std::string>(_rData.data.type, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("type", json_value, _rData.doc->GetAllocator());
        }
        {
            JSONCharValue json_value;
            if (!(TDataDoc<float>(_rData.data.minimumValue, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("minimumValue", json_value, _rData.doc->GetAllocator());
        }
        {
            JSONCharValue json_value;
            if (!(TDataDoc<float>(_rData.data.maximumValue, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("maximumValue", json_value, _rData.doc->GetAllocator());
        }
        {
            JSONCharValue json_value;
            if (!(TDataDoc<float>(_rData.data.initialValue, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("initialValue", json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SArticulationStage>& _pData, const JSONCharValue& _JsonValue)
    {
        std::shared_ptr<SArticulationStage> data_ptr = !!_pData ? _pData : std::make_shared<SArticulationStage>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SArticulationStage>>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SArticulationStage>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SArticulationStage>>& _vDatas, const JSONCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SArticulationStage>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SArticulationStage>>>& _rData, JSONCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SArticulationStage>>(_rData, _JsonValue);
    }

    bool operator<<(SAGI_articulationsglTFextension& _rData, const JSONCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember("articulations") && _JsonValue["articulations"].IsArray())
        {
            if (!(_rData.articulations << _JsonValue["articulations"])) return false;
        }
        _rData.schemaType = "gltf.AGI_articulations.schema.json";
        return true;
    }

    bool operator>>(const TDataDoc<SAGI_articulationsglTFextension>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        if (!_rData.data.articulations.empty())
        {
            JSONCharValue json_value;
            if (!(TDataDoc<std::vector<std::shared_ptr<SArticulation>>>(_rData.data.articulations, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("articulations", json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SAGI_articulationsglTFextension>& _pData, const JSONCharValue& _JsonValue)
    {
        std::shared_ptr<SAGI_articulationsglTFextension> data_ptr = !!_pData ? _pData : std::make_shared<SAGI_articulationsglTFextension>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SAGI_articulationsglTFextension>>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SAGI_articulationsglTFextension>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SAGI_articulationsglTFextension>>& _vDatas, const JSONCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SAGI_articulationsglTFextension>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SAGI_articulationsglTFextension>>>& _rData, JSONCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SAGI_articulationsglTFextension>>(_rData, _JsonValue);
    }

    bool operator<<(SAGI_articulationsglTFNodeextension& _rData, const JSONCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember("isAttachPoint") && _JsonValue["isAttachPoint"].IsBool())
        {
            _rData.isAttachPoint = _JsonValue["isAttachPoint"].GetBool();
        }
        if (_JsonValue.HasMember("articulationName") && _JsonValue["articulationName"].IsString())
        {
            _rData.articulationName = _JsonValue["articulationName"].GetString();
        }
        _rData.schemaType = "node.AGI_articulations.schema.json";
        return true;
    }

    bool operator>>(const TDataDoc<SAGI_articulationsglTFNodeextension>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        {
            JSONCharValue json_value;
            if (!(TDataDoc<bool>(_rData.data.isAttachPoint, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("isAttachPoint", json_value, _rData.doc->GetAllocator());
        }
        {
            JSONCharValue json_value;
            if (!(TDataDoc<std::string>(_rData.data.articulationName, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("articulationName", json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SAGI_articulationsglTFNodeextension>& _pData, const JSONCharValue& _JsonValue)
    {
        std::shared_ptr<SAGI_articulationsglTFNodeextension> data_ptr = !!_pData ? _pData : std::make_shared<SAGI_articulationsglTFNodeextension>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SAGI_articulationsglTFNodeextension>>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SAGI_articulationsglTFNodeextension>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SAGI_articulationsglTFNodeextension>>& _vDatas, const JSONCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SAGI_articulationsglTFNodeextension>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SAGI_articulationsglTFNodeextension>>>& _rData, JSONCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SAGI_articulationsglTFNodeextension>>(_rData, _JsonValue);
    }

    bool operator<<(SAGI_stk_metadataglTFextension& _rData, const JSONCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember("solarPanelGroups") && _JsonValue["solarPanelGroups"].IsArray())
        {
            if (!(_rData.solarPanelGroups << _JsonValue["solarPanelGroups"])) return false;
        }
        _rData.schemaType = "gltf.AGI_stk_metadata.schema.json";
        return true;
    }

    bool operator>>(const TDataDoc<SAGI_stk_metadataglTFextension>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        if (!_rData.data.solarPanelGroups.empty())
        {
            JSONCharValue json_value;
            if (!(TDataDoc<std::vector<std::shared_ptr<SSolarPanelGroup>>>(_rData.data.solarPanelGroups, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("solarPanelGroups", json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SAGI_stk_metadataglTFextension>& _pData, const JSONCharValue& _JsonValue)
    {
        std::shared_ptr<SAGI_stk_metadataglTFextension> data_ptr = !!_pData ? _pData : std::make_shared<SAGI_stk_metadataglTFextension>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SAGI_stk_metadataglTFextension>>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SAGI_stk_metadataglTFextension>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SAGI_stk_metadataglTFextension>>& _vDatas, const JSONCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SAGI_stk_metadataglTFextension>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SAGI_stk_metadataglTFextension>>>& _rData, JSONCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SAGI_stk_metadataglTFextension>>(_rData, _JsonValue);
    }

    bool operator<<(SAGI_stk_metadataglTFNodeextension& _rData, const JSONCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember("solarPanelGroupName") && _JsonValue["solarPanelGroupName"].IsString())
        {
            _rData.solarPanelGroupName = _JsonValue["solarPanelGroupName"].GetString();
        }
        if (_JsonValue.HasMember("noObscuration") && _JsonValue["noObscuration"].IsBool())
        {
            _rData.noObscuration = _JsonValue["noObscuration"].GetBool();
        }
        _rData.schemaType = "node.AGI_stk_metadata.schema.json";
        return true;
    }

    bool operator>>(const TDataDoc<SAGI_stk_metadataglTFNodeextension>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        {
            JSONCharValue json_value;
            if (!(TDataDoc<std::string>(_rData.data.solarPanelGroupName, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("solarPanelGroupName", json_value, _rData.doc->GetAllocator());
        }
        {
            JSONCharValue json_value;
            if (!(TDataDoc<bool>(_rData.data.noObscuration, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("noObscuration", json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SAGI_stk_metadataglTFNodeextension>& _pData, const JSONCharValue& _JsonValue)
    {
        std::shared_ptr<SAGI_stk_metadataglTFNodeextension> data_ptr = !!_pData ? _pData : std::make_shared<SAGI_stk_metadataglTFNodeextension>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SAGI_stk_metadataglTFNodeextension>>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SAGI_stk_metadataglTFNodeextension>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SAGI_stk_metadataglTFNodeextension>>& _vDatas, const JSONCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SAGI_stk_metadataglTFNodeextension>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SAGI_stk_metadataglTFNodeextension>>>& _rData, JSONCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SAGI_stk_metadataglTFNodeextension>>(_rData, _JsonValue);
    }

    bool operator<<(SSolarPanelGroup& _rData, const JSONCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember("name") && _JsonValue["name"].IsString())
        {
            _rData.name = _JsonValue["name"].GetString();
        }
        if (_JsonValue.HasMember("efficiency") && _JsonValue["efficiency"].IsNumber())
        {
            _rData.efficiency = _JsonValue["efficiency"].GetFloat();
        }
        _rData.schemaType = "solarPanelGroup.schema.json";
        return true;
    }

    bool operator>>(const TDataDoc<SSolarPanelGroup>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        {
            JSONCharValue json_value;
            if (!(TDataDoc<std::string>(_rData.data.name, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("name", json_value, _rData.doc->GetAllocator());
        }
        {
            JSONCharValue json_value;
            if (!(TDataDoc<float>(_rData.data.efficiency, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("efficiency", json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SSolarPanelGroup>& _pData, const JSONCharValue& _JsonValue)
    {
        std::shared_ptr<SSolarPanelGroup> data_ptr = !!_pData ? _pData : std::make_shared<SSolarPanelGroup>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SSolarPanelGroup>>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SSolarPanelGroup>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SSolarPanelGroup>>& _vDatas, const JSONCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SSolarPanelGroup>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SSolarPanelGroup>>>& _rData, JSONCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SSolarPanelGroup>>(_rData, _JsonValue);
    }

    bool operator<<(SCESIUM_primitive_outlineglTFprimitiveextension& _rData, const JSONCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember("indices") && _JsonValue["indices"].IsInt())
        {
            _rData.indices = _JsonValue["indices"].GetInt();
        }
        _rData.schemaType = "primitive.CESIUM_primitive_outline.schema.json";
        return true;
    }

    bool operator>>(const TDataDoc<SCESIUM_primitive_outlineglTFprimitiveextension>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        {
            JSONCharValue json_value;
            if (!(TDataDoc<int32_t>(_rData.data.indices, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("indices", json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SCESIUM_primitive_outlineglTFprimitiveextension>& _pData, const JSONCharValue& _JsonValue)
    {
        std::shared_ptr<SCESIUM_primitive_outlineglTFprimitiveextension> data_ptr = !!_pData ? _pData : std::make_shared<SCESIUM_primitive_outlineglTFprimitiveextension>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SCESIUM_primitive_outlineglTFprimitiveextension>>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SCESIUM_primitive_outlineglTFprimitiveextension>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SCESIUM_primitive_outlineglTFprimitiveextension>>& _vDatas, const JSONCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SCESIUM_primitive_outlineglTFprimitiveextension>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SCESIUM_primitive_outlineglTFprimitiveextension>>>& _rData, JSONCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SCESIUM_primitive_outlineglTFprimitiveextension>>(_rData, _JsonValue);
    }

    bool operator<<(SEXT_lights_iesglTFextension& _rData, const JSONCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember("lights") && _JsonValue["lights"].IsArray())
        {
            if (!(_rData.lights << _JsonValue["lights"])) return false;
        }
        _rData.schemaType = "glTF.EXT_lights_ies.schema.json";
        return true;
    }

    bool operator>>(const TDataDoc<SEXT_lights_iesglTFextension>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        if (!_rData.data.lights.empty())
        {
            JSONCharValue json_value;
            if (!(TDataDoc<std::vector<std::shared_ptr<SLightProfile>>>(_rData.data.lights, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("lights", json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SEXT_lights_iesglTFextension>& _pData, const JSONCharValue& _JsonValue)
    {
        std::shared_ptr<SEXT_lights_iesglTFextension> data_ptr = !!_pData ? _pData : std::make_shared<SEXT_lights_iesglTFextension>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SEXT_lights_iesglTFextension>>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SEXT_lights_iesglTFextension>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SEXT_lights_iesglTFextension>>& _vDatas, const JSONCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SEXT_lights_iesglTFextension>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SEXT_lights_iesglTFextension>>>& _rData, JSONCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SEXT_lights_iesglTFextension>>(_rData, _JsonValue);
    }

    bool operator<<(SLightProfile& _rData, const JSONCharValue& _JsonValue)
    {
        {
            SGlTFChildofRootProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember("uri") && _JsonValue["uri"].IsString())
        {
            _rData.uri = _JsonValue["uri"].GetString();
        }
        if (_JsonValue.HasMember("mimeType") && _JsonValue["mimeType"].IsString())
        {
            _rData.mimeType = _JsonValue["mimeType"].GetString();
        }
        if (_JsonValue.HasMember("bufferView") && _JsonValue["bufferView"].IsInt())
        {
            if (!(_rData.bufferView << _JsonValue["bufferView"])) return false;
        }
        _rData.schemaType = "lightProfile.schema.json";
        return true;
    }

    bool operator>>(const TDataDoc<SLightProfile>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFChildofRootProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFChildofRootProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        {
            JSONCharValue json_value;
            if (!(TDataDoc<std::string>(_rData.data.uri, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("uri", json_value, _rData.doc->GetAllocator());
        }
        {
            JSONCharValue json_value;
            if (!(TDataDoc<std::string>(_rData.data.mimeType, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("mimeType", json_value, _rData.doc->GetAllocator());
        }
        if (!!_rData.data.bufferView)
        {
            JSONCharValue json_value;
            if (!(TDataDoc<SGlTFId>(*_rData.data.bufferView, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("bufferView", json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SLightProfile>& _pData, const JSONCharValue& _JsonValue)
    {
        std::shared_ptr<SLightProfile> data_ptr = !!_pData ? _pData : std::make_shared<SLightProfile>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SLightProfile>>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SLightProfile>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SLightProfile>>& _vDatas, const JSONCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SLightProfile>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SLightProfile>>>& _rData, JSONCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SLightProfile>>(_rData, _JsonValue);
    }

    bool operator<<(SEXT_lights_iesnodeextension& _rData, const JSONCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember("light") && _JsonValue["light"].IsInt())
        {
            if (!(_rData.light << _JsonValue["light"])) return false;
        }
        if (_JsonValue.HasMember("multiplier") && _JsonValue["multiplier"].IsNumber())
        {
            _rData.multiplier = _JsonValue["multiplier"].GetFloat();
        }
        if (_JsonValue.HasMember("color") && _JsonValue["color"].IsArray())
        {
            if (!(_rData.color << _JsonValue["color"])) return false;
        }
        _rData.schemaType = "node.EXT_lights_ies.schema.json";
        return true;
    }

    bool operator>>(const TDataDoc<SEXT_lights_iesnodeextension>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        if (!!_rData.data.light)
        {
            JSONCharValue json_value;
            if (!(TDataDoc<SGlTFId>(*_rData.data.light, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("light", json_value, _rData.doc->GetAllocator());
        }
        {
            JSONCharValue json_value;
            if (!(TDataDoc<float>(_rData.data.multiplier, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("multiplier", json_value, _rData.doc->GetAllocator());
        }
        if (!_rData.data.color.empty())
        {
            JSONCharValue json_value;
            if (!(TDataDoc<std::vector<float>>(_rData.data.color, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("color", json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SEXT_lights_iesnodeextension>& _pData, const JSONCharValue& _JsonValue)
    {
        std::shared_ptr<SEXT_lights_iesnodeextension> data_ptr = !!_pData ? _pData : std::make_shared<SEXT_lights_iesnodeextension>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SEXT_lights_iesnodeextension>>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SEXT_lights_iesnodeextension>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SEXT_lights_iesnodeextension>>& _vDatas, const JSONCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SEXT_lights_iesnodeextension>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SEXT_lights_iesnodeextension>>>& _rData, JSONCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SEXT_lights_iesnodeextension>>(_rData, _JsonValue);
    }

    bool operator<<(SEXT_mesh_gpu_instancingglTFextension& _rData, const JSONCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember("attributes") && _JsonValue["attributes"].IsObject())
        {
            if (!(_rData.attributes << _JsonValue["attributes"])) return false;
        }
        _rData.schemaType = "glTF.EXT_mesh_gpu_instancing.schema.json";
        return true;
    }

    bool operator>>(const TDataDoc<SEXT_mesh_gpu_instancingglTFextension>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        if (!_rData.data.attributes.empty())
        {
            JSONCharValue json_value;
            if (!(TDataDoc<std::map<std::string, std::shared_ptr<SGlTFId>>>(_rData.data.attributes, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("attributes", json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SEXT_mesh_gpu_instancingglTFextension>& _pData, const JSONCharValue& _JsonValue)
    {
        std::shared_ptr<SEXT_mesh_gpu_instancingglTFextension> data_ptr = !!_pData ? _pData : std::make_shared<SEXT_mesh_gpu_instancingglTFextension>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SEXT_mesh_gpu_instancingglTFextension>>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SEXT_mesh_gpu_instancingglTFextension>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SEXT_mesh_gpu_instancingglTFextension>>& _vDatas, const JSONCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SEXT_mesh_gpu_instancingglTFextension>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SEXT_mesh_gpu_instancingglTFextension>>>& _rData, JSONCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SEXT_mesh_gpu_instancingglTFextension>>(_rData, _JsonValue);
    }

    bool operator<<(SEXT_texture_webpglTFextension& _rData, const JSONCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember("source") && _JsonValue["source"].IsInt())
        {
            if (!(_rData.source << _JsonValue["source"])) return false;
        }
        _rData.schemaType = "glTF.EXT_texture_webp.schema.json";
        return true;
    }

    bool operator>>(const TDataDoc<SEXT_texture_webpglTFextension>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        if (!!_rData.data.source)
        {
            JSONCharValue json_value;
            if (!(TDataDoc<SGlTFId>(*_rData.data.source, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("source", json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SEXT_texture_webpglTFextension>& _pData, const JSONCharValue& _JsonValue)
    {
        std::shared_ptr<SEXT_texture_webpglTFextension> data_ptr = !!_pData ? _pData : std::make_shared<SEXT_texture_webpglTFextension>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SEXT_texture_webpglTFextension>>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SEXT_texture_webpglTFextension>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SEXT_texture_webpglTFextension>>& _vDatas, const JSONCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SEXT_texture_webpglTFextension>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SEXT_texture_webpglTFextension>>>& _rData, JSONCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SEXT_texture_webpglTFextension>>(_rData, _JsonValue);
    }

    bool operator<<(SFB_geometry_metadatasceneextension& _rData, const JSONCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember("vertexCount") && _JsonValue["vertexCount"].IsNumber())
        {
            _rData.vertexCount = _JsonValue["vertexCount"].GetFloat();
        }
        if (_JsonValue.HasMember("primitiveCount") && _JsonValue["primitiveCount"].IsNumber())
        {
            _rData.primitiveCount = _JsonValue["primitiveCount"].GetFloat();
        }
        if (_JsonValue.HasMember("sceneBounds") && _JsonValue["sceneBounds"].IsObject())
        {
            if (!(_rData.sceneBounds << _JsonValue["sceneBounds"])) return false;
        }
        _rData.schemaType = "scene.FB_geometry_metadata.schema.json";
        return true;
    }

    bool operator>>(const TDataDoc<SFB_geometry_metadatasceneextension>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        {
            JSONCharValue json_value;
            if (!(TDataDoc<float>(_rData.data.vertexCount, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("vertexCount", json_value, _rData.doc->GetAllocator());
        }
        {
            JSONCharValue json_value;
            if (!(TDataDoc<float>(_rData.data.primitiveCount, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("primitiveCount", json_value, _rData.doc->GetAllocator());
        }
        if (!!_rData.data.sceneBounds)
        {
            JSONCharValue json_value;
            if (!(TDataDoc<SMinimumandmaximumboundingboxextent>(*_rData.data.sceneBounds, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("sceneBounds", json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SFB_geometry_metadatasceneextension>& _pData, const JSONCharValue& _JsonValue)
    {
        std::shared_ptr<SFB_geometry_metadatasceneextension> data_ptr = !!_pData ? _pData : std::make_shared<SFB_geometry_metadatasceneextension>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SFB_geometry_metadatasceneextension>>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SFB_geometry_metadatasceneextension>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SFB_geometry_metadatasceneextension>>& _vDatas, const JSONCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SFB_geometry_metadatasceneextension>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SFB_geometry_metadatasceneextension>>>& _rData, JSONCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SFB_geometry_metadatasceneextension>>(_rData, _JsonValue);
    }

    bool operator<<(SMinimumandmaximumboundingboxextent& _rData, const JSONCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember("min") && _JsonValue["min"].IsArray())
        {
            if (!(_rData.min << _JsonValue["min"])) return false;
        }
        if (_JsonValue.HasMember("max") && _JsonValue["max"].IsArray())
        {
            if (!(_rData.max << _JsonValue["max"])) return false;
        }
        _rData.schemaType = "sceneBounds.schema.json";
        return true;
    }

    bool operator>>(const TDataDoc<SMinimumandmaximumboundingboxextent>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        if (!_rData.data.min.empty())
        {
            JSONCharValue json_value;
            if (!(TDataDoc<std::vector<float>>(_rData.data.min, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("min", json_value, _rData.doc->GetAllocator());
        }
        if (!_rData.data.max.empty())
        {
            JSONCharValue json_value;
            if (!(TDataDoc<std::vector<float>>(_rData.data.max, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("max", json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SMinimumandmaximumboundingboxextent>& _pData, const JSONCharValue& _JsonValue)
    {
        std::shared_ptr<SMinimumandmaximumboundingboxextent> data_ptr = !!_pData ? _pData : std::make_shared<SMinimumandmaximumboundingboxextent>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SMinimumandmaximumboundingboxextent>>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SMinimumandmaximumboundingboxextent>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SMinimumandmaximumboundingboxextent>>& _vDatas, const JSONCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SMinimumandmaximumboundingboxextent>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SMinimumandmaximumboundingboxextent>>>& _rData, JSONCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SMinimumandmaximumboundingboxextent>>(_rData, _JsonValue);
    }

    bool operator<<(SMSFT_lodglTFextension& _rData, const JSONCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember("ids") && _JsonValue["ids"].IsArray())
        {
            if (!(_rData.ids << _JsonValue["ids"])) return false;
        }
        _rData.schemaType = "glTF.MSFT_lod.schema.json";
        return true;
    }

    bool operator>>(const TDataDoc<SMSFT_lodglTFextension>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        if (!_rData.data.ids.empty())
        {
            JSONCharValue json_value;
            if (!(TDataDoc<std::vector<int32_t>>(_rData.data.ids, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("ids", json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SMSFT_lodglTFextension>& _pData, const JSONCharValue& _JsonValue)
    {
        std::shared_ptr<SMSFT_lodglTFextension> data_ptr = !!_pData ? _pData : std::make_shared<SMSFT_lodglTFextension>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SMSFT_lodglTFextension>>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SMSFT_lodglTFextension>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SMSFT_lodglTFextension>>& _vDatas, const JSONCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SMSFT_lodglTFextension>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SMSFT_lodglTFextension>>>& _rData, JSONCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SMSFT_lodglTFextension>>(_rData, _JsonValue);
    }

    bool operator<<(SMSFT_texture_ddsextension& _rData, const JSONCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember("source") && _JsonValue["source"].IsInt())
        {
            if (!(_rData.source << _JsonValue["source"])) return false;
        }
        _rData.schemaType = "glTF.MSFT_texture_dds.schema.json";
        return true;
    }

    bool operator>>(const TDataDoc<SMSFT_texture_ddsextension>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        if (!!_rData.data.source)
        {
            JSONCharValue json_value;
            if (!(TDataDoc<SGlTFId>(*_rData.data.source, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember("source", json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SMSFT_texture_ddsextension>& _pData, const JSONCharValue& _JsonValue)
    {
        std::shared_ptr<SMSFT_texture_ddsextension> data_ptr = !!_pData ? _pData : std::make_shared<SMSFT_texture_ddsextension>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SMSFT_texture_ddsextension>>& _rData, JSONCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SMSFT_texture_ddsextension>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SMSFT_texture_ddsextension>>& _vDatas, const JSONCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SMSFT_texture_ddsextension>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SMSFT_texture_ddsextension>>>& _rData, JSONCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SMSFT_texture_ddsextension>>(_rData, _JsonValue);
    }

}

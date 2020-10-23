/*
 * This software is released under the MIT license.
 *
 * Copyright (c) 2017-2020 Alex Chi, The Code 4 Game Organization
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is furnished to do
 * so, subject to the following conditions:
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

#include "libgltfpch.h"
#include "libgltfparser.h"
#include "libgltf/libgltf.h"

namespace libgltf
{
    bool operator<<(std::shared_ptr<SGlTF>& _pGlTF, const string_t& _sContent)
    {
        GLTFCharDocument json_doc;
        json_doc.Parse(_sContent.c_str());
        if (!json_doc.IsObject()) return false;
        return (_pGlTF << json_doc.GetObject());
    }

    bool operator>>(const std::shared_ptr<SGlTF>& _pGlTF, string_t& _sContent)
    {
        if (!_pGlTF) return false;
        GLTFCharDocument json_doc;
        if (!(TDataDoc<SGlTF>(*_pGlTF, &json_doc) >> json_doc)) return false;
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

    bool operator>>(const TDataDoc<bool>& _rData, GLTFCharValue& _JsonValue)
    {
        _JsonValue.SetBool(_rData.data);
        return true;
    }

    bool operator<<(int32_t& _rData, const GLTFCharValue& _JsonValue)
    {
        if (!_JsonValue.IsInt()) return false;
        _rData = _JsonValue.GetInt();
        return true;
    }

    bool operator>>(const TDataDoc<int32_t>& _rData, GLTFCharValue& _JsonValue)
    {
        _JsonValue.SetInt(_rData.data);
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

    bool operator>>(const TDataDoc<float>& _rData, GLTFCharValue& _JsonValue)
    {
        _JsonValue.SetFloat(_rData.data);
        return true;
    }

    bool operator<<(string_t& _rData, const GLTFCharValue& _JsonValue)
    {
        if (!_JsonValue.IsString()) return false;
        _rData = _JsonValue.GetString();
        return true;
    }

    bool operator>>(const TDataDoc<string_t>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetString(_rData.data.c_str(), _rData.doc->GetAllocator());
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
    bool operator>>(const TDataDoc<std::vector<TData>>& _rData, GLTFCharValue& _JsonValue)
    {
        if (_rData.data.empty() || !_rData.doc) return false;
        GLTFCharValue& json_array = _JsonValue.SetArray();
        for (const TData& data : _rData.data)
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<TData>(data, _rData.doc) >> json_value)) return false;
            json_array.PushBack(json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    template<typename TData>
    bool operator<<(std::map<string_t, TData>& _mDatas, const GLTFCharValue& _JsonValue)
    {
        if (!_JsonValue.IsObject()) return false;
        std::map<string_t, TData> datas;
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
    bool operator>>(const TDataDoc<std::map<string_t, TData>>& _rData, GLTFCharValue& _JsonValue)
    {
        if (_rData.data.empty() || !_rData.doc) return false;
        _JsonValue.SetObject();
        for (const std::pair<string_t, TData>& data : _rData.data)
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<TData>(data.second, _rData.doc) >> json_value)) return false;
            GLTFCharValue json_key(data.first.c_str(), _rData.doc->GetAllocator());
            _JsonValue.AddMember(json_key, json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(SAccessor& _rData, const GLTFCharValue& _JsonValue)
    {
        {
            SGlTFChildofRootProperty& super_ptr = _rData;
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
        if (_JsonValue.HasMember(GLTFTEXT("componentType")) && _JsonValue[GLTFTEXT("componentType")].IsInt())
        {
            _rData.componentType = _JsonValue[GLTFTEXT("componentType")].GetInt();
        }
        if (_JsonValue.HasMember(GLTFTEXT("normalized")) && _JsonValue[GLTFTEXT("normalized")].IsBool())
        {
            _rData.normalized = _JsonValue[GLTFTEXT("normalized")].GetBool();
        }
        if (_JsonValue.HasMember(GLTFTEXT("count")) && _JsonValue[GLTFTEXT("count")].IsInt())
        {
            _rData.count = _JsonValue[GLTFTEXT("count")].GetInt();
        }
        if (_JsonValue.HasMember(GLTFTEXT("type")) && _JsonValue[GLTFTEXT("type")].IsString())
        {
            _rData.type = _JsonValue[GLTFTEXT("type")].GetString();
        }
        if (_JsonValue.HasMember(GLTFTEXT("max")) && _JsonValue[GLTFTEXT("max")].IsArray())
        {
            if (!(_rData.max << _JsonValue[GLTFTEXT("max")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("min")) && _JsonValue[GLTFTEXT("min")].IsArray())
        {
            if (!(_rData.min << _JsonValue[GLTFTEXT("min")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("sparse")) && _JsonValue[GLTFTEXT("sparse")].IsObject())
        {
            if (!(_rData.sparse << _JsonValue[GLTFTEXT("sparse")])) return false;
        }
        _rData.schemaType = GLTFTEXT("accessor.schema.json");
        return true;
    }

    bool operator>>(const TDataDoc<SAccessor>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFChildofRootProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFChildofRootProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        if (!!_rData.data.bufferView)
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<SGlTFId>(*_rData.data.bufferView, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("bufferView"), json_value, _rData.doc->GetAllocator());
        }
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<int32_t>(_rData.data.byteOffset, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("byteOffset"), json_value, _rData.doc->GetAllocator());
        }
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<int32_t>(_rData.data.componentType, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("componentType"), json_value, _rData.doc->GetAllocator());
        }
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<bool>(_rData.data.normalized, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("normalized"), json_value, _rData.doc->GetAllocator());
        }
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<int32_t>(_rData.data.count, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("count"), json_value, _rData.doc->GetAllocator());
        }
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<string_t>(_rData.data.type, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("type"), json_value, _rData.doc->GetAllocator());
        }
        if (!_rData.data.max.empty())
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<std::vector<float>>(_rData.data.max, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("max"), json_value, _rData.doc->GetAllocator());
        }
        if (!_rData.data.min.empty())
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<std::vector<float>>(_rData.data.min, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("min"), json_value, _rData.doc->GetAllocator());
        }
        if (!!_rData.data.sparse)
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<SAccessorSparse>(*_rData.data.sparse, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("sparse"), json_value, _rData.doc->GetAllocator());
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

    bool operator>>(const TDataDoc<std::shared_ptr<SAccessor>>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SAccessor>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SAccessor>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SAccessor>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SAccessor>>>& _rData, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SAccessor>>(_rData, _JsonValue);
    }

    bool operator<<(SAccessorSparseIndices& _rData, const GLTFCharValue& _JsonValue)
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
        if (_JsonValue.HasMember(GLTFTEXT("componentType")) && _JsonValue[GLTFTEXT("componentType")].IsInt())
        {
            _rData.componentType = _JsonValue[GLTFTEXT("componentType")].GetInt();
        }
        _rData.schemaType = GLTFTEXT("accessor.sparse.indices.schema.json");
        return true;
    }

    bool operator>>(const TDataDoc<SAccessorSparseIndices>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        if (!!_rData.data.bufferView)
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<SGlTFId>(*_rData.data.bufferView, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("bufferView"), json_value, _rData.doc->GetAllocator());
        }
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<int32_t>(_rData.data.byteOffset, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("byteOffset"), json_value, _rData.doc->GetAllocator());
        }
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<int32_t>(_rData.data.componentType, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("componentType"), json_value, _rData.doc->GetAllocator());
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

    bool operator>>(const TDataDoc<std::shared_ptr<SAccessorSparseIndices>>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SAccessorSparseIndices>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SAccessorSparseIndices>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SAccessorSparseIndices>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SAccessorSparseIndices>>>& _rData, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SAccessorSparseIndices>>(_rData, _JsonValue);
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

    bool operator>>(const TDataDoc<SAccessorSparse>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<int32_t>(_rData.data.count, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("count"), json_value, _rData.doc->GetAllocator());
        }
        if (!!_rData.data.indices)
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<SAccessorSparseIndices>(*_rData.data.indices, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("indices"), json_value, _rData.doc->GetAllocator());
        }
        if (!!_rData.data.values)
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<SAccessorSparseValues>(*_rData.data.values, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("values"), json_value, _rData.doc->GetAllocator());
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

    bool operator>>(const TDataDoc<std::shared_ptr<SAccessorSparse>>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SAccessorSparse>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SAccessorSparse>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SAccessorSparse>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SAccessorSparse>>>& _rData, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SAccessorSparse>>(_rData, _JsonValue);
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

    bool operator>>(const TDataDoc<SAccessorSparseValues>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        if (!!_rData.data.bufferView)
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<SGlTFId>(*_rData.data.bufferView, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("bufferView"), json_value, _rData.doc->GetAllocator());
        }
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<int32_t>(_rData.data.byteOffset, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("byteOffset"), json_value, _rData.doc->GetAllocator());
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

    bool operator>>(const TDataDoc<std::shared_ptr<SAccessorSparseValues>>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SAccessorSparseValues>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SAccessorSparseValues>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SAccessorSparseValues>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SAccessorSparseValues>>>& _rData, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SAccessorSparseValues>>(_rData, _JsonValue);
    }

    bool operator<<(SAnimationChannel& _rData, const GLTFCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("sampler")) && _JsonValue[GLTFTEXT("sampler")].IsInt())
        {
            if (!(_rData.sampler << _JsonValue[GLTFTEXT("sampler")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("target")) && _JsonValue[GLTFTEXT("target")].IsObject())
        {
            if (!(_rData.target << _JsonValue[GLTFTEXT("target")])) return false;
        }
        _rData.schemaType = GLTFTEXT("animation.channel.schema.json");
        return true;
    }

    bool operator>>(const TDataDoc<SAnimationChannel>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        if (!!_rData.data.sampler)
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<SGlTFId>(*_rData.data.sampler, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("sampler"), json_value, _rData.doc->GetAllocator());
        }
        if (!!_rData.data.target)
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<SAnimationChannelTarget>(*_rData.data.target, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("target"), json_value, _rData.doc->GetAllocator());
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

    bool operator>>(const TDataDoc<std::shared_ptr<SAnimationChannel>>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SAnimationChannel>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SAnimationChannel>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SAnimationChannel>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SAnimationChannel>>>& _rData, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SAnimationChannel>>(_rData, _JsonValue);
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

    bool operator>>(const TDataDoc<SAnimationChannelTarget>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        if (!!_rData.data.node)
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<SGlTFId>(*_rData.data.node, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("node"), json_value, _rData.doc->GetAllocator());
        }
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<string_t>(_rData.data.path, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("path"), json_value, _rData.doc->GetAllocator());
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

    bool operator>>(const TDataDoc<std::shared_ptr<SAnimationChannelTarget>>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SAnimationChannelTarget>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SAnimationChannelTarget>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SAnimationChannelTarget>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SAnimationChannelTarget>>>& _rData, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SAnimationChannelTarget>>(_rData, _JsonValue);
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
        if (_JsonValue.HasMember(GLTFTEXT("interpolation")) && _JsonValue[GLTFTEXT("interpolation")].IsString())
        {
            _rData.interpolation = _JsonValue[GLTFTEXT("interpolation")].GetString();
        }
        if (_JsonValue.HasMember(GLTFTEXT("output")) && _JsonValue[GLTFTEXT("output")].IsInt())
        {
            if (!(_rData.output << _JsonValue[GLTFTEXT("output")])) return false;
        }
        _rData.schemaType = GLTFTEXT("animation.sampler.schema.json");
        return true;
    }

    bool operator>>(const TDataDoc<SAnimationSampler>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        if (!!_rData.data.input)
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<SGlTFId>(*_rData.data.input, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("input"), json_value, _rData.doc->GetAllocator());
        }
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<string_t>(_rData.data.interpolation, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("interpolation"), json_value, _rData.doc->GetAllocator());
        }
        if (!!_rData.data.output)
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<SGlTFId>(*_rData.data.output, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("output"), json_value, _rData.doc->GetAllocator());
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

    bool operator>>(const TDataDoc<std::shared_ptr<SAnimationSampler>>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SAnimationSampler>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SAnimationSampler>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SAnimationSampler>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SAnimationSampler>>>& _rData, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SAnimationSampler>>(_rData, _JsonValue);
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

    bool operator>>(const TDataDoc<SAnimation>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFChildofRootProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFChildofRootProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        if (!_rData.data.channels.empty())
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<std::vector<std::shared_ptr<SAnimationChannel>>>(_rData.data.channels, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("channels"), json_value, _rData.doc->GetAllocator());
        }
        if (!_rData.data.samplers.empty())
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<std::vector<std::shared_ptr<SAnimationSampler>>>(_rData.data.samplers, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("samplers"), json_value, _rData.doc->GetAllocator());
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

    bool operator>>(const TDataDoc<std::shared_ptr<SAnimation>>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SAnimation>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SAnimation>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SAnimation>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SAnimation>>>& _rData, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SAnimation>>(_rData, _JsonValue);
    }

    bool operator<<(SAsset& _rData, const GLTFCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("copyright")) && _JsonValue[GLTFTEXT("copyright")].IsString())
        {
            _rData.copyright = _JsonValue[GLTFTEXT("copyright")].GetString();
        }
        if (_JsonValue.HasMember(GLTFTEXT("generator")) && _JsonValue[GLTFTEXT("generator")].IsString())
        {
            _rData.generator = _JsonValue[GLTFTEXT("generator")].GetString();
        }
        if (_JsonValue.HasMember(GLTFTEXT("version")) && _JsonValue[GLTFTEXT("version")].IsString())
        {
            _rData.version = _JsonValue[GLTFTEXT("version")].GetString();
        }
        if (_JsonValue.HasMember(GLTFTEXT("minVersion")) && _JsonValue[GLTFTEXT("minVersion")].IsString())
        {
            _rData.minVersion = _JsonValue[GLTFTEXT("minVersion")].GetString();
        }
        _rData.schemaType = GLTFTEXT("asset.schema.json");
        return true;
    }

    bool operator>>(const TDataDoc<SAsset>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<string_t>(_rData.data.copyright, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("copyright"), json_value, _rData.doc->GetAllocator());
        }
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<string_t>(_rData.data.generator, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("generator"), json_value, _rData.doc->GetAllocator());
        }
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<string_t>(_rData.data.version, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("version"), json_value, _rData.doc->GetAllocator());
        }
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<string_t>(_rData.data.minVersion, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("minVersion"), json_value, _rData.doc->GetAllocator());
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

    bool operator>>(const TDataDoc<std::shared_ptr<SAsset>>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SAsset>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SAsset>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SAsset>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SAsset>>>& _rData, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SAsset>>(_rData, _JsonValue);
    }

    bool operator<<(SBuffer& _rData, const GLTFCharValue& _JsonValue)
    {
        {
            SGlTFChildofRootProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("uri")) && _JsonValue[GLTFTEXT("uri")].IsString())
        {
            _rData.uri = _JsonValue[GLTFTEXT("uri")].GetString();
        }
        if (_JsonValue.HasMember(GLTFTEXT("byteLength")) && _JsonValue[GLTFTEXT("byteLength")].IsInt())
        {
            _rData.byteLength = _JsonValue[GLTFTEXT("byteLength")].GetInt();
        }
        _rData.schemaType = GLTFTEXT("buffer.schema.json");
        return true;
    }

    bool operator>>(const TDataDoc<SBuffer>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFChildofRootProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFChildofRootProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<string_t>(_rData.data.uri, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("uri"), json_value, _rData.doc->GetAllocator());
        }
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<int32_t>(_rData.data.byteLength, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("byteLength"), json_value, _rData.doc->GetAllocator());
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

    bool operator>>(const TDataDoc<std::shared_ptr<SBuffer>>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SBuffer>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SBuffer>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SBuffer>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SBuffer>>>& _rData, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SBuffer>>(_rData, _JsonValue);
    }

    bool operator<<(SBufferView& _rData, const GLTFCharValue& _JsonValue)
    {
        {
            SGlTFChildofRootProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("buffer")) && _JsonValue[GLTFTEXT("buffer")].IsInt())
        {
            if (!(_rData.buffer << _JsonValue[GLTFTEXT("buffer")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("byteOffset")) && _JsonValue[GLTFTEXT("byteOffset")].IsInt())
        {
            _rData.byteOffset = _JsonValue[GLTFTEXT("byteOffset")].GetInt();
        }
        if (_JsonValue.HasMember(GLTFTEXT("byteLength")) && _JsonValue[GLTFTEXT("byteLength")].IsInt())
        {
            _rData.byteLength = _JsonValue[GLTFTEXT("byteLength")].GetInt();
        }
        if (_JsonValue.HasMember(GLTFTEXT("byteStride")) && _JsonValue[GLTFTEXT("byteStride")].IsInt())
        {
            _rData.byteStride = _JsonValue[GLTFTEXT("byteStride")].GetInt();
        }
        if (_JsonValue.HasMember(GLTFTEXT("target")) && _JsonValue[GLTFTEXT("target")].IsInt())
        {
            _rData.target = _JsonValue[GLTFTEXT("target")].GetInt();
        }
        _rData.schemaType = GLTFTEXT("bufferView.schema.json");
        return true;
    }

    bool operator>>(const TDataDoc<SBufferView>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFChildofRootProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFChildofRootProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        if (!!_rData.data.buffer)
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<SGlTFId>(*_rData.data.buffer, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("buffer"), json_value, _rData.doc->GetAllocator());
        }
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<int32_t>(_rData.data.byteOffset, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("byteOffset"), json_value, _rData.doc->GetAllocator());
        }
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<int32_t>(_rData.data.byteLength, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("byteLength"), json_value, _rData.doc->GetAllocator());
        }
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<int32_t>(_rData.data.byteStride, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("byteStride"), json_value, _rData.doc->GetAllocator());
        }
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<int32_t>(_rData.data.target, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("target"), json_value, _rData.doc->GetAllocator());
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

    bool operator>>(const TDataDoc<std::shared_ptr<SBufferView>>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SBufferView>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SBufferView>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SBufferView>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SBufferView>>>& _rData, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SBufferView>>(_rData, _JsonValue);
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

    bool operator>>(const TDataDoc<SCameraOrthographic>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<float>(_rData.data.xmag, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("xmag"), json_value, _rData.doc->GetAllocator());
        }
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<float>(_rData.data.ymag, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("ymag"), json_value, _rData.doc->GetAllocator());
        }
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<float>(_rData.data.zfar, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("zfar"), json_value, _rData.doc->GetAllocator());
        }
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<float>(_rData.data.znear, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("znear"), json_value, _rData.doc->GetAllocator());
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

    bool operator>>(const TDataDoc<std::shared_ptr<SCameraOrthographic>>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SCameraOrthographic>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SCameraOrthographic>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SCameraOrthographic>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SCameraOrthographic>>>& _rData, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SCameraOrthographic>>(_rData, _JsonValue);
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
        if (_JsonValue.HasMember(GLTFTEXT("zfar")) && _JsonValue[GLTFTEXT("zfar")].IsFloat())
        {
            _rData.zfar = _JsonValue[GLTFTEXT("zfar")].GetFloat();
        }
        if (_JsonValue.HasMember(GLTFTEXT("znear")) && _JsonValue[GLTFTEXT("znear")].IsFloat())
        {
            _rData.znear = _JsonValue[GLTFTEXT("znear")].GetFloat();
        }
        _rData.schemaType = GLTFTEXT("camera.perspective.schema.json");
        return true;
    }

    bool operator>>(const TDataDoc<SCameraPerspective>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<float>(_rData.data.aspectRatio, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("aspectRatio"), json_value, _rData.doc->GetAllocator());
        }
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<float>(_rData.data.yfov, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("yfov"), json_value, _rData.doc->GetAllocator());
        }
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<float>(_rData.data.zfar, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("zfar"), json_value, _rData.doc->GetAllocator());
        }
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<float>(_rData.data.znear, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("znear"), json_value, _rData.doc->GetAllocator());
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

    bool operator>>(const TDataDoc<std::shared_ptr<SCameraPerspective>>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SCameraPerspective>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SCameraPerspective>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SCameraPerspective>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SCameraPerspective>>>& _rData, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SCameraPerspective>>(_rData, _JsonValue);
    }

    bool operator<<(SCamera& _rData, const GLTFCharValue& _JsonValue)
    {
        {
            SGlTFChildofRootProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("orthographic")) && _JsonValue[GLTFTEXT("orthographic")].IsObject())
        {
            if (!(_rData.orthographic << _JsonValue[GLTFTEXT("orthographic")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("perspective")) && _JsonValue[GLTFTEXT("perspective")].IsObject())
        {
            if (!(_rData.perspective << _JsonValue[GLTFTEXT("perspective")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("type")) && _JsonValue[GLTFTEXT("type")].IsString())
        {
            _rData.type = _JsonValue[GLTFTEXT("type")].GetString();
        }
        _rData.schemaType = GLTFTEXT("camera.schema.json");
        return true;
    }

    bool operator>>(const TDataDoc<SCamera>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFChildofRootProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFChildofRootProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        if (!!_rData.data.orthographic)
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<SCameraOrthographic>(*_rData.data.orthographic, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("orthographic"), json_value, _rData.doc->GetAllocator());
        }
        if (!!_rData.data.perspective)
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<SCameraPerspective>(*_rData.data.perspective, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("perspective"), json_value, _rData.doc->GetAllocator());
        }
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<string_t>(_rData.data.type, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("type"), json_value, _rData.doc->GetAllocator());
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

    bool operator>>(const TDataDoc<std::shared_ptr<SCamera>>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SCamera>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SCamera>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SCamera>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SCamera>>>& _rData, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SCamera>>(_rData, _JsonValue);
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
        if (_JsonValue.HasMember(GLTFTEXT("KHR_lights_punctual")) && _JsonValue[GLTFTEXT("KHR_lights_punctual")].IsObject())
        {
            const GLTFCharValue& json_extension = _JsonValue[GLTFTEXT("KHR_lights_punctual")];
            if (json_extension.HasMember(GLTFTEXT("lights")))
            {
                std::shared_ptr<SKHR_lights_punctualglTFextension> extension;
                if (extension << json_extension)
                {
                    _rData.properties.insert(std::make_pair(GLTFTEXT("KHR_lights_punctual"), extension));
                }
            }
            if (json_extension.HasMember(GLTFTEXT("light")))
            {
                std::shared_ptr<SKHR_lights_punctualnodeextension> extension;
                if (extension << json_extension)
                {
                    _rData.properties.insert(std::make_pair(GLTFTEXT("KHR_lights_punctual"), extension));
                }
            }
        }
        if (_JsonValue.HasMember(GLTFTEXT("KHR_materials_clearcoat")) && _JsonValue[GLTFTEXT("KHR_materials_clearcoat")].IsObject())
        {
            std::shared_ptr<SKHR_materials_clearcoatglTFextension> extension;
            if (!(extension << _JsonValue[GLTFTEXT("KHR_materials_clearcoat")])) return false;
            _rData.properties.insert(std::make_pair(GLTFTEXT("KHR_materials_clearcoat"), extension));
        }
        if (_JsonValue.HasMember(GLTFTEXT("KHR_materials_pbrSpecularGlossiness")) && _JsonValue[GLTFTEXT("KHR_materials_pbrSpecularGlossiness")].IsObject())
        {
            std::shared_ptr<SKHR_materials_pbrSpecularGlossinessglTFextension> extension;
            if (!(extension << _JsonValue[GLTFTEXT("KHR_materials_pbrSpecularGlossiness")])) return false;
            _rData.properties.insert(std::make_pair(GLTFTEXT("KHR_materials_pbrSpecularGlossiness"), extension));
        }
        if (_JsonValue.HasMember(GLTFTEXT("KHR_materials_unlit")) && _JsonValue[GLTFTEXT("KHR_materials_unlit")].IsObject())
        {
            std::shared_ptr<SKHR_materials_unlitglTFextension> extension;
            if (!(extension << _JsonValue[GLTFTEXT("KHR_materials_unlit")])) return false;
            _rData.properties.insert(std::make_pair(GLTFTEXT("KHR_materials_unlit"), extension));
        }
        if (_JsonValue.HasMember(GLTFTEXT("KHR_techniques_webgl")) && _JsonValue[GLTFTEXT("KHR_techniques_webgl")].IsObject())
        {
            std::shared_ptr<SKHR_techniques_webglmaterialextension> extension;
            if (!(extension << _JsonValue[GLTFTEXT("KHR_techniques_webgl")])) return false;
            _rData.properties.insert(std::make_pair(GLTFTEXT("KHR_techniques_webgl"), extension));
        }
        if (_JsonValue.HasMember(GLTFTEXT("KHR_texture_transform")) && _JsonValue[GLTFTEXT("KHR_texture_transform")].IsObject())
        {
            std::shared_ptr<SKHR_texture_transformtextureInfoextension> extension;
            if (!(extension << _JsonValue[GLTFTEXT("KHR_texture_transform")])) return false;
            _rData.properties.insert(std::make_pair(GLTFTEXT("KHR_texture_transform"), extension));
        }
        if (_JsonValue.HasMember(GLTFTEXT("ADOBE_materials_thin_transparency")) && _JsonValue[GLTFTEXT("ADOBE_materials_thin_transparency")].IsObject())
        {
            std::shared_ptr<SADOBE_materials_thin_transparencyglTFextension> extension;
            if (!(extension << _JsonValue[GLTFTEXT("ADOBE_materials_thin_transparency")])) return false;
            _rData.properties.insert(std::make_pair(GLTFTEXT("ADOBE_materials_thin_transparency"), extension));
        }
        if (_JsonValue.HasMember(GLTFTEXT("AGI_articulations")) && _JsonValue[GLTFTEXT("AGI_articulations")].IsObject())
        {
            std::shared_ptr<SAGI_articulationsglTFextension> extension;
            if (!(extension << _JsonValue[GLTFTEXT("AGI_articulations")])) return false;
            _rData.properties.insert(std::make_pair(GLTFTEXT("AGI_articulations"), extension));
        }
        if (_JsonValue.HasMember(GLTFTEXT("AGI_stk_metadata")) && _JsonValue[GLTFTEXT("AGI_stk_metadata")].IsObject())
        {
            std::shared_ptr<SAGI_stk_metadataglTFNodeextension> extension;
            if (!(extension << _JsonValue[GLTFTEXT("AGI_stk_metadata")])) return false;
            _rData.properties.insert(std::make_pair(GLTFTEXT("AGI_stk_metadata"), extension));
        }
        if (_JsonValue.HasMember(GLTFTEXT("CESIUM_primitive_outline")) && _JsonValue[GLTFTEXT("CESIUM_primitive_outline")].IsObject())
        {
            std::shared_ptr<SCESIUM_primitive_outlineglTFprimitiveextension> extension;
            if (!(extension << _JsonValue[GLTFTEXT("CESIUM_primitive_outline")])) return false;
            _rData.properties.insert(std::make_pair(GLTFTEXT("CESIUM_primitive_outline"), extension));
        }
        if (_JsonValue.HasMember(GLTFTEXT("EXT_mesh_gpu_instancing")) && _JsonValue[GLTFTEXT("EXT_mesh_gpu_instancing")].IsObject())
        {
            std::shared_ptr<SEXT_mesh_gpu_instancingglTFextension> extension;
            if (!(extension << _JsonValue[GLTFTEXT("EXT_mesh_gpu_instancing")])) return false;
            _rData.properties.insert(std::make_pair(GLTFTEXT("EXT_mesh_gpu_instancing"), extension));
        }
        if (_JsonValue.HasMember(GLTFTEXT("EXT_texture_webp")) && _JsonValue[GLTFTEXT("EXT_texture_webp")].IsObject())
        {
            std::shared_ptr<SEXT_texture_webpglTFextension> extension;
            if (!(extension << _JsonValue[GLTFTEXT("EXT_texture_webp")])) return false;
            _rData.properties.insert(std::make_pair(GLTFTEXT("EXT_texture_webp"), extension));
        }
        if (_JsonValue.HasMember(GLTFTEXT("MSFT_lod")) && _JsonValue[GLTFTEXT("MSFT_lod")].IsObject())
        {
            std::shared_ptr<SMSFT_lodglTFextension> extension;
            if (!(extension << _JsonValue[GLTFTEXT("MSFT_lod")])) return false;
            _rData.properties.insert(std::make_pair(GLTFTEXT("MSFT_lod"), extension));
        }
        if (_JsonValue.HasMember(GLTFTEXT("MSFT_texture_dds")) && _JsonValue[GLTFTEXT("MSFT_texture_dds")].IsObject())
        {
            std::shared_ptr<SMSFT_texture_ddsextension> extension;
            if (!(extension << _JsonValue[GLTFTEXT("MSFT_texture_dds")])) return false;
            _rData.properties.insert(std::make_pair(GLTFTEXT("MSFT_texture_dds"), extension));
        }
        _rData.schemaType = GLTFTEXT("extension.schema.json");
        return true;
    }

    bool operator>>(const TDataDoc<SExtension>& _rData, GLTFCharValue& _JsonValue)
    {
        // Manual code lines
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        for (const std::pair<string_t, std::shared_ptr<SObject>>& extensionProperty : _rData.data.properties)
        {
            const std::shared_ptr<SObject>& extensionValue = extensionProperty.second;
            if (!extensionValue) return false;
            if (extensionValue->schemaType == GLTFTEXT("node.KHR_draco_mesh_compression.schema.json"))
            {
                const SKHR_draco_mesh_compressionextension& extension = *((const SKHR_draco_mesh_compressionextension*)extensionValue.get());
                GLTFCharValue json_value;
                if (!(TDataDoc<SKHR_draco_mesh_compressionextension>(extension, _rData.doc) >> json_value)) return false;
                _JsonValue.AddMember(GLTFTEXT("KHR_draco_mesh_compression"), json_value, _rData.doc->GetAllocator());
            }
            else if (extensionValue->schemaType == GLTFTEXT("node.KHR_lights_punctual.schema.json"))
            {
                const SKHR_lights_punctualnodeextension& extension = *((const SKHR_lights_punctualnodeextension*)extensionValue.get());
                GLTFCharValue json_value;
                if (!(TDataDoc<SKHR_lights_punctualnodeextension>(extension, _rData.doc) >> json_value)) return false;
                _JsonValue.AddMember(GLTFTEXT("KHR_lights_punctual"), json_value, _rData.doc->GetAllocator());
            }
            else if (extensionValue->schemaType == GLTFTEXT("glTF.KHR_lights_punctual.schema.json"))
            {
                const SKHR_lights_punctualglTFextension& extension = *((const SKHR_lights_punctualglTFextension*)extensionValue.get());
                GLTFCharValue json_value;
                if (!(TDataDoc<SKHR_lights_punctualglTFextension>(extension, _rData.doc) >> json_value)) return false;
                _JsonValue.AddMember(GLTFTEXT("KHR_lights_punctual"), json_value, _rData.doc->GetAllocator());
            }
            else if (extensionValue->schemaType == GLTFTEXT("glTF.KHR_materials_clearcoat.schema.json"))
            {
                const SKHR_materials_clearcoatglTFextension& extension = *((const SKHR_materials_clearcoatglTFextension*)extensionValue.get());
                GLTFCharValue json_value;
                if (!(TDataDoc<SKHR_materials_clearcoatglTFextension>(extension, _rData.doc) >> json_value)) return false;
                _JsonValue.AddMember(GLTFTEXT("KHR_materials_clearcoat"), json_value, _rData.doc->GetAllocator());
            }
            else if (extensionValue->schemaType == GLTFTEXT("glTF.KHR_materials_pbrSpecularGlossiness.schema.json"))
            {
                const SKHR_materials_pbrSpecularGlossinessglTFextension& extension = *((const SKHR_materials_pbrSpecularGlossinessglTFextension*)extensionValue.get());
                GLTFCharValue json_value;
                if (!(TDataDoc<SKHR_materials_pbrSpecularGlossinessglTFextension>(extension, _rData.doc) >> json_value)) return false;
                _JsonValue.AddMember(GLTFTEXT("KHR_materials_pbrSpecularGlossiness"), json_value, _rData.doc->GetAllocator());
            }
            else if (extensionValue->schemaType == GLTFTEXT("glTF.KHR_materials_unlit.schema.json"))
            {
                const SKHR_materials_unlitglTFextension& extension = *((const SKHR_materials_unlitglTFextension*)extensionValue.get());
                GLTFCharValue json_value;
                if (!(TDataDoc<SKHR_materials_unlitglTFextension>(extension, _rData.doc) >> json_value)) return false;
                _JsonValue.AddMember(GLTFTEXT("KHR_materials_unlit"), json_value, _rData.doc->GetAllocator());
            }
            else if (extensionValue->schemaType == GLTFTEXT("glTF.KHR_techniques_webgl.schema.json"))
            {
                const SKHR_techniques_webglmaterialextension& extension = *((const SKHR_techniques_webglmaterialextension*)extensionValue.get());
                GLTFCharValue json_value;
                if (!(TDataDoc<SKHR_techniques_webglmaterialextension>(extension, _rData.doc) >> json_value)) return false;
                _JsonValue.AddMember(GLTFTEXT("KHR_techniques_webgl"), json_value, _rData.doc->GetAllocator());
            }
            else if (extensionValue->schemaType == GLTFTEXT("glTF.KHR_texture_transform.schema.json"))
            {
                const SKHR_texture_transformtextureInfoextension& extension = *((const SKHR_texture_transformtextureInfoextension*)extensionValue.get());
                GLTFCharValue json_value;
                if (!(TDataDoc<SKHR_texture_transformtextureInfoextension>(extension, _rData.doc) >> json_value)) return false;
                _JsonValue.AddMember(GLTFTEXT("KHR_texture_transform"), json_value, _rData.doc->GetAllocator());
            }
            else if (extensionValue->schemaType == GLTFTEXT("glTF.ADOBE_materials_thin_transparency.schema.json"))
            {
                const SADOBE_materials_thin_transparencyglTFextension& extension = *((const SADOBE_materials_thin_transparencyglTFextension*)extensionValue.get());
                GLTFCharValue json_value;
                if (!(TDataDoc<SADOBE_materials_thin_transparencyglTFextension>(extension, _rData.doc) >> json_value)) return false;
                _JsonValue.AddMember(GLTFTEXT("ADOBE_materials_thin_transparency"), json_value, _rData.doc->GetAllocator());
            }
            else if (extensionValue->schemaType == GLTFTEXT("glTF.AGI_articulations.schema.json"))
            {
                const SAGI_articulationsglTFextension& extension = *((const SAGI_articulationsglTFextension*)extensionValue.get());
                GLTFCharValue json_value;
                if (!(TDataDoc<SAGI_articulationsglTFextension>(extension, _rData.doc) >> json_value)) return false;
                _JsonValue.AddMember(GLTFTEXT("AGI_articulations"), json_value, _rData.doc->GetAllocator());
            }
            else if (extensionValue->schemaType == GLTFTEXT("glTF.AGI_stk_metadata.schema.json"))
            {
                const SAGI_stk_metadataglTFextension& extension = *((const SAGI_stk_metadataglTFextension*)extensionValue.get());
                GLTFCharValue json_value;
                if (!(TDataDoc<SAGI_stk_metadataglTFextension>(extension, _rData.doc) >> json_value)) return false;
                _JsonValue.AddMember(GLTFTEXT("AGI_stk_metadata"), json_value, _rData.doc->GetAllocator());
            }
            else if (extensionValue->schemaType == GLTFTEXT("glTF.CESIUM_primitive_outline.schema.json"))
            {
                const SCESIUM_primitive_outlineglTFprimitiveextension& extension = *((const SCESIUM_primitive_outlineglTFprimitiveextension*)extensionValue.get());
                GLTFCharValue json_value;
                if (!(TDataDoc<SCESIUM_primitive_outlineglTFprimitiveextension>(extension, _rData.doc) >> json_value)) return false;
                _JsonValue.AddMember(GLTFTEXT("CESIUM_primitive_outline"), json_value, _rData.doc->GetAllocator());
            }
            else if (extensionValue->schemaType == GLTFTEXT("glTF.EXT_mesh_gpu_instancing.schema.json"))
            {
                const SEXT_mesh_gpu_instancingglTFextension& extension = *((const SEXT_mesh_gpu_instancingglTFextension*)extensionValue.get());
                GLTFCharValue json_value;
                if (!(TDataDoc<SEXT_mesh_gpu_instancingglTFextension>(extension, _rData.doc) >> json_value)) return false;
                _JsonValue.AddMember(GLTFTEXT("EXT_mesh_gpu_instancing"), json_value, _rData.doc->GetAllocator());
            }
            else if (extensionValue->schemaType == GLTFTEXT("glTF.EXT_texture_webp.schema.json"))
            {
                const SEXT_texture_webpglTFextension& extension = *((const SEXT_texture_webpglTFextension*)extensionValue.get());
                GLTFCharValue json_value;
                if (!(TDataDoc<SEXT_texture_webpglTFextension>(extension, _rData.doc) >> json_value)) return false;
                _JsonValue.AddMember(GLTFTEXT("EXT_texture_webp"), json_value, _rData.doc->GetAllocator());
            }
            else if (extensionValue->schemaType == GLTFTEXT("glTF.MSFT_lod.schema.json"))
            {
                const SMSFT_lodglTFextension& extension = *((const SMSFT_lodglTFextension*)extensionValue.get());
                GLTFCharValue json_value;
                if (!(TDataDoc<SMSFT_lodglTFextension>(extension, _rData.doc) >> json_value)) return false;
                _JsonValue.AddMember(GLTFTEXT("MSFT_lod"), json_value, _rData.doc->GetAllocator());
            }
            else if (extensionValue->schemaType == GLTFTEXT("glTF.MSFT_texture_dds.schema.json"))
            {
                const SMSFT_texture_ddsextension& extension = *((const SMSFT_texture_ddsextension*)extensionValue.get());
                GLTFCharValue json_value;
                if (!(TDataDoc<SMSFT_texture_ddsextension>(extension, _rData.doc) >> json_value)) return false;
                _JsonValue.AddMember(GLTFTEXT("MSFT_texture_dds"), json_value, _rData.doc->GetAllocator());
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

    bool operator>>(const TDataDoc<std::shared_ptr<SExtension>>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SExtension>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SExtension>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SExtension>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SExtension>>>& _rData, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SExtension>>(_rData, _JsonValue);
    }

    bool operator<<(SExtras& _rData, const GLTFCharValue& _JsonValue)
    {
        _rData.schemaType = GLTFTEXT("extras.schema.json");
        return true;
    }

    bool operator>>(const TDataDoc<SExtras>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
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

    bool operator>>(const TDataDoc<std::shared_ptr<SExtras>>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SExtras>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SExtras>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SExtras>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SExtras>>>& _rData, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SExtras>>(_rData, _JsonValue);
    }

    bool operator<<(SGlTF& _rData, const GLTFCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("extensionsUsed")) && _JsonValue[GLTFTEXT("extensionsUsed")].IsArray())
        {
            if (!(_rData.extensionsUsed << _JsonValue[GLTFTEXT("extensionsUsed")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("extensionsRequired")) && _JsonValue[GLTFTEXT("extensionsRequired")].IsArray())
        {
            if (!(_rData.extensionsRequired << _JsonValue[GLTFTEXT("extensionsRequired")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("accessors")) && _JsonValue[GLTFTEXT("accessors")].IsArray())
        {
            if (!(_rData.accessors << _JsonValue[GLTFTEXT("accessors")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("animations")) && _JsonValue[GLTFTEXT("animations")].IsArray())
        {
            if (!(_rData.animations << _JsonValue[GLTFTEXT("animations")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("asset")) && _JsonValue[GLTFTEXT("asset")].IsObject())
        {
            if (!(_rData.asset << _JsonValue[GLTFTEXT("asset")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("buffers")) && _JsonValue[GLTFTEXT("buffers")].IsArray())
        {
            if (!(_rData.buffers << _JsonValue[GLTFTEXT("buffers")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("bufferViews")) && _JsonValue[GLTFTEXT("bufferViews")].IsArray())
        {
            if (!(_rData.bufferViews << _JsonValue[GLTFTEXT("bufferViews")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("cameras")) && _JsonValue[GLTFTEXT("cameras")].IsArray())
        {
            if (!(_rData.cameras << _JsonValue[GLTFTEXT("cameras")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("images")) && _JsonValue[GLTFTEXT("images")].IsArray())
        {
            if (!(_rData.images << _JsonValue[GLTFTEXT("images")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("materials")) && _JsonValue[GLTFTEXT("materials")].IsArray())
        {
            if (!(_rData.materials << _JsonValue[GLTFTEXT("materials")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("meshes")) && _JsonValue[GLTFTEXT("meshes")].IsArray())
        {
            if (!(_rData.meshes << _JsonValue[GLTFTEXT("meshes")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("nodes")) && _JsonValue[GLTFTEXT("nodes")].IsArray())
        {
            if (!(_rData.nodes << _JsonValue[GLTFTEXT("nodes")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("samplers")) && _JsonValue[GLTFTEXT("samplers")].IsArray())
        {
            if (!(_rData.samplers << _JsonValue[GLTFTEXT("samplers")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("scene")) && _JsonValue[GLTFTEXT("scene")].IsInt())
        {
            if (!(_rData.scene << _JsonValue[GLTFTEXT("scene")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("scenes")) && _JsonValue[GLTFTEXT("scenes")].IsArray())
        {
            if (!(_rData.scenes << _JsonValue[GLTFTEXT("scenes")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("skins")) && _JsonValue[GLTFTEXT("skins")].IsArray())
        {
            if (!(_rData.skins << _JsonValue[GLTFTEXT("skins")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("textures")) && _JsonValue[GLTFTEXT("textures")].IsArray())
        {
            if (!(_rData.textures << _JsonValue[GLTFTEXT("textures")])) return false;
        }
        _rData.schemaType = GLTFTEXT("glTF.schema.json");
        return true;
    }

    bool operator>>(const TDataDoc<SGlTF>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        if (!_rData.data.extensionsUsed.empty())
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<std::vector<string_t>>(_rData.data.extensionsUsed, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("extensionsUsed"), json_value, _rData.doc->GetAllocator());
        }
        if (!_rData.data.extensionsRequired.empty())
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<std::vector<string_t>>(_rData.data.extensionsRequired, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("extensionsRequired"), json_value, _rData.doc->GetAllocator());
        }
        if (!_rData.data.accessors.empty())
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<std::vector<std::shared_ptr<SAccessor>>>(_rData.data.accessors, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("accessors"), json_value, _rData.doc->GetAllocator());
        }
        if (!_rData.data.animations.empty())
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<std::vector<std::shared_ptr<SAnimation>>>(_rData.data.animations, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("animations"), json_value, _rData.doc->GetAllocator());
        }
        if (!!_rData.data.asset)
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<SAsset>(*_rData.data.asset, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("asset"), json_value, _rData.doc->GetAllocator());
        }
        if (!_rData.data.buffers.empty())
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<std::vector<std::shared_ptr<SBuffer>>>(_rData.data.buffers, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("buffers"), json_value, _rData.doc->GetAllocator());
        }
        if (!_rData.data.bufferViews.empty())
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<std::vector<std::shared_ptr<SBufferView>>>(_rData.data.bufferViews, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("bufferViews"), json_value, _rData.doc->GetAllocator());
        }
        if (!_rData.data.cameras.empty())
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<std::vector<std::shared_ptr<SCamera>>>(_rData.data.cameras, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("cameras"), json_value, _rData.doc->GetAllocator());
        }
        if (!_rData.data.images.empty())
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<std::vector<std::shared_ptr<SImage>>>(_rData.data.images, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("images"), json_value, _rData.doc->GetAllocator());
        }
        if (!_rData.data.materials.empty())
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<std::vector<std::shared_ptr<SMaterial>>>(_rData.data.materials, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("materials"), json_value, _rData.doc->GetAllocator());
        }
        if (!_rData.data.meshes.empty())
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<std::vector<std::shared_ptr<SMesh>>>(_rData.data.meshes, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("meshes"), json_value, _rData.doc->GetAllocator());
        }
        if (!_rData.data.nodes.empty())
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<std::vector<std::shared_ptr<SNode>>>(_rData.data.nodes, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("nodes"), json_value, _rData.doc->GetAllocator());
        }
        if (!_rData.data.samplers.empty())
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<std::vector<std::shared_ptr<SSampler>>>(_rData.data.samplers, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("samplers"), json_value, _rData.doc->GetAllocator());
        }
        if (!!_rData.data.scene)
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<SGlTFId>(*_rData.data.scene, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("scene"), json_value, _rData.doc->GetAllocator());
        }
        if (!_rData.data.scenes.empty())
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<std::vector<std::shared_ptr<SScene>>>(_rData.data.scenes, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("scenes"), json_value, _rData.doc->GetAllocator());
        }
        if (!_rData.data.skins.empty())
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<std::vector<std::shared_ptr<SSkin>>>(_rData.data.skins, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("skins"), json_value, _rData.doc->GetAllocator());
        }
        if (!_rData.data.textures.empty())
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<std::vector<std::shared_ptr<STexture>>>(_rData.data.textures, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("textures"), json_value, _rData.doc->GetAllocator());
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

    bool operator>>(const TDataDoc<std::shared_ptr<SGlTF>>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SGlTF>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SGlTF>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SGlTF>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SGlTF>>>& _rData, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SGlTF>>(_rData, _JsonValue);
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

    bool operator>>(const TDataDoc<SGlTFChildofRootProperty>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<string_t>(_rData.data.name, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("name"), json_value, _rData.doc->GetAllocator());
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

    bool operator>>(const TDataDoc<std::shared_ptr<SGlTFChildofRootProperty>>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SGlTFChildofRootProperty>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SGlTFChildofRootProperty>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SGlTFChildofRootProperty>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SGlTFChildofRootProperty>>>& _rData, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SGlTFChildofRootProperty>>(_rData, _JsonValue);
    }

    bool operator<<(SGlTFId& _rData, const GLTFCharValue& _JsonValue)
    {
        if (!(_rData.int32_tValue << _JsonValue)) return false;
        _rData.schemaType = GLTFTEXT("glTFid.schema.json");
        return true;
    }

    bool operator>>(const TDataDoc<SGlTFId>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        if (!(TDataDoc<int32_t>(_rData.data.int32_tValue, _rData.doc) >> _JsonValue)) return false;
        return true;
    }

    bool operator<<(std::shared_ptr<SGlTFId>& _pData, const GLTFCharValue& _JsonValue)
    {
        std::shared_ptr<SGlTFId> data_ptr = !!_pData ? _pData : std::make_shared<SGlTFId>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SGlTFId>>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SGlTFId>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SGlTFId>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SGlTFId>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SGlTFId>>>& _rData, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SGlTFId>>(_rData, _JsonValue);
    }

    bool operator<<(SGlTFProperty& _rData, const GLTFCharValue& _JsonValue)
    {
        if (_JsonValue.HasMember(GLTFTEXT("extensions")) && _JsonValue[GLTFTEXT("extensions")].IsObject())
        {
            if (!(_rData.extensions << _JsonValue[GLTFTEXT("extensions")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("extras")) && _JsonValue[GLTFTEXT("extras")].IsObject())
        {
            if (!(_rData.extras << _JsonValue[GLTFTEXT("extras")])) return false;
        }
        _rData.schemaType = GLTFTEXT("glTFProperty.schema.json");
        return true;
    }

    bool operator>>(const TDataDoc<SGlTFProperty>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        if (!!_rData.data.extensions)
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<SExtension>(*_rData.data.extensions, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("extensions"), json_value, _rData.doc->GetAllocator());
        }
        if (!!_rData.data.extras)
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<SExtras>(*_rData.data.extras, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("extras"), json_value, _rData.doc->GetAllocator());
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

    bool operator>>(const TDataDoc<std::shared_ptr<SGlTFProperty>>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SGlTFProperty>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SGlTFProperty>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SGlTFProperty>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SGlTFProperty>>>& _rData, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SGlTFProperty>>(_rData, _JsonValue);
    }

    bool operator<<(SImage& _rData, const GLTFCharValue& _JsonValue)
    {
        {
            SGlTFChildofRootProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("uri")) && _JsonValue[GLTFTEXT("uri")].IsString())
        {
            _rData.uri = _JsonValue[GLTFTEXT("uri")].GetString();
        }
        if (_JsonValue.HasMember(GLTFTEXT("mimeType")) && _JsonValue[GLTFTEXT("mimeType")].IsString())
        {
            _rData.mimeType = _JsonValue[GLTFTEXT("mimeType")].GetString();
        }
        if (_JsonValue.HasMember(GLTFTEXT("bufferView")) && _JsonValue[GLTFTEXT("bufferView")].IsInt())
        {
            if (!(_rData.bufferView << _JsonValue[GLTFTEXT("bufferView")])) return false;
        }
        _rData.schemaType = GLTFTEXT("image.schema.json");
        return true;
    }

    bool operator>>(const TDataDoc<SImage>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFChildofRootProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFChildofRootProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<string_t>(_rData.data.uri, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("uri"), json_value, _rData.doc->GetAllocator());
        }
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<string_t>(_rData.data.mimeType, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("mimeType"), json_value, _rData.doc->GetAllocator());
        }
        if (!!_rData.data.bufferView)
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<SGlTFId>(*_rData.data.bufferView, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("bufferView"), json_value, _rData.doc->GetAllocator());
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

    bool operator>>(const TDataDoc<std::shared_ptr<SImage>>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SImage>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SImage>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SImage>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SImage>>>& _rData, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SImage>>(_rData, _JsonValue);
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

    bool operator>>(const TDataDoc<SMaterialNormalTextureInfo>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const STextureInfo& super_ptr = _rData.data;
            if (!(TDataDoc<STextureInfo>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<float>(_rData.data.scale, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("scale"), json_value, _rData.doc->GetAllocator());
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

    bool operator>>(const TDataDoc<std::shared_ptr<SMaterialNormalTextureInfo>>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SMaterialNormalTextureInfo>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SMaterialNormalTextureInfo>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SMaterialNormalTextureInfo>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SMaterialNormalTextureInfo>>>& _rData, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SMaterialNormalTextureInfo>>(_rData, _JsonValue);
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

    bool operator>>(const TDataDoc<SMaterialOcclusionTextureInfo>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const STextureInfo& super_ptr = _rData.data;
            if (!(TDataDoc<STextureInfo>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<float>(_rData.data.strength, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("strength"), json_value, _rData.doc->GetAllocator());
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

    bool operator>>(const TDataDoc<std::shared_ptr<SMaterialOcclusionTextureInfo>>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SMaterialOcclusionTextureInfo>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SMaterialOcclusionTextureInfo>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SMaterialOcclusionTextureInfo>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SMaterialOcclusionTextureInfo>>>& _rData, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SMaterialOcclusionTextureInfo>>(_rData, _JsonValue);
    }

    bool operator<<(SMaterialPBRMetallicRoughness& _rData, const GLTFCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("baseColorFactor")) && _JsonValue[GLTFTEXT("baseColorFactor")].IsArray())
        {
            if (!(_rData.baseColorFactor << _JsonValue[GLTFTEXT("baseColorFactor")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("baseColorTexture")) && _JsonValue[GLTFTEXT("baseColorTexture")].IsObject())
        {
            if (!(_rData.baseColorTexture << _JsonValue[GLTFTEXT("baseColorTexture")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("metallicFactor")) && _JsonValue[GLTFTEXT("metallicFactor")].IsFloat())
        {
            _rData.metallicFactor = _JsonValue[GLTFTEXT("metallicFactor")].GetFloat();
        }
        if (_JsonValue.HasMember(GLTFTEXT("roughnessFactor")) && _JsonValue[GLTFTEXT("roughnessFactor")].IsFloat())
        {
            _rData.roughnessFactor = _JsonValue[GLTFTEXT("roughnessFactor")].GetFloat();
        }
        if (_JsonValue.HasMember(GLTFTEXT("metallicRoughnessTexture")) && _JsonValue[GLTFTEXT("metallicRoughnessTexture")].IsObject())
        {
            if (!(_rData.metallicRoughnessTexture << _JsonValue[GLTFTEXT("metallicRoughnessTexture")])) return false;
        }
        _rData.schemaType = GLTFTEXT("material.pbrMetallicRoughness.schema.json");
        return true;
    }

    bool operator>>(const TDataDoc<SMaterialPBRMetallicRoughness>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        if (!_rData.data.baseColorFactor.empty())
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<std::vector<float>>(_rData.data.baseColorFactor, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("baseColorFactor"), json_value, _rData.doc->GetAllocator());
        }
        if (!!_rData.data.baseColorTexture)
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<STextureInfo>(*_rData.data.baseColorTexture, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("baseColorTexture"), json_value, _rData.doc->GetAllocator());
        }
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<float>(_rData.data.metallicFactor, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("metallicFactor"), json_value, _rData.doc->GetAllocator());
        }
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<float>(_rData.data.roughnessFactor, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("roughnessFactor"), json_value, _rData.doc->GetAllocator());
        }
        if (!!_rData.data.metallicRoughnessTexture)
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<STextureInfo>(*_rData.data.metallicRoughnessTexture, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("metallicRoughnessTexture"), json_value, _rData.doc->GetAllocator());
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

    bool operator>>(const TDataDoc<std::shared_ptr<SMaterialPBRMetallicRoughness>>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SMaterialPBRMetallicRoughness>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SMaterialPBRMetallicRoughness>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SMaterialPBRMetallicRoughness>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SMaterialPBRMetallicRoughness>>>& _rData, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SMaterialPBRMetallicRoughness>>(_rData, _JsonValue);
    }

    bool operator<<(SMaterial& _rData, const GLTFCharValue& _JsonValue)
    {
        {
            SGlTFChildofRootProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("pbrMetallicRoughness")) && _JsonValue[GLTFTEXT("pbrMetallicRoughness")].IsObject())
        {
            if (!(_rData.pbrMetallicRoughness << _JsonValue[GLTFTEXT("pbrMetallicRoughness")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("normalTexture")) && _JsonValue[GLTFTEXT("normalTexture")].IsObject())
        {
            if (!(_rData.normalTexture << _JsonValue[GLTFTEXT("normalTexture")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("occlusionTexture")) && _JsonValue[GLTFTEXT("occlusionTexture")].IsObject())
        {
            if (!(_rData.occlusionTexture << _JsonValue[GLTFTEXT("occlusionTexture")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("emissiveTexture")) && _JsonValue[GLTFTEXT("emissiveTexture")].IsObject())
        {
            if (!(_rData.emissiveTexture << _JsonValue[GLTFTEXT("emissiveTexture")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("emissiveFactor")) && _JsonValue[GLTFTEXT("emissiveFactor")].IsArray())
        {
            if (!(_rData.emissiveFactor << _JsonValue[GLTFTEXT("emissiveFactor")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("alphaMode")) && _JsonValue[GLTFTEXT("alphaMode")].IsString())
        {
            _rData.alphaMode = _JsonValue[GLTFTEXT("alphaMode")].GetString();
        }
        if (_JsonValue.HasMember(GLTFTEXT("alphaCutoff")) && _JsonValue[GLTFTEXT("alphaCutoff")].IsFloat())
        {
            _rData.alphaCutoff = _JsonValue[GLTFTEXT("alphaCutoff")].GetFloat();
        }
        if (_JsonValue.HasMember(GLTFTEXT("doubleSided")) && _JsonValue[GLTFTEXT("doubleSided")].IsBool())
        {
            _rData.doubleSided = _JsonValue[GLTFTEXT("doubleSided")].GetBool();
        }
        _rData.schemaType = GLTFTEXT("material.schema.json");
        return true;
    }

    bool operator>>(const TDataDoc<SMaterial>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFChildofRootProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFChildofRootProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        if (!!_rData.data.pbrMetallicRoughness)
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<SMaterialPBRMetallicRoughness>(*_rData.data.pbrMetallicRoughness, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("pbrMetallicRoughness"), json_value, _rData.doc->GetAllocator());
        }
        if (!!_rData.data.normalTexture)
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<SMaterialNormalTextureInfo>(*_rData.data.normalTexture, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("normalTexture"), json_value, _rData.doc->GetAllocator());
        }
        if (!!_rData.data.occlusionTexture)
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<SMaterialOcclusionTextureInfo>(*_rData.data.occlusionTexture, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("occlusionTexture"), json_value, _rData.doc->GetAllocator());
        }
        if (!!_rData.data.emissiveTexture)
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<STextureInfo>(*_rData.data.emissiveTexture, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("emissiveTexture"), json_value, _rData.doc->GetAllocator());
        }
        if (!_rData.data.emissiveFactor.empty())
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<std::vector<float>>(_rData.data.emissiveFactor, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("emissiveFactor"), json_value, _rData.doc->GetAllocator());
        }
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<string_t>(_rData.data.alphaMode, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("alphaMode"), json_value, _rData.doc->GetAllocator());
        }
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<float>(_rData.data.alphaCutoff, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("alphaCutoff"), json_value, _rData.doc->GetAllocator());
        }
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<bool>(_rData.data.doubleSided, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("doubleSided"), json_value, _rData.doc->GetAllocator());
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

    bool operator>>(const TDataDoc<std::shared_ptr<SMaterial>>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SMaterial>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SMaterial>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SMaterial>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SMaterial>>>& _rData, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SMaterial>>(_rData, _JsonValue);
    }

    bool operator<<(SMeshPrimitive& _rData, const GLTFCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("attributes")) && _JsonValue[GLTFTEXT("attributes")].IsObject())
        {
            if (!(_rData.attributes << _JsonValue[GLTFTEXT("attributes")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("indices")) && _JsonValue[GLTFTEXT("indices")].IsInt())
        {
            if (!(_rData.indices << _JsonValue[GLTFTEXT("indices")])) return false;
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

    bool operator>>(const TDataDoc<SMeshPrimitive>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        if (!_rData.data.attributes.empty())
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<std::map<string_t, std::shared_ptr<SGlTFId>>>(_rData.data.attributes, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("attributes"), json_value, _rData.doc->GetAllocator());
        }
        if (!!_rData.data.indices)
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<SGlTFId>(*_rData.data.indices, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("indices"), json_value, _rData.doc->GetAllocator());
        }
        if (!!_rData.data.material)
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<SGlTFId>(*_rData.data.material, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("material"), json_value, _rData.doc->GetAllocator());
        }
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<int32_t>(_rData.data.mode, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("mode"), json_value, _rData.doc->GetAllocator());
        }
        if (!_rData.data.targets.empty())
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<std::vector<std::shared_ptr<SGlTFId>>>(_rData.data.targets, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("targets"), json_value, _rData.doc->GetAllocator());
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

    bool operator>>(const TDataDoc<std::shared_ptr<SMeshPrimitive>>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SMeshPrimitive>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SMeshPrimitive>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SMeshPrimitive>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SMeshPrimitive>>>& _rData, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SMeshPrimitive>>(_rData, _JsonValue);
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

    bool operator>>(const TDataDoc<SMesh>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFChildofRootProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFChildofRootProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        if (!_rData.data.primitives.empty())
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<std::vector<std::shared_ptr<SMeshPrimitive>>>(_rData.data.primitives, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("primitives"), json_value, _rData.doc->GetAllocator());
        }
        if (!_rData.data.weights.empty())
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<std::vector<float>>(_rData.data.weights, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("weights"), json_value, _rData.doc->GetAllocator());
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

    bool operator>>(const TDataDoc<std::shared_ptr<SMesh>>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SMesh>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SMesh>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SMesh>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SMesh>>>& _rData, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SMesh>>(_rData, _JsonValue);
    }

    bool operator<<(SNode& _rData, const GLTFCharValue& _JsonValue)
    {
        {
            SGlTFChildofRootProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("camera")) && _JsonValue[GLTFTEXT("camera")].IsInt())
        {
            if (!(_rData.camera << _JsonValue[GLTFTEXT("camera")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("children")) && _JsonValue[GLTFTEXT("children")].IsArray())
        {
            if (!(_rData.children << _JsonValue[GLTFTEXT("children")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("skin")) && _JsonValue[GLTFTEXT("skin")].IsInt())
        {
            if (!(_rData.skin << _JsonValue[GLTFTEXT("skin")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("matrix")) && _JsonValue[GLTFTEXT("matrix")].IsArray())
        {
            if (!(_rData.matrix << _JsonValue[GLTFTEXT("matrix")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("mesh")) && _JsonValue[GLTFTEXT("mesh")].IsInt())
        {
            if (!(_rData.mesh << _JsonValue[GLTFTEXT("mesh")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("rotation")) && _JsonValue[GLTFTEXT("rotation")].IsArray())
        {
            if (!(_rData.rotation << _JsonValue[GLTFTEXT("rotation")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("scale")) && _JsonValue[GLTFTEXT("scale")].IsArray())
        {
            if (!(_rData.scale << _JsonValue[GLTFTEXT("scale")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("translation")) && _JsonValue[GLTFTEXT("translation")].IsArray())
        {
            if (!(_rData.translation << _JsonValue[GLTFTEXT("translation")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("weights")) && _JsonValue[GLTFTEXT("weights")].IsArray())
        {
            if (!(_rData.weights << _JsonValue[GLTFTEXT("weights")])) return false;
        }
        _rData.schemaType = GLTFTEXT("node.schema.json");
        return true;
    }

    bool operator>>(const TDataDoc<SNode>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFChildofRootProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFChildofRootProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        if (!!_rData.data.camera)
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<SGlTFId>(*_rData.data.camera, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("camera"), json_value, _rData.doc->GetAllocator());
        }
        if (!_rData.data.children.empty())
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<std::vector<std::shared_ptr<SGlTFId>>>(_rData.data.children, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("children"), json_value, _rData.doc->GetAllocator());
        }
        if (!!_rData.data.skin)
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<SGlTFId>(*_rData.data.skin, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("skin"), json_value, _rData.doc->GetAllocator());
        }
        if (!_rData.data.matrix.empty())
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<std::vector<float>>(_rData.data.matrix, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("matrix"), json_value, _rData.doc->GetAllocator());
        }
        if (!!_rData.data.mesh)
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<SGlTFId>(*_rData.data.mesh, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("mesh"), json_value, _rData.doc->GetAllocator());
        }
        if (!_rData.data.rotation.empty())
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<std::vector<float>>(_rData.data.rotation, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("rotation"), json_value, _rData.doc->GetAllocator());
        }
        if (!_rData.data.scale.empty())
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<std::vector<float>>(_rData.data.scale, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("scale"), json_value, _rData.doc->GetAllocator());
        }
        if (!_rData.data.translation.empty())
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<std::vector<float>>(_rData.data.translation, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("translation"), json_value, _rData.doc->GetAllocator());
        }
        if (!_rData.data.weights.empty())
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<std::vector<float>>(_rData.data.weights, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("weights"), json_value, _rData.doc->GetAllocator());
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

    bool operator>>(const TDataDoc<std::shared_ptr<SNode>>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SNode>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SNode>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SNode>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SNode>>>& _rData, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SNode>>(_rData, _JsonValue);
    }

    bool operator<<(SSampler& _rData, const GLTFCharValue& _JsonValue)
    {
        {
            SGlTFChildofRootProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("magFilter")) && _JsonValue[GLTFTEXT("magFilter")].IsInt())
        {
            _rData.magFilter = _JsonValue[GLTFTEXT("magFilter")].GetInt();
        }
        if (_JsonValue.HasMember(GLTFTEXT("minFilter")) && _JsonValue[GLTFTEXT("minFilter")].IsInt())
        {
            _rData.minFilter = _JsonValue[GLTFTEXT("minFilter")].GetInt();
        }
        if (_JsonValue.HasMember(GLTFTEXT("wrapS")) && _JsonValue[GLTFTEXT("wrapS")].IsInt())
        {
            _rData.wrapS = _JsonValue[GLTFTEXT("wrapS")].GetInt();
        }
        if (_JsonValue.HasMember(GLTFTEXT("wrapT")) && _JsonValue[GLTFTEXT("wrapT")].IsInt())
        {
            _rData.wrapT = _JsonValue[GLTFTEXT("wrapT")].GetInt();
        }
        _rData.schemaType = GLTFTEXT("sampler.schema.json");
        return true;
    }

    bool operator>>(const TDataDoc<SSampler>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFChildofRootProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFChildofRootProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<int32_t>(_rData.data.magFilter, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("magFilter"), json_value, _rData.doc->GetAllocator());
        }
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<int32_t>(_rData.data.minFilter, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("minFilter"), json_value, _rData.doc->GetAllocator());
        }
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<int32_t>(_rData.data.wrapS, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("wrapS"), json_value, _rData.doc->GetAllocator());
        }
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<int32_t>(_rData.data.wrapT, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("wrapT"), json_value, _rData.doc->GetAllocator());
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

    bool operator>>(const TDataDoc<std::shared_ptr<SSampler>>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SSampler>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SSampler>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SSampler>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SSampler>>>& _rData, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SSampler>>(_rData, _JsonValue);
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

    bool operator>>(const TDataDoc<SScene>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFChildofRootProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFChildofRootProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        if (!_rData.data.nodes.empty())
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<std::vector<std::shared_ptr<SGlTFId>>>(_rData.data.nodes, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("nodes"), json_value, _rData.doc->GetAllocator());
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

    bool operator>>(const TDataDoc<std::shared_ptr<SScene>>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SScene>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SScene>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SScene>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SScene>>>& _rData, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SScene>>(_rData, _JsonValue);
    }

    bool operator<<(SSkin& _rData, const GLTFCharValue& _JsonValue)
    {
        {
            SGlTFChildofRootProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("inverseBindMatrices")) && _JsonValue[GLTFTEXT("inverseBindMatrices")].IsInt())
        {
            if (!(_rData.inverseBindMatrices << _JsonValue[GLTFTEXT("inverseBindMatrices")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("skeleton")) && _JsonValue[GLTFTEXT("skeleton")].IsInt())
        {
            if (!(_rData.skeleton << _JsonValue[GLTFTEXT("skeleton")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("joints")) && _JsonValue[GLTFTEXT("joints")].IsArray())
        {
            if (!(_rData.joints << _JsonValue[GLTFTEXT("joints")])) return false;
        }
        _rData.schemaType = GLTFTEXT("skin.schema.json");
        return true;
    }

    bool operator>>(const TDataDoc<SSkin>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFChildofRootProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFChildofRootProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        if (!!_rData.data.inverseBindMatrices)
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<SGlTFId>(*_rData.data.inverseBindMatrices, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("inverseBindMatrices"), json_value, _rData.doc->GetAllocator());
        }
        if (!!_rData.data.skeleton)
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<SGlTFId>(*_rData.data.skeleton, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("skeleton"), json_value, _rData.doc->GetAllocator());
        }
        if (!_rData.data.joints.empty())
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<std::vector<std::shared_ptr<SGlTFId>>>(_rData.data.joints, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("joints"), json_value, _rData.doc->GetAllocator());
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

    bool operator>>(const TDataDoc<std::shared_ptr<SSkin>>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SSkin>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SSkin>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SSkin>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SSkin>>>& _rData, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SSkin>>(_rData, _JsonValue);
    }

    bool operator<<(STexture& _rData, const GLTFCharValue& _JsonValue)
    {
        {
            SGlTFChildofRootProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("sampler")) && _JsonValue[GLTFTEXT("sampler")].IsInt())
        {
            if (!(_rData.sampler << _JsonValue[GLTFTEXT("sampler")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("source")) && _JsonValue[GLTFTEXT("source")].IsInt())
        {
            if (!(_rData.source << _JsonValue[GLTFTEXT("source")])) return false;
        }
        _rData.schemaType = GLTFTEXT("texture.schema.json");
        return true;
    }

    bool operator>>(const TDataDoc<STexture>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFChildofRootProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFChildofRootProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        if (!!_rData.data.sampler)
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<SGlTFId>(*_rData.data.sampler, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("sampler"), json_value, _rData.doc->GetAllocator());
        }
        if (!!_rData.data.source)
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<SGlTFId>(*_rData.data.source, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("source"), json_value, _rData.doc->GetAllocator());
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

    bool operator>>(const TDataDoc<std::shared_ptr<STexture>>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<STexture>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<STexture>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<STexture>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<STexture>>>& _rData, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<STexture>>(_rData, _JsonValue);
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

    bool operator>>(const TDataDoc<STextureInfo>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        if (!!_rData.data.index)
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<SGlTFId>(*_rData.data.index, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("index"), json_value, _rData.doc->GetAllocator());
        }
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<int32_t>(_rData.data.texCoord, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("texCoord"), json_value, _rData.doc->GetAllocator());
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

    bool operator>>(const TDataDoc<std::shared_ptr<STextureInfo>>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<STextureInfo>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<STextureInfo>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<STextureInfo>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<STextureInfo>>>& _rData, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<STextureInfo>>(_rData, _JsonValue);
    }

    bool operator<<(SKHR_draco_mesh_compressionextension& _rData, const GLTFCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("bufferView")) && _JsonValue[GLTFTEXT("bufferView")].IsInt())
        {
            if (!(_rData.bufferView << _JsonValue[GLTFTEXT("bufferView")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("attributes")) && _JsonValue[GLTFTEXT("attributes")].IsObject())
        {
            if (!(_rData.attributes << _JsonValue[GLTFTEXT("attributes")])) return false;
        }
        _rData.schemaType = GLTFTEXT("node.KHR_draco_mesh_compression.schema.json");
        return true;
    }

    bool operator>>(const TDataDoc<SKHR_draco_mesh_compressionextension>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        if (!!_rData.data.bufferView)
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<SGlTFId>(*_rData.data.bufferView, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("bufferView"), json_value, _rData.doc->GetAllocator());
        }
        if (!_rData.data.attributes.empty())
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<std::map<string_t, std::shared_ptr<SGlTFId>>>(_rData.data.attributes, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("attributes"), json_value, _rData.doc->GetAllocator());
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

    bool operator>>(const TDataDoc<std::shared_ptr<SKHR_draco_mesh_compressionextension>>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SKHR_draco_mesh_compressionextension>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SKHR_draco_mesh_compressionextension>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SKHR_draco_mesh_compressionextension>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SKHR_draco_mesh_compressionextension>>>& _rData, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SKHR_draco_mesh_compressionextension>>(_rData, _JsonValue);
    }

    bool operator<<(SKHR_lights_punctualglTFextension& _rData, const GLTFCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("lights")) && _JsonValue[GLTFTEXT("lights")].IsArray())
        {
            if (!(_rData.lights << _JsonValue[GLTFTEXT("lights")])) return false;
        }
        _rData.schemaType = GLTFTEXT("glTF.KHR_lights_punctual.schema.json");
        return true;
    }

    bool operator>>(const TDataDoc<SKHR_lights_punctualglTFextension>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        if (!_rData.data.lights.empty())
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<std::vector<std::shared_ptr<SLight>>>(_rData.data.lights, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("lights"), json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SKHR_lights_punctualglTFextension>& _pData, const GLTFCharValue& _JsonValue)
    {
        std::shared_ptr<SKHR_lights_punctualglTFextension> data_ptr = !!_pData ? _pData : std::make_shared<SKHR_lights_punctualglTFextension>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SKHR_lights_punctualglTFextension>>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SKHR_lights_punctualglTFextension>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SKHR_lights_punctualglTFextension>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SKHR_lights_punctualglTFextension>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SKHR_lights_punctualglTFextension>>>& _rData, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SKHR_lights_punctualglTFextension>>(_rData, _JsonValue);
    }

    bool operator<<(SLight& _rData, const GLTFCharValue& _JsonValue)
    {
        {
            SGlTFChildofRootProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("color")) && _JsonValue[GLTFTEXT("color")].IsArray())
        {
            if (!(_rData.color << _JsonValue[GLTFTEXT("color")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("intensity")) && _JsonValue[GLTFTEXT("intensity")].IsFloat())
        {
            _rData.intensity = _JsonValue[GLTFTEXT("intensity")].GetFloat();
        }
        if (_JsonValue.HasMember(GLTFTEXT("spot")) && _JsonValue[GLTFTEXT("spot")].IsObject())
        {
            if (!(_rData.spot << _JsonValue[GLTFTEXT("spot")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("type")) && _JsonValue[GLTFTEXT("type")].IsString())
        {
            _rData.type = _JsonValue[GLTFTEXT("type")].GetString();
        }
        if (_JsonValue.HasMember(GLTFTEXT("range")) && _JsonValue[GLTFTEXT("range")].IsFloat())
        {
            _rData.range = _JsonValue[GLTFTEXT("range")].GetFloat();
        }
        _rData.schemaType = GLTFTEXT("light.schema.json");
        return true;
    }

    bool operator>>(const TDataDoc<SLight>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFChildofRootProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFChildofRootProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        if (!_rData.data.color.empty())
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<std::vector<float>>(_rData.data.color, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("color"), json_value, _rData.doc->GetAllocator());
        }
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<float>(_rData.data.intensity, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("intensity"), json_value, _rData.doc->GetAllocator());
        }
        if (!!_rData.data.spot)
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<SLightspot>(*_rData.data.spot, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("spot"), json_value, _rData.doc->GetAllocator());
        }
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<string_t>(_rData.data.type, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("type"), json_value, _rData.doc->GetAllocator());
        }
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<float>(_rData.data.range, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("range"), json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SLight>& _pData, const GLTFCharValue& _JsonValue)
    {
        std::shared_ptr<SLight> data_ptr = !!_pData ? _pData : std::make_shared<SLight>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SLight>>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SLight>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SLight>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SLight>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SLight>>>& _rData, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SLight>>(_rData, _JsonValue);
    }

    bool operator<<(SLightspot& _rData, const GLTFCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("innerConeAngle")) && _JsonValue[GLTFTEXT("innerConeAngle")].IsFloat())
        {
            _rData.innerConeAngle = _JsonValue[GLTFTEXT("innerConeAngle")].GetFloat();
        }
        if (_JsonValue.HasMember(GLTFTEXT("outerConeAngle")) && _JsonValue[GLTFTEXT("outerConeAngle")].IsFloat())
        {
            _rData.outerConeAngle = _JsonValue[GLTFTEXT("outerConeAngle")].GetFloat();
        }
        _rData.schemaType = GLTFTEXT("light.spot.schema.json");
        return true;
    }

    bool operator>>(const TDataDoc<SLightspot>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<float>(_rData.data.innerConeAngle, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("innerConeAngle"), json_value, _rData.doc->GetAllocator());
        }
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<float>(_rData.data.outerConeAngle, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("outerConeAngle"), json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SLightspot>& _pData, const GLTFCharValue& _JsonValue)
    {
        std::shared_ptr<SLightspot> data_ptr = !!_pData ? _pData : std::make_shared<SLightspot>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SLightspot>>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SLightspot>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SLightspot>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SLightspot>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SLightspot>>>& _rData, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SLightspot>>(_rData, _JsonValue);
    }

    bool operator<<(SKHR_lights_punctualnodeextension& _rData, const GLTFCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("light")) && _JsonValue[GLTFTEXT("light")].IsInt())
        {
            if (!(_rData.light << _JsonValue[GLTFTEXT("light")])) return false;
        }
        _rData.schemaType = GLTFTEXT("node.KHR_lights_punctual.schema.json");
        return true;
    }

    bool operator>>(const TDataDoc<SKHR_lights_punctualnodeextension>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        if (!!_rData.data.light)
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<SGlTFId>(*_rData.data.light, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("light"), json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SKHR_lights_punctualnodeextension>& _pData, const GLTFCharValue& _JsonValue)
    {
        std::shared_ptr<SKHR_lights_punctualnodeextension> data_ptr = !!_pData ? _pData : std::make_shared<SKHR_lights_punctualnodeextension>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SKHR_lights_punctualnodeextension>>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SKHR_lights_punctualnodeextension>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SKHR_lights_punctualnodeextension>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SKHR_lights_punctualnodeextension>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SKHR_lights_punctualnodeextension>>>& _rData, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SKHR_lights_punctualnodeextension>>(_rData, _JsonValue);
    }

    bool operator<<(SKHR_materials_clearcoatglTFextension& _rData, const GLTFCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("clearcoatFactor")) && _JsonValue[GLTFTEXT("clearcoatFactor")].IsFloat())
        {
            _rData.clearcoatFactor = _JsonValue[GLTFTEXT("clearcoatFactor")].GetFloat();
        }
        if (_JsonValue.HasMember(GLTFTEXT("clearcoatTexture")) && _JsonValue[GLTFTEXT("clearcoatTexture")].IsObject())
        {
            if (!(_rData.clearcoatTexture << _JsonValue[GLTFTEXT("clearcoatTexture")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("clearcoatRoughnessFactor")) && _JsonValue[GLTFTEXT("clearcoatRoughnessFactor")].IsFloat())
        {
            _rData.clearcoatRoughnessFactor = _JsonValue[GLTFTEXT("clearcoatRoughnessFactor")].GetFloat();
        }
        if (_JsonValue.HasMember(GLTFTEXT("clearcoatRoughnessTexture")) && _JsonValue[GLTFTEXT("clearcoatRoughnessTexture")].IsObject())
        {
            if (!(_rData.clearcoatRoughnessTexture << _JsonValue[GLTFTEXT("clearcoatRoughnessTexture")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("clearcoatNormalTexture")) && _JsonValue[GLTFTEXT("clearcoatNormalTexture")].IsObject())
        {
            if (!(_rData.clearcoatNormalTexture << _JsonValue[GLTFTEXT("clearcoatNormalTexture")])) return false;
        }
        _rData.schemaType = GLTFTEXT("glTF.KHR_materials_clearcoat.schema.json");
        return true;
    }

    bool operator>>(const TDataDoc<SKHR_materials_clearcoatglTFextension>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<float>(_rData.data.clearcoatFactor, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("clearcoatFactor"), json_value, _rData.doc->GetAllocator());
        }
        if (!!_rData.data.clearcoatTexture)
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<STextureInfo>(*_rData.data.clearcoatTexture, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("clearcoatTexture"), json_value, _rData.doc->GetAllocator());
        }
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<float>(_rData.data.clearcoatRoughnessFactor, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("clearcoatRoughnessFactor"), json_value, _rData.doc->GetAllocator());
        }
        if (!!_rData.data.clearcoatRoughnessTexture)
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<STextureInfo>(*_rData.data.clearcoatRoughnessTexture, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("clearcoatRoughnessTexture"), json_value, _rData.doc->GetAllocator());
        }
        if (!!_rData.data.clearcoatNormalTexture)
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<SMaterialNormalTextureInfo>(*_rData.data.clearcoatNormalTexture, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("clearcoatNormalTexture"), json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SKHR_materials_clearcoatglTFextension>& _pData, const GLTFCharValue& _JsonValue)
    {
        std::shared_ptr<SKHR_materials_clearcoatglTFextension> data_ptr = !!_pData ? _pData : std::make_shared<SKHR_materials_clearcoatglTFextension>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SKHR_materials_clearcoatglTFextension>>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SKHR_materials_clearcoatglTFextension>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SKHR_materials_clearcoatglTFextension>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SKHR_materials_clearcoatglTFextension>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SKHR_materials_clearcoatglTFextension>>>& _rData, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SKHR_materials_clearcoatglTFextension>>(_rData, _JsonValue);
    }

    bool operator<<(SKHR_materials_pbrSpecularGlossinessglTFextension& _rData, const GLTFCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("diffuseFactor")) && _JsonValue[GLTFTEXT("diffuseFactor")].IsArray())
        {
            if (!(_rData.diffuseFactor << _JsonValue[GLTFTEXT("diffuseFactor")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("diffuseTexture")) && _JsonValue[GLTFTEXT("diffuseTexture")].IsObject())
        {
            if (!(_rData.diffuseTexture << _JsonValue[GLTFTEXT("diffuseTexture")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("specularFactor")) && _JsonValue[GLTFTEXT("specularFactor")].IsArray())
        {
            if (!(_rData.specularFactor << _JsonValue[GLTFTEXT("specularFactor")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("glossinessFactor")) && _JsonValue[GLTFTEXT("glossinessFactor")].IsFloat())
        {
            _rData.glossinessFactor = _JsonValue[GLTFTEXT("glossinessFactor")].GetFloat();
        }
        if (_JsonValue.HasMember(GLTFTEXT("specularGlossinessTexture")) && _JsonValue[GLTFTEXT("specularGlossinessTexture")].IsObject())
        {
            if (!(_rData.specularGlossinessTexture << _JsonValue[GLTFTEXT("specularGlossinessTexture")])) return false;
        }
        _rData.schemaType = GLTFTEXT("glTF.KHR_materials_pbrSpecularGlossiness.schema.json");
        return true;
    }

    bool operator>>(const TDataDoc<SKHR_materials_pbrSpecularGlossinessglTFextension>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        if (!_rData.data.diffuseFactor.empty())
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<std::vector<float>>(_rData.data.diffuseFactor, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("diffuseFactor"), json_value, _rData.doc->GetAllocator());
        }
        if (!!_rData.data.diffuseTexture)
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<STextureInfo>(*_rData.data.diffuseTexture, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("diffuseTexture"), json_value, _rData.doc->GetAllocator());
        }
        if (!_rData.data.specularFactor.empty())
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<std::vector<float>>(_rData.data.specularFactor, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("specularFactor"), json_value, _rData.doc->GetAllocator());
        }
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<float>(_rData.data.glossinessFactor, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("glossinessFactor"), json_value, _rData.doc->GetAllocator());
        }
        if (!!_rData.data.specularGlossinessTexture)
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<STextureInfo>(*_rData.data.specularGlossinessTexture, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("specularGlossinessTexture"), json_value, _rData.doc->GetAllocator());
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

    bool operator>>(const TDataDoc<std::shared_ptr<SKHR_materials_pbrSpecularGlossinessglTFextension>>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SKHR_materials_pbrSpecularGlossinessglTFextension>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SKHR_materials_pbrSpecularGlossinessglTFextension>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SKHR_materials_pbrSpecularGlossinessglTFextension>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SKHR_materials_pbrSpecularGlossinessglTFextension>>>& _rData, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SKHR_materials_pbrSpecularGlossinessglTFextension>>(_rData, _JsonValue);
    }

    bool operator<<(SKHR_materials_unlitglTFextension& _rData, const GLTFCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        _rData.schemaType = GLTFTEXT("glTF.KHR_materials_unlit.schema.json");
        return true;
    }

    bool operator>>(const TDataDoc<SKHR_materials_unlitglTFextension>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SKHR_materials_unlitglTFextension>& _pData, const GLTFCharValue& _JsonValue)
    {
        std::shared_ptr<SKHR_materials_unlitglTFextension> data_ptr = !!_pData ? _pData : std::make_shared<SKHR_materials_unlitglTFextension>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SKHR_materials_unlitglTFextension>>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SKHR_materials_unlitglTFextension>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SKHR_materials_unlitglTFextension>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SKHR_materials_unlitglTFextension>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SKHR_materials_unlitglTFextension>>>& _rData, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SKHR_materials_unlitglTFextension>>(_rData, _JsonValue);
    }

    bool operator<<(SKHR_techniques_webglglTFextension& _rData, const GLTFCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("programs")) && _JsonValue[GLTFTEXT("programs")].IsArray())
        {
            if (!(_rData.programs << _JsonValue[GLTFTEXT("programs")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("shaders")) && _JsonValue[GLTFTEXT("shaders")].IsArray())
        {
            if (!(_rData.shaders << _JsonValue[GLTFTEXT("shaders")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("techniques")) && _JsonValue[GLTFTEXT("techniques")].IsArray())
        {
            if (!(_rData.techniques << _JsonValue[GLTFTEXT("techniques")])) return false;
        }
        _rData.schemaType = GLTFTEXT("glTF.KHR_techniques_webgl.schema.json");
        return true;
    }

    bool operator>>(const TDataDoc<SKHR_techniques_webglglTFextension>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        if (!_rData.data.programs.empty())
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<std::vector<std::shared_ptr<SProgram>>>(_rData.data.programs, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("programs"), json_value, _rData.doc->GetAllocator());
        }
        if (!_rData.data.shaders.empty())
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<std::vector<std::shared_ptr<SShader>>>(_rData.data.shaders, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("shaders"), json_value, _rData.doc->GetAllocator());
        }
        if (!_rData.data.techniques.empty())
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<std::vector<std::shared_ptr<STechnique>>>(_rData.data.techniques, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("techniques"), json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SKHR_techniques_webglglTFextension>& _pData, const GLTFCharValue& _JsonValue)
    {
        std::shared_ptr<SKHR_techniques_webglglTFextension> data_ptr = !!_pData ? _pData : std::make_shared<SKHR_techniques_webglglTFextension>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SKHR_techniques_webglglTFextension>>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SKHR_techniques_webglglTFextension>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SKHR_techniques_webglglTFextension>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SKHR_techniques_webglglTFextension>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SKHR_techniques_webglglTFextension>>>& _rData, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SKHR_techniques_webglglTFextension>>(_rData, _JsonValue);
    }

    bool operator<<(SKHR_techniques_webglmaterialextension& _rData, const GLTFCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("technique")) && _JsonValue[GLTFTEXT("technique")].IsInt())
        {
            if (!(_rData.technique << _JsonValue[GLTFTEXT("technique")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("values")) && _JsonValue[GLTFTEXT("values")].IsObject())
        {
            if (!(_rData.values << _JsonValue[GLTFTEXT("values")])) return false;
        }
        _rData.schemaType = GLTFTEXT("material.KHR_techniques_webgl.schema.json");
        return true;
    }

    bool operator>>(const TDataDoc<SKHR_techniques_webglmaterialextension>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        if (!!_rData.data.technique)
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<SGlTFId>(*_rData.data.technique, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("technique"), json_value, _rData.doc->GetAllocator());
        }
        if (!_rData.data.values.empty())
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<std::map<string_t, std::shared_ptr<SUniformValue>>>(_rData.data.values, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("values"), json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SKHR_techniques_webglmaterialextension>& _pData, const GLTFCharValue& _JsonValue)
    {
        std::shared_ptr<SKHR_techniques_webglmaterialextension> data_ptr = !!_pData ? _pData : std::make_shared<SKHR_techniques_webglmaterialextension>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SKHR_techniques_webglmaterialextension>>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SKHR_techniques_webglmaterialextension>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SKHR_techniques_webglmaterialextension>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SKHR_techniques_webglmaterialextension>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SKHR_techniques_webglmaterialextension>>>& _rData, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SKHR_techniques_webglmaterialextension>>(_rData, _JsonValue);
    }

    bool operator<<(SProgram& _rData, const GLTFCharValue& _JsonValue)
    {
        {
            SGlTFChildofRootProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("fragmentShader")) && _JsonValue[GLTFTEXT("fragmentShader")].IsInt())
        {
            if (!(_rData.fragmentShader << _JsonValue[GLTFTEXT("fragmentShader")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("vertexShader")) && _JsonValue[GLTFTEXT("vertexShader")].IsInt())
        {
            if (!(_rData.vertexShader << _JsonValue[GLTFTEXT("vertexShader")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("glExtensions")) && _JsonValue[GLTFTEXT("glExtensions")].IsArray())
        {
            if (!(_rData.glExtensions << _JsonValue[GLTFTEXT("glExtensions")])) return false;
        }
        _rData.schemaType = GLTFTEXT("program.schema.json");
        return true;
    }

    bool operator>>(const TDataDoc<SProgram>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFChildofRootProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFChildofRootProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        if (!!_rData.data.fragmentShader)
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<SGlTFId>(*_rData.data.fragmentShader, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("fragmentShader"), json_value, _rData.doc->GetAllocator());
        }
        if (!!_rData.data.vertexShader)
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<SGlTFId>(*_rData.data.vertexShader, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("vertexShader"), json_value, _rData.doc->GetAllocator());
        }
        if (!_rData.data.glExtensions.empty())
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<std::vector<string_t>>(_rData.data.glExtensions, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("glExtensions"), json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SProgram>& _pData, const GLTFCharValue& _JsonValue)
    {
        std::shared_ptr<SProgram> data_ptr = !!_pData ? _pData : std::make_shared<SProgram>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SProgram>>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SProgram>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SProgram>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SProgram>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SProgram>>>& _rData, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SProgram>>(_rData, _JsonValue);
    }

    bool operator<<(SShader& _rData, const GLTFCharValue& _JsonValue)
    {
        {
            SGlTFChildofRootProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("uri")) && _JsonValue[GLTFTEXT("uri")].IsString())
        {
            _rData.uri = _JsonValue[GLTFTEXT("uri")].GetString();
        }
        if (_JsonValue.HasMember(GLTFTEXT("type")) && _JsonValue[GLTFTEXT("type")].IsInt())
        {
            _rData.type = _JsonValue[GLTFTEXT("type")].GetInt();
        }
        if (_JsonValue.HasMember(GLTFTEXT("bufferView")) && _JsonValue[GLTFTEXT("bufferView")].IsInt())
        {
            if (!(_rData.bufferView << _JsonValue[GLTFTEXT("bufferView")])) return false;
        }
        _rData.schemaType = GLTFTEXT("shader.schema.json");
        return true;
    }

    bool operator>>(const TDataDoc<SShader>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFChildofRootProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFChildofRootProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<string_t>(_rData.data.uri, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("uri"), json_value, _rData.doc->GetAllocator());
        }
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<int32_t>(_rData.data.type, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("type"), json_value, _rData.doc->GetAllocator());
        }
        if (!!_rData.data.bufferView)
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<SGlTFId>(*_rData.data.bufferView, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("bufferView"), json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SShader>& _pData, const GLTFCharValue& _JsonValue)
    {
        std::shared_ptr<SShader> data_ptr = !!_pData ? _pData : std::make_shared<SShader>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SShader>>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SShader>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SShader>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SShader>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SShader>>>& _rData, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SShader>>(_rData, _JsonValue);
    }

    bool operator<<(SAttribute& _rData, const GLTFCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("semantic")) && _JsonValue[GLTFTEXT("semantic")].IsString())
        {
            _rData.semantic = _JsonValue[GLTFTEXT("semantic")].GetString();
        }
        _rData.schemaType = GLTFTEXT("technique.attribute.schema.json");
        return true;
    }

    bool operator>>(const TDataDoc<SAttribute>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<string_t>(_rData.data.semantic, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("semantic"), json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SAttribute>& _pData, const GLTFCharValue& _JsonValue)
    {
        std::shared_ptr<SAttribute> data_ptr = !!_pData ? _pData : std::make_shared<SAttribute>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SAttribute>>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SAttribute>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SAttribute>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SAttribute>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SAttribute>>>& _rData, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SAttribute>>(_rData, _JsonValue);
    }

    bool operator<<(STechnique& _rData, const GLTFCharValue& _JsonValue)
    {
        {
            SGlTFChildofRootProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("program")) && _JsonValue[GLTFTEXT("program")].IsInt())
        {
            if (!(_rData.program << _JsonValue[GLTFTEXT("program")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("attributes")) && _JsonValue[GLTFTEXT("attributes")].IsObject())
        {
            if (!(_rData.attributes << _JsonValue[GLTFTEXT("attributes")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("uniforms")) && _JsonValue[GLTFTEXT("uniforms")].IsObject())
        {
            if (!(_rData.uniforms << _JsonValue[GLTFTEXT("uniforms")])) return false;
        }
        _rData.schemaType = GLTFTEXT("technique.schema.json");
        return true;
    }

    bool operator>>(const TDataDoc<STechnique>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFChildofRootProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFChildofRootProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        if (!!_rData.data.program)
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<SGlTFId>(*_rData.data.program, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("program"), json_value, _rData.doc->GetAllocator());
        }
        if (!_rData.data.attributes.empty())
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<std::map<string_t, std::shared_ptr<SAttribute>>>(_rData.data.attributes, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("attributes"), json_value, _rData.doc->GetAllocator());
        }
        if (!_rData.data.uniforms.empty())
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<std::map<string_t, std::shared_ptr<SUniform>>>(_rData.data.uniforms, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("uniforms"), json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<STechnique>& _pData, const GLTFCharValue& _JsonValue)
    {
        std::shared_ptr<STechnique> data_ptr = !!_pData ? _pData : std::make_shared<STechnique>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<STechnique>>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<STechnique>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<STechnique>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<STechnique>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<STechnique>>>& _rData, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<STechnique>>(_rData, _JsonValue);
    }

    bool operator<<(SUniform& _rData, const GLTFCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("count")) && _JsonValue[GLTFTEXT("count")].IsInt())
        {
            _rData.count = _JsonValue[GLTFTEXT("count")].GetInt();
        }
        if (_JsonValue.HasMember(GLTFTEXT("node")) && _JsonValue[GLTFTEXT("node")].IsInt())
        {
            if (!(_rData.node << _JsonValue[GLTFTEXT("node")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("type")) && _JsonValue[GLTFTEXT("type")].IsInt())
        {
            _rData.type = _JsonValue[GLTFTEXT("type")].GetInt();
        }
        if (_JsonValue.HasMember(GLTFTEXT("semantic")) && _JsonValue[GLTFTEXT("semantic")].IsString())
        {
            _rData.semantic = _JsonValue[GLTFTEXT("semantic")].GetString();
        }
        if (_JsonValue.HasMember(GLTFTEXT("value")) && _JsonValue[GLTFTEXT("value")].IsObject())
        {
            if (!(_rData.value << _JsonValue[GLTFTEXT("value")])) return false;
        }
        _rData.schemaType = GLTFTEXT("technique.uniform.schema.json");
        return true;
    }

    bool operator>>(const TDataDoc<SUniform>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<int32_t>(_rData.data.count, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("count"), json_value, _rData.doc->GetAllocator());
        }
        if (!!_rData.data.node)
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<SGlTFId>(*_rData.data.node, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("node"), json_value, _rData.doc->GetAllocator());
        }
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<int32_t>(_rData.data.type, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("type"), json_value, _rData.doc->GetAllocator());
        }
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<string_t>(_rData.data.semantic, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("semantic"), json_value, _rData.doc->GetAllocator());
        }
        if (!!_rData.data.value)
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<SUniformValue>(*_rData.data.value, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("value"), json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SUniform>& _pData, const GLTFCharValue& _JsonValue)
    {
        std::shared_ptr<SUniform> data_ptr = !!_pData ? _pData : std::make_shared<SUniform>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SUniform>>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SUniform>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SUniform>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SUniform>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SUniform>>>& _rData, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SUniform>>(_rData, _JsonValue);
    }

    bool operator<<(SUniformValue& _rData, const GLTFCharValue& _JsonValue)
    {
        _rData.schemaType = GLTFTEXT("uniform.value.schema.json");
        return true;
    }

    bool operator>>(const TDataDoc<SUniformValue>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        return true;
    }

    bool operator<<(std::shared_ptr<SUniformValue>& _pData, const GLTFCharValue& _JsonValue)
    {
        std::shared_ptr<SUniformValue> data_ptr = !!_pData ? _pData : std::make_shared<SUniformValue>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SUniformValue>>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SUniformValue>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SUniformValue>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SUniformValue>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SUniformValue>>>& _rData, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SUniformValue>>(_rData, _JsonValue);
    }

    bool operator<<(SKHR_texture_transformtextureInfoextension& _rData, const GLTFCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("offset")) && _JsonValue[GLTFTEXT("offset")].IsArray())
        {
            if (!(_rData.offset << _JsonValue[GLTFTEXT("offset")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("rotation")) && _JsonValue[GLTFTEXT("rotation")].IsFloat())
        {
            _rData.rotation = _JsonValue[GLTFTEXT("rotation")].GetFloat();
        }
        if (_JsonValue.HasMember(GLTFTEXT("scale")) && _JsonValue[GLTFTEXT("scale")].IsArray())
        {
            if (!(_rData.scale << _JsonValue[GLTFTEXT("scale")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("texCoord")) && _JsonValue[GLTFTEXT("texCoord")].IsInt())
        {
            _rData.texCoord = _JsonValue[GLTFTEXT("texCoord")].GetInt();
        }
        _rData.schemaType = GLTFTEXT("KHR_texture_transform.textureInfo.schema.json");
        return true;
    }

    bool operator>>(const TDataDoc<SKHR_texture_transformtextureInfoextension>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        if (!_rData.data.offset.empty())
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<std::vector<float>>(_rData.data.offset, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("offset"), json_value, _rData.doc->GetAllocator());
        }
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<float>(_rData.data.rotation, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("rotation"), json_value, _rData.doc->GetAllocator());
        }
        if (!_rData.data.scale.empty())
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<std::vector<float>>(_rData.data.scale, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("scale"), json_value, _rData.doc->GetAllocator());
        }
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<int32_t>(_rData.data.texCoord, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("texCoord"), json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SKHR_texture_transformtextureInfoextension>& _pData, const GLTFCharValue& _JsonValue)
    {
        std::shared_ptr<SKHR_texture_transformtextureInfoextension> data_ptr = !!_pData ? _pData : std::make_shared<SKHR_texture_transformtextureInfoextension>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SKHR_texture_transformtextureInfoextension>>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SKHR_texture_transformtextureInfoextension>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SKHR_texture_transformtextureInfoextension>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SKHR_texture_transformtextureInfoextension>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SKHR_texture_transformtextureInfoextension>>>& _rData, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SKHR_texture_transformtextureInfoextension>>(_rData, _JsonValue);
    }

    bool operator<<(SADOBE_materials_thin_transparencyglTFextension& _rData, const GLTFCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("transmissionFactor")) && _JsonValue[GLTFTEXT("transmissionFactor")].IsFloat())
        {
            _rData.transmissionFactor = _JsonValue[GLTFTEXT("transmissionFactor")].GetFloat();
        }
        if (_JsonValue.HasMember(GLTFTEXT("transmissionTexture")) && _JsonValue[GLTFTEXT("transmissionTexture")].IsObject())
        {
            if (!(_rData.transmissionTexture << _JsonValue[GLTFTEXT("transmissionTexture")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("ior")) && _JsonValue[GLTFTEXT("ior")].IsFloat())
        {
            _rData.ior = _JsonValue[GLTFTEXT("ior")].GetFloat();
        }
        _rData.schemaType = GLTFTEXT("glTF.ADOBE_materials_thin_transparency.schema.json");
        return true;
    }

    bool operator>>(const TDataDoc<SADOBE_materials_thin_transparencyglTFextension>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<float>(_rData.data.transmissionFactor, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("transmissionFactor"), json_value, _rData.doc->GetAllocator());
        }
        if (!!_rData.data.transmissionTexture)
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<STextureInfo>(*_rData.data.transmissionTexture, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("transmissionTexture"), json_value, _rData.doc->GetAllocator());
        }
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<float>(_rData.data.ior, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("ior"), json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SADOBE_materials_thin_transparencyglTFextension>& _pData, const GLTFCharValue& _JsonValue)
    {
        std::shared_ptr<SADOBE_materials_thin_transparencyglTFextension> data_ptr = !!_pData ? _pData : std::make_shared<SADOBE_materials_thin_transparencyglTFextension>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SADOBE_materials_thin_transparencyglTFextension>>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SADOBE_materials_thin_transparencyglTFextension>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SADOBE_materials_thin_transparencyglTFextension>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SADOBE_materials_thin_transparencyglTFextension>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SADOBE_materials_thin_transparencyglTFextension>>>& _rData, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SADOBE_materials_thin_transparencyglTFextension>>(_rData, _JsonValue);
    }

    bool operator<<(SArticulation& _rData, const GLTFCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("name")) && _JsonValue[GLTFTEXT("name")].IsString())
        {
            _rData.name = _JsonValue[GLTFTEXT("name")].GetString();
        }
        if (_JsonValue.HasMember(GLTFTEXT("stages")) && _JsonValue[GLTFTEXT("stages")].IsArray())
        {
            if (!(_rData.stages << _JsonValue[GLTFTEXT("stages")])) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("pointingVector")) && _JsonValue[GLTFTEXT("pointingVector")].IsArray())
        {
            if (!(_rData.pointingVector << _JsonValue[GLTFTEXT("pointingVector")])) return false;
        }
        _rData.schemaType = GLTFTEXT("articulation.schema.json");
        return true;
    }

    bool operator>>(const TDataDoc<SArticulation>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<string_t>(_rData.data.name, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("name"), json_value, _rData.doc->GetAllocator());
        }
        if (!_rData.data.stages.empty())
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<std::vector<std::shared_ptr<SArticulationStage>>>(_rData.data.stages, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("stages"), json_value, _rData.doc->GetAllocator());
        }
        if (!_rData.data.pointingVector.empty())
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<std::vector<float>>(_rData.data.pointingVector, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("pointingVector"), json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SArticulation>& _pData, const GLTFCharValue& _JsonValue)
    {
        std::shared_ptr<SArticulation> data_ptr = !!_pData ? _pData : std::make_shared<SArticulation>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SArticulation>>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SArticulation>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SArticulation>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SArticulation>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SArticulation>>>& _rData, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SArticulation>>(_rData, _JsonValue);
    }

    bool operator<<(SArticulationStage& _rData, const GLTFCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("name")) && _JsonValue[GLTFTEXT("name")].IsString())
        {
            _rData.name = _JsonValue[GLTFTEXT("name")].GetString();
        }
        if (_JsonValue.HasMember(GLTFTEXT("type")) && _JsonValue[GLTFTEXT("type")].IsString())
        {
            _rData.type = _JsonValue[GLTFTEXT("type")].GetString();
        }
        if (_JsonValue.HasMember(GLTFTEXT("minimumValue")) && _JsonValue[GLTFTEXT("minimumValue")].IsFloat())
        {
            _rData.minimumValue = _JsonValue[GLTFTEXT("minimumValue")].GetFloat();
        }
        if (_JsonValue.HasMember(GLTFTEXT("maximumValue")) && _JsonValue[GLTFTEXT("maximumValue")].IsFloat())
        {
            _rData.maximumValue = _JsonValue[GLTFTEXT("maximumValue")].GetFloat();
        }
        if (_JsonValue.HasMember(GLTFTEXT("initialValue")) && _JsonValue[GLTFTEXT("initialValue")].IsFloat())
        {
            _rData.initialValue = _JsonValue[GLTFTEXT("initialValue")].GetFloat();
        }
        _rData.schemaType = GLTFTEXT("articulation.stage.schema.json");
        return true;
    }

    bool operator>>(const TDataDoc<SArticulationStage>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<string_t>(_rData.data.name, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("name"), json_value, _rData.doc->GetAllocator());
        }
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<string_t>(_rData.data.type, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("type"), json_value, _rData.doc->GetAllocator());
        }
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<float>(_rData.data.minimumValue, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("minimumValue"), json_value, _rData.doc->GetAllocator());
        }
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<float>(_rData.data.maximumValue, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("maximumValue"), json_value, _rData.doc->GetAllocator());
        }
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<float>(_rData.data.initialValue, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("initialValue"), json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SArticulationStage>& _pData, const GLTFCharValue& _JsonValue)
    {
        std::shared_ptr<SArticulationStage> data_ptr = !!_pData ? _pData : std::make_shared<SArticulationStage>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SArticulationStage>>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SArticulationStage>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SArticulationStage>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SArticulationStage>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SArticulationStage>>>& _rData, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SArticulationStage>>(_rData, _JsonValue);
    }

    bool operator<<(SAGI_articulationsglTFextension& _rData, const GLTFCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("articulations")) && _JsonValue[GLTFTEXT("articulations")].IsArray())
        {
            if (!(_rData.articulations << _JsonValue[GLTFTEXT("articulations")])) return false;
        }
        _rData.schemaType = GLTFTEXT("gltf.AGI_articulations.schema.json");
        return true;
    }

    bool operator>>(const TDataDoc<SAGI_articulationsglTFextension>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        if (!_rData.data.articulations.empty())
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<std::vector<std::shared_ptr<SArticulation>>>(_rData.data.articulations, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("articulations"), json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SAGI_articulationsglTFextension>& _pData, const GLTFCharValue& _JsonValue)
    {
        std::shared_ptr<SAGI_articulationsglTFextension> data_ptr = !!_pData ? _pData : std::make_shared<SAGI_articulationsglTFextension>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SAGI_articulationsglTFextension>>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SAGI_articulationsglTFextension>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SAGI_articulationsglTFextension>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SAGI_articulationsglTFextension>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SAGI_articulationsglTFextension>>>& _rData, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SAGI_articulationsglTFextension>>(_rData, _JsonValue);
    }

    bool operator<<(SAGI_articulationsglTFNodeextension& _rData, const GLTFCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("isAttachPoint")) && _JsonValue[GLTFTEXT("isAttachPoint")].IsBool())
        {
            _rData.isAttachPoint = _JsonValue[GLTFTEXT("isAttachPoint")].GetBool();
        }
        if (_JsonValue.HasMember(GLTFTEXT("articulationName")) && _JsonValue[GLTFTEXT("articulationName")].IsString())
        {
            _rData.articulationName = _JsonValue[GLTFTEXT("articulationName")].GetString();
        }
        _rData.schemaType = GLTFTEXT("node.AGI_articulations.schema.json");
        return true;
    }

    bool operator>>(const TDataDoc<SAGI_articulationsglTFNodeextension>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<bool>(_rData.data.isAttachPoint, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("isAttachPoint"), json_value, _rData.doc->GetAllocator());
        }
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<string_t>(_rData.data.articulationName, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("articulationName"), json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SAGI_articulationsglTFNodeextension>& _pData, const GLTFCharValue& _JsonValue)
    {
        std::shared_ptr<SAGI_articulationsglTFNodeextension> data_ptr = !!_pData ? _pData : std::make_shared<SAGI_articulationsglTFNodeextension>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SAGI_articulationsglTFNodeextension>>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SAGI_articulationsglTFNodeextension>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SAGI_articulationsglTFNodeextension>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SAGI_articulationsglTFNodeextension>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SAGI_articulationsglTFNodeextension>>>& _rData, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SAGI_articulationsglTFNodeextension>>(_rData, _JsonValue);
    }

    bool operator<<(SAGI_stk_metadataglTFextension& _rData, const GLTFCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("solarPanelGroups")) && _JsonValue[GLTFTEXT("solarPanelGroups")].IsArray())
        {
            if (!(_rData.solarPanelGroups << _JsonValue[GLTFTEXT("solarPanelGroups")])) return false;
        }
        _rData.schemaType = GLTFTEXT("gltf.AGI_stk_metadata.schema.json");
        return true;
    }

    bool operator>>(const TDataDoc<SAGI_stk_metadataglTFextension>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        if (!_rData.data.solarPanelGroups.empty())
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<std::vector<std::shared_ptr<SSolarPanelGroup>>>(_rData.data.solarPanelGroups, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("solarPanelGroups"), json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SAGI_stk_metadataglTFextension>& _pData, const GLTFCharValue& _JsonValue)
    {
        std::shared_ptr<SAGI_stk_metadataglTFextension> data_ptr = !!_pData ? _pData : std::make_shared<SAGI_stk_metadataglTFextension>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SAGI_stk_metadataglTFextension>>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SAGI_stk_metadataglTFextension>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SAGI_stk_metadataglTFextension>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SAGI_stk_metadataglTFextension>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SAGI_stk_metadataglTFextension>>>& _rData, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SAGI_stk_metadataglTFextension>>(_rData, _JsonValue);
    }

    bool operator<<(SAGI_stk_metadataglTFNodeextension& _rData, const GLTFCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("solarPanelGroupName")) && _JsonValue[GLTFTEXT("solarPanelGroupName")].IsString())
        {
            _rData.solarPanelGroupName = _JsonValue[GLTFTEXT("solarPanelGroupName")].GetString();
        }
        if (_JsonValue.HasMember(GLTFTEXT("noObscuration")) && _JsonValue[GLTFTEXT("noObscuration")].IsBool())
        {
            _rData.noObscuration = _JsonValue[GLTFTEXT("noObscuration")].GetBool();
        }
        _rData.schemaType = GLTFTEXT("node.AGI_stk_metadata.schema.json");
        return true;
    }

    bool operator>>(const TDataDoc<SAGI_stk_metadataglTFNodeextension>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<string_t>(_rData.data.solarPanelGroupName, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("solarPanelGroupName"), json_value, _rData.doc->GetAllocator());
        }
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<bool>(_rData.data.noObscuration, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("noObscuration"), json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SAGI_stk_metadataglTFNodeextension>& _pData, const GLTFCharValue& _JsonValue)
    {
        std::shared_ptr<SAGI_stk_metadataglTFNodeextension> data_ptr = !!_pData ? _pData : std::make_shared<SAGI_stk_metadataglTFNodeextension>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SAGI_stk_metadataglTFNodeextension>>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SAGI_stk_metadataglTFNodeextension>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SAGI_stk_metadataglTFNodeextension>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SAGI_stk_metadataglTFNodeextension>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SAGI_stk_metadataglTFNodeextension>>>& _rData, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SAGI_stk_metadataglTFNodeextension>>(_rData, _JsonValue);
    }

    bool operator<<(SSolarPanelGroup& _rData, const GLTFCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("name")) && _JsonValue[GLTFTEXT("name")].IsString())
        {
            _rData.name = _JsonValue[GLTFTEXT("name")].GetString();
        }
        if (_JsonValue.HasMember(GLTFTEXT("efficiency")) && _JsonValue[GLTFTEXT("efficiency")].IsFloat())
        {
            _rData.efficiency = _JsonValue[GLTFTEXT("efficiency")].GetFloat();
        }
        _rData.schemaType = GLTFTEXT("solarPanelGroup.schema.json");
        return true;
    }

    bool operator>>(const TDataDoc<SSolarPanelGroup>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<string_t>(_rData.data.name, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("name"), json_value, _rData.doc->GetAllocator());
        }
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<float>(_rData.data.efficiency, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("efficiency"), json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SSolarPanelGroup>& _pData, const GLTFCharValue& _JsonValue)
    {
        std::shared_ptr<SSolarPanelGroup> data_ptr = !!_pData ? _pData : std::make_shared<SSolarPanelGroup>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SSolarPanelGroup>>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SSolarPanelGroup>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SSolarPanelGroup>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SSolarPanelGroup>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SSolarPanelGroup>>>& _rData, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SSolarPanelGroup>>(_rData, _JsonValue);
    }

    bool operator<<(SCESIUM_primitive_outlineglTFprimitiveextension& _rData, const GLTFCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("indices")) && _JsonValue[GLTFTEXT("indices")].IsInt())
        {
            _rData.indices = _JsonValue[GLTFTEXT("indices")].GetInt();
        }
        _rData.schemaType = GLTFTEXT("primitive.CESIUM_primitive_outline.schema.json");
        return true;
    }

    bool operator>>(const TDataDoc<SCESIUM_primitive_outlineglTFprimitiveextension>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<int32_t>(_rData.data.indices, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("indices"), json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SCESIUM_primitive_outlineglTFprimitiveextension>& _pData, const GLTFCharValue& _JsonValue)
    {
        std::shared_ptr<SCESIUM_primitive_outlineglTFprimitiveextension> data_ptr = !!_pData ? _pData : std::make_shared<SCESIUM_primitive_outlineglTFprimitiveextension>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SCESIUM_primitive_outlineglTFprimitiveextension>>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SCESIUM_primitive_outlineglTFprimitiveextension>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SCESIUM_primitive_outlineglTFprimitiveextension>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SCESIUM_primitive_outlineglTFprimitiveextension>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SCESIUM_primitive_outlineglTFprimitiveextension>>>& _rData, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SCESIUM_primitive_outlineglTFprimitiveextension>>(_rData, _JsonValue);
    }

    bool operator<<(SEXT_mesh_gpu_instancingglTFextension& _rData, const GLTFCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("attributes")) && _JsonValue[GLTFTEXT("attributes")].IsObject())
        {
            if (!(_rData.attributes << _JsonValue[GLTFTEXT("attributes")])) return false;
        }
        _rData.schemaType = GLTFTEXT("glTF.EXT_mesh_gpu_instancing.schema.json");
        return true;
    }

    bool operator>>(const TDataDoc<SEXT_mesh_gpu_instancingglTFextension>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        if (!_rData.data.attributes.empty())
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<std::map<string_t, std::shared_ptr<SGlTFId>>>(_rData.data.attributes, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("attributes"), json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SEXT_mesh_gpu_instancingglTFextension>& _pData, const GLTFCharValue& _JsonValue)
    {
        std::shared_ptr<SEXT_mesh_gpu_instancingglTFextension> data_ptr = !!_pData ? _pData : std::make_shared<SEXT_mesh_gpu_instancingglTFextension>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SEXT_mesh_gpu_instancingglTFextension>>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SEXT_mesh_gpu_instancingglTFextension>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SEXT_mesh_gpu_instancingglTFextension>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SEXT_mesh_gpu_instancingglTFextension>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SEXT_mesh_gpu_instancingglTFextension>>>& _rData, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SEXT_mesh_gpu_instancingglTFextension>>(_rData, _JsonValue);
    }

    bool operator<<(SEXT_texture_webpglTFextension& _rData, const GLTFCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("source")) && _JsonValue[GLTFTEXT("source")].IsInt())
        {
            if (!(_rData.source << _JsonValue[GLTFTEXT("source")])) return false;
        }
        _rData.schemaType = GLTFTEXT("glTF.EXT_texture_webp.schema.json");
        return true;
    }

    bool operator>>(const TDataDoc<SEXT_texture_webpglTFextension>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        if (!!_rData.data.source)
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<SGlTFId>(*_rData.data.source, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("source"), json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SEXT_texture_webpglTFextension>& _pData, const GLTFCharValue& _JsonValue)
    {
        std::shared_ptr<SEXT_texture_webpglTFextension> data_ptr = !!_pData ? _pData : std::make_shared<SEXT_texture_webpglTFextension>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SEXT_texture_webpglTFextension>>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SEXT_texture_webpglTFextension>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SEXT_texture_webpglTFextension>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SEXT_texture_webpglTFextension>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SEXT_texture_webpglTFextension>>>& _rData, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SEXT_texture_webpglTFextension>>(_rData, _JsonValue);
    }

    bool operator<<(SMSFT_lodglTFextension& _rData, const GLTFCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("ids")) && _JsonValue[GLTFTEXT("ids")].IsArray())
        {
            if (!(_rData.ids << _JsonValue[GLTFTEXT("ids")])) return false;
        }
        _rData.schemaType = GLTFTEXT("glTF.MSFT_lod.schema.json");
        return true;
    }

    bool operator>>(const TDataDoc<SMSFT_lodglTFextension>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        if (!_rData.data.ids.empty())
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<std::vector<int32_t>>(_rData.data.ids, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("ids"), json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SMSFT_lodglTFextension>& _pData, const GLTFCharValue& _JsonValue)
    {
        std::shared_ptr<SMSFT_lodglTFextension> data_ptr = !!_pData ? _pData : std::make_shared<SMSFT_lodglTFextension>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SMSFT_lodglTFextension>>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SMSFT_lodglTFextension>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SMSFT_lodglTFextension>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SMSFT_lodglTFextension>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SMSFT_lodglTFextension>>>& _rData, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SMSFT_lodglTFextension>>(_rData, _JsonValue);
    }

    bool operator<<(SMSFT_texture_ddsextension& _rData, const GLTFCharValue& _JsonValue)
    {
        {
            SGlTFProperty& super_ptr = _rData;
            if (!(super_ptr << _JsonValue)) return false;
        }
        if (_JsonValue.HasMember(GLTFTEXT("source")) && _JsonValue[GLTFTEXT("source")].IsInt())
        {
            if (!(_rData.source << _JsonValue[GLTFTEXT("source")])) return false;
        }
        _rData.schemaType = GLTFTEXT("glTF.MSFT_texture_dds.schema.json");
        return true;
    }

    bool operator>>(const TDataDoc<SMSFT_texture_ddsextension>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.doc) return false;
        _JsonValue.SetObject();
        {
            const SGlTFProperty& super_ptr = _rData.data;
            if (!(TDataDoc<SGlTFProperty>(super_ptr, _rData.doc) >> _JsonValue)) return false;
        }
        if (!!_rData.data.source)
        {
            GLTFCharValue json_value;
            if (!(TDataDoc<SGlTFId>(*_rData.data.source, _rData.doc) >> json_value)) return false;
            _JsonValue.AddMember(GLTFTEXT("source"), json_value, _rData.doc->GetAllocator());
        }
        return true;
    }

    bool operator<<(std::shared_ptr<SMSFT_texture_ddsextension>& _pData, const GLTFCharValue& _JsonValue)
    {
        std::shared_ptr<SMSFT_texture_ddsextension> data_ptr = !!_pData ? _pData : std::make_shared<SMSFT_texture_ddsextension>();
        if (!(*data_ptr << _JsonValue)) return false;
        _pData = data_ptr;
        return true;
    }

    bool operator>>(const TDataDoc<std::shared_ptr<SMSFT_texture_ddsextension>>& _rData, GLTFCharValue& _JsonValue)
    {
        if (!_rData.data) return false;
        return (TDataDoc<SMSFT_texture_ddsextension>(*_rData.data, _rData.doc) >> _JsonValue);
    }

    bool operator<<(std::vector<std::shared_ptr<SMSFT_texture_ddsextension>>& _vDatas, const GLTFCharValue& _JsonValue)
    {
        return operator<< <std::shared_ptr<SMSFT_texture_ddsextension>>(_vDatas, _JsonValue);
    }

    bool operator>>(const TDataDoc<std::vector<std::shared_ptr<SMSFT_texture_ddsextension>>>& _rData, GLTFCharValue& _JsonValue)
    {
        return operator>> <std::shared_ptr<SMSFT_texture_ddsextension>>(_rData, _JsonValue);
    }

}

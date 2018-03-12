import sys
import os
import argparse
from ConfigParser import SafeConfigParser
import logging
import json

from c11types import BuildC11Type
from c11types.c11typestruct import C11TypeStruct

logging.basicConfig(level=logging.DEBUG, format=u'%(asctime)s [%(name)s][%(levelname)s](%(filename)s:%(lineno)d) %(message)s')
logger = logging.getLogger(name=u'JSONSchemaToC11')

class C11TypeLibrary(object):
    def __init__(self):
        self.c11Types = dict()

    def setVersion(self, major=0, minor=0, patch=0):
        self.major = major
        self.minor = minor
        self.patch = patch

    def addC11Type(self, schemaName, c11Type):
        if schemaName in self.c11Types:
            return (1, u'already have %s in c11 type library' % schemaName)
        self.c11Types[schemaName] = c11Type
        return (0, u'')

    def addSchema(self, rootPath, schemaFileName, config=None):
        schema_file_path = os.path.join(rootPath, schemaFileName)
        with open(schema_file_path, u'r') as schema_file:
            schema = json.load(schema_file)
            if schema is None:
                return (1, u'Can\'t parse the schema file %s' % schema_file_path)
            code_headers = None
            if config is not None and config.has_option(u'code.headers', schemaFileName):
                code_file_path = config.get(u'code.headers', schemaFileName)
                if os.path.isfile(code_file_path):
                    with open(code_file_path, u'r') as code_file:
                        code_headers = code_file.readlines()
            code_sources_variable = None
            if config is not None and config.has_option(u'code.sources.variable', schemaFileName):
                code_file_path = config.get(u'code.sources.variable', schemaFileName)
                if os.path.isfile(code_file_path):
                    with open(code_file_path, u'r') as code_file:
                        code_sources_variable = code_file.readlines()
            code_sources_function = None
            if config is not None and config.has_option(u'code.sources.function', schemaFileName):
                code_file_path = config.get(u'code.sources.function', schemaFileName)
                if os.path.isfile(code_file_path):
                    with open(code_file_path, u'r') as code_file:
                        code_sources_function = code_file.readlines()
            code_parsers_from = None
            if config is not None and config.has_option(u'code.parsers.from', schemaFileName):
                code_file_path = config.get(u'code.parsers.from', schemaFileName)
                if os.path.isfile(code_file_path):
                    with open(code_file_path, u'r') as code_file:
                        code_parsers_from = code_file.readlines()
            code_parsers_to = None
            if config is not None and config.has_option(u'code.parsers.to', schemaFileName):
                code_file_path = config.get(u'code.parsers.to', schemaFileName)
                if os.path.isfile(code_file_path):
                    with open(code_file_path, u'r') as code_file:
                        code_parsers_to = code_file.readlines()
            (c11_type, error_code, error_message) = BuildC11Type(schemaFileName, schema, isSchema=True, manualCodeHeaders=code_headers, manualCodeSourcesVariable=code_sources_variable, manualCodeSourcesFunction=code_sources_function , manualCodeParsersFrom=code_parsers_from, manualCodeParsersTo=code_parsers_to)
            if error_code != 0:
                return (error_code, u'Has error when build - %s' % error_message)
            (error_code, error_message) = self.addC11Type(schemaFileName, c11_type)
            if error_code != 0:
                return (error_code, u'Has error when add - %s' % error_message)
        return (0, u'')

    def addSchemaDirectory(self, schemaDirectory, config=None):
        schema_directory = os.path.normpath(schemaDirectory)
        if not os.path.isdir(schema_directory):
            return (1, u'The schema directory (%s) is not a valid directory' % schema_directory)

        for root_path, _, file_names in os.walk(schema_directory, topdown=False):
            for file_name in file_names:
                if not file_name.endswith(u'schema.json'):
                    continue
                (error_code, error_message) = self.addSchema(root_path, file_name, config)
                if error_code != 0:
                    return (error_code, error_message)
        return (0, u'')

    def codeVersion(self):
        code_lines = []
        code_lines.append(u'#define LIBGLTF_MAJOR_VERSION    %s' % self.major)
        code_lines.append(u'#define LIBGLTF_MINOR_VERSION    %s' % self.minor)
        code_lines.append(u'#define LIBGLTF_PATCH_VERSION    %s' % self.patch)
        return code_lines

    @classmethod
    def codeHeaderParser(self):
        code_lines = []
        code_lines.append(u'struct SGlTF;')
        code_lines.append(u'bool operator<<(std::shared_ptr<SGlTF>& _pGlTF, const GLTFString& _sContent);')
        code_lines.append(u'bool operator>>(const std::shared_ptr<SGlTF>& _pGlTF, GLTFString& _sContent);')
        code_lines.append(u'struct SObject')
        code_lines.append(u'{')
        code_lines.append(u'    SObject();')
        code_lines.append(u'    GLTFString schemaType;')
        code_lines.append(u'};')
        return code_lines

    @classmethod
    def codeSourceParser(self):
        code_lines = []
        code_lines.append(u'SObject::SObject()')
        code_lines.append(u'    : schemaType(GLTFTEXT(""))')
        code_lines.append(u'{')
        code_lines.append(u'    //')
        code_lines.append(u'}')
        return code_lines

    def preprocess(self):
        for key in self.c11Types:
            (error_code, error_message) = self.c11Types[key].revise(self.c11Types)
            if error_code != 0:
                return (error_code, error_message)
        return (0, u'')

    def generate(self, codeFileName, outputHeaderPath=None, outputSourcePath=None, nameSpace=None):
        header_file_path = u'%s.h' % codeFileName
        source_file_path = u'%s.cpp' % codeFileName
        if outputHeaderPath is not None:
            header_file_path = os.path.join(outputHeaderPath, header_file_path)
        if outputSourcePath is not None:
            source_file_path = os.path.join(outputSourcePath, source_file_path)

        with open(header_file_path, u'w') as header_file:
            header_file.write(u'#pragma once\n')
            header_file.write(u'\n')
            header_file.write(u'#include <stdint.h>\n')
            header_file.write(u'#include <vector>\n')
            header_file.write(u'#include <map>\n')
            header_file.write(u'#include <string>\n')
            header_file.write(u'#include <memory>\n')
            header_file.write(u'\n')

            begin_space = u''

            code_version_parser_lines = self.codeVersion()
            for code_version_parser_line in code_version_parser_lines:
                code_version_parser_line = u'%s%s\n' % (begin_space, code_version_parser_line)
                header_file.write(code_version_parser_line)
            if code_version_parser_line:
                header_file.write(u'\n')

            header_file.write(u'#if defined(LIBGLTF_USE_WCHAR)\n')
            header_file.write(u'%s%s\n' % (begin_space, u'typedef std::wstring                                        GLTFString;'))
            header_file.write(u'#else\n')
            header_file.write(u'%s%s\n' % (begin_space, u'typedef std::string                                         GLTFString;'))
            header_file.write(u'#endif\n')
            header_file.write(u'\n')

            if nameSpace is not None:
                header_file.write(u'namespace %s\n' % nameSpace)
                header_file.write(u'{\n')
                begin_space = u'    '

            code_header_parser_lines = self.codeHeaderParser()
            for code_header_parser_line in code_header_parser_lines:
                code_header_parser_line = u'%s%s\n' % (begin_space, code_header_parser_line)
                header_file.write(code_header_parser_line)
            if code_header_parser_lines:
                header_file.write(u'\n')

            parent_type_names = []
            for key in self.c11Types:
                c11_type = self.c11Types[key]
                if c11_type.codeTypeName() in parent_type_names:
                    continue

                c11_type_code_lines = c11_type.codeHeader(parent_type_names)
                for c11_type_code_line in c11_type_code_lines:
                    if c11_type_code_line:
                        c11_type_code_line = u'%s%s' % (begin_space, c11_type_code_line)
                    header_file.write(u'%s\n' % c11_type_code_line)
                header_file.write(u'\n')

                parent_type_names.append(c11_type.codeTypeName())
                if isinstance(c11_type, C11TypeStruct):
                    for c11_type_parent_type_name in c11_type.getParentTypeNames():
                        parent_type_names.append(c11_type_parent_type_name)

            if nameSpace != None:
                header_file.write(u'}\n')

        with open(source_file_path, u'w') as source_file:
            source_file.write(u'#include "%spch.h"\n' % codeFileName)
            source_file.write(u'#include "%s.h"\n' % codeFileName)
            source_file.write(u'\n')

            begin_space = u''
            if nameSpace != None:
                source_file.write(u'namespace %s\n' % nameSpace)
                source_file.write(u'{\n')
                begin_space = u'    '

            code_source_parser_lines = self.codeSourceParser()
            for code_source_parser_line in code_source_parser_lines:
                code_source_parser_line = u'%s%s\n' % (begin_space, code_source_parser_line)
                source_file.write(code_source_parser_line)
            if code_source_parser_lines:
                source_file.write(u'\n')

            parent_type_names = []
            for key in self.c11Types:
                c11_type = self.c11Types[key]
                if c11_type.codeTypeName() in parent_type_names:
                    continue

                c11_type_code_lines = c11_type.codeSource(parent_type_names)
                for c11_type_code_line in c11_type_code_lines:
                    if c11_type_code_line:
                        c11_type_code_line = u'%s%s' % (begin_space, c11_type_code_line)
                    source_file.write(u'%s\n' % c11_type_code_line)
                source_file.write(u'\n')

                parent_type_names.append(c11_type.codeTypeName())
                if isinstance(c11_type, C11TypeStruct):
                    for c11_type_parent_type_name in c11_type.getParentTypeNames():
                        parent_type_names.append(c11_type_parent_type_name)

            if nameSpace != None:
                source_file.write(u'}\n')

        # generate parser
        header_file_path = u'%sparser.h' % codeFileName
        source_file_path = u'%sparser.cpp' % codeFileName
        if outputSourcePath != None:
            header_file_path = os.path.join(outputSourcePath, header_file_path)
            source_file_path = os.path.join(outputSourcePath, source_file_path)
        with open(header_file_path, u'w') as header_file:
            header_file.write(u'#pragma once\n')
            header_file.write(u'\n')
            header_file.write(u'#include "%spch.h"\n' % codeFileName)
            header_file.write(u'\n')
            header_file.write(u'#include <memory>\n')
            header_file.write(u'#include <vector>\n')
            header_file.write(u'\n')

            begin_space = u''
            if nameSpace != None:
                header_file.write(u'namespace %s\n' % nameSpace)
                header_file.write(u'{\n')
                begin_space = u'    '

            for key in self.c11Types:
                c11_type = self.c11Types[key]
                if not isinstance(c11_type, C11TypeStruct):
                    continue

                code_lines = c11_type.codeParserHeader()
                for code_line in code_lines:
                    if not code_line:
                        header_file.write(u'\n')
                    else:
                        header_file.write(u'%s%s\n' % (begin_space, code_line))
                header_file.write(u'\n')

            if nameSpace != None:
                header_file.write(u'}\n')

        with open(source_file_path, u'w') as source_file:
            source_file.write(u'#include "%spch.h"\n' % codeFileName)
            source_file.write(u'#include "%sparser.h"\n' % codeFileName)
            source_file.write(u'#include "%s.h"\n' % codeFileName)
            source_file.write(u'\n')

            begin_space = u''
            if nameSpace != None:
                source_file.write(u'namespace %s\n' % nameSpace)
                source_file.write(u'{\n')
                begin_space = u'    '

            source_file.write(u'%sclass GLTFCharDocumentPtrWrapper\n' % begin_space)
            source_file.write(u'%s{\n' % begin_space)
            source_file.write(u'%spublic:\n' % begin_space)
            source_file.write(u'%s    explicit GLTFCharDocumentPtrWrapper(GLTFCharDocument*& _pDocument) : m_pDocument(_pDocument) { }\n' % begin_space)
            source_file.write(u'%s    ~GLTFCharDocumentPtrWrapper() { m_pDocument = nullptr; }\n' % begin_space)
            source_file.write(u'%sprivate:\n' % begin_space)
            source_file.write(u'%s    GLTFCharDocument*& m_pDocument;\n' % begin_space)
            source_file.write(u'%s};\n' % begin_space)
            source_file.write(u'%sGLTFCharDocument* g_json_doc_ptr = nullptr;\n' % begin_space)
            source_file.write(u'\n')

            source_file.write(u'%sbool operator<<(std::shared_ptr<SGlTF>& _pGlTF, const GLTFString& _sContent)\n' % begin_space)
            source_file.write(u'%s{\n' % begin_space)
            source_file.write(u'%s    GLTFCharDocument json_doc;\n' % begin_space)
            source_file.write(u'%s    json_doc.Parse(_sContent.c_str());\n' % begin_space)
            source_file.write(u'%s    if (!json_doc.IsObject()) return false;\n' % begin_space)
            source_file.write(u'%s    return (_pGlTF << json_doc.GetObject());\n' % begin_space)
            source_file.write(u'%s}\n' % begin_space)
            source_file.write(u'\n')

            source_file.write(u'%sbool operator>>(const std::shared_ptr<SGlTF>& _pGlTF, GLTFString& _sContent)\n' % begin_space)
            source_file.write(u'%s{\n' % begin_space)
            source_file.write(u'%s    if (!_pGlTF) return false;\n' % begin_space)
            source_file.write(u'%s    GLTFCharDocument json_doc;\n' % begin_space)
            source_file.write(u'%s    g_json_doc_ptr = &json_doc;\n' % begin_space)
            source_file.write(u'%s    GLTFCharDocumentPtrWrapper wrapper(g_json_doc_ptr);\n' % begin_space)
            source_file.write(u'%s    if (!(_pGlTF >> json_doc)) return false;\n' % begin_space)
            source_file.write(u'%s    GLTFStringBuffer json_string_buffer;\n' % begin_space)
            source_file.write(u'%s    GLTFWriter json_writer(json_string_buffer);\n' % begin_space)
            source_file.write(u'%s    if (!json_doc.Accept(json_writer)) return false;\n' % begin_space)
            source_file.write(u'%s    _sContent = json_string_buffer.GetString();\n' % begin_space)
            source_file.write(u'%s    return true;\n' % begin_space)
            source_file.write(u'%s}\n' % begin_space)
            source_file.write(u'\n')

            source_file.write(u'%sbool operator<<(bool& _rData, const GLTFCharValue& _JsonValue)\n' % begin_space)
            source_file.write(u'%s{\n' % begin_space)
            source_file.write(u'%s    if (!_JsonValue.IsBool()) return false;\n' % begin_space)
            source_file.write(u'%s    _rData = _JsonValue.GetBool();\n' % begin_space)
            source_file.write(u'%s    return true;\n' % begin_space)
            source_file.write(u'%s}\n' % begin_space)
            source_file.write(u'\n')

            source_file.write(u'%sbool operator>>(bool _bData, GLTFCharValue& _JsonValue)\n' % begin_space)
            source_file.write(u'%s{\n' % begin_space)
            source_file.write(u'%s    _JsonValue.SetBool(_bData);\n' % begin_space)
            source_file.write(u'%s    return true;\n' % begin_space)
            source_file.write(u'%s}\n' % begin_space)
            source_file.write(u'\n')

            source_file.write(u'%sbool operator<<(int32_t& _rData, const GLTFCharValue& _JsonValue)\n' % begin_space)
            source_file.write(u'%s{\n' % begin_space)
            source_file.write(u'%s    if (!_JsonValue.IsInt()) return false;\n' % begin_space)
            source_file.write(u'%s    _rData = _JsonValue.GetInt();\n' % begin_space)
            source_file.write(u'%s    return true;\n' % begin_space)
            source_file.write(u'%s}\n' % begin_space)
            source_file.write(u'\n')

            source_file.write(u'%sbool operator>>(int32_t _iData, GLTFCharValue& _JsonValue)\n' % begin_space)
            source_file.write(u'%s{\n' % begin_space)
            source_file.write(u'%s    _JsonValue.SetInt(_iData);\n' % begin_space)
            source_file.write(u'%s    return true;\n' % begin_space)
            source_file.write(u'%s}\n' % begin_space)
            source_file.write(u'\n')

            source_file.write(u'%sbool operator<<(float& _rData, const GLTFCharValue& _JsonValue)\n' % begin_space)
            source_file.write(u'%s{\n' % begin_space)
            source_file.write(u'%s    if (_JsonValue.IsFloat())\n' % begin_space)
            source_file.write(u'%s    {\n' % begin_space)
            source_file.write(u'%s        _rData = _JsonValue.GetFloat();\n' % begin_space)
            source_file.write(u'%s        return true;\n' % begin_space)
            source_file.write(u'%s    }\n' % begin_space)
            source_file.write(u'%s    if (_JsonValue.IsInt())\n' % begin_space)
            source_file.write(u'%s    {\n' % begin_space)
            source_file.write(u'%s        _rData = static_cast<float>(_JsonValue.GetInt());\n' % begin_space)
            source_file.write(u'%s        return true;\n' % begin_space)
            source_file.write(u'%s    }\n' % begin_space)
            source_file.write(u'%s    return false;\n' % begin_space)
            source_file.write(u'%s}\n' % begin_space)
            source_file.write(u'\n')

            source_file.write(u'%sbool operator>>(float _fData, GLTFCharValue& _JsonValue)\n' % begin_space)
            source_file.write(u'%s{\n' % begin_space)
            source_file.write(u'%s    _JsonValue.SetFloat(_fData);\n' % begin_space)
            source_file.write(u'%s    return true;\n' % begin_space)
            source_file.write(u'%s}\n' % begin_space)
            source_file.write(u'\n')

            source_file.write(u'%sbool operator<<(GLTFString& _rData, const GLTFCharValue& _JsonValue)\n' % begin_space)
            source_file.write(u'%s{\n' % begin_space)
            source_file.write(u'%s    if (!_JsonValue.IsString()) return false;\n' % begin_space)
            source_file.write(u'%s    _rData = _JsonValue.GetString();\n' % begin_space)
            source_file.write(u'%s    return true;\n' % begin_space)
            source_file.write(u'%s}\n' % begin_space)
            source_file.write(u'\n')

            source_file.write(u'%sbool operator>>(const GLTFString& _rsData, GLTFCharValue& _JsonValue)\n' % begin_space)
            source_file.write(u'%s{\n' % begin_space)
            source_file.write(u'%s    if (!g_json_doc_ptr) return false;\n' % begin_space)
            source_file.write(u'%s    _JsonValue.SetString(_rsData.c_str(), g_json_doc_ptr->GetAllocator());\n' % begin_space)
            source_file.write(u'%s    return true;\n' % begin_space)
            source_file.write(u'%s}\n' % begin_space)
            source_file.write(u'\n')

            source_file.write(u'%stemplate<typename TData>\n' % begin_space)
            source_file.write(u'%sbool operator<<(std::vector<TData>& _vDatas, const GLTFCharValue& _JsonValue)\n' % begin_space)
            source_file.write(u'%s{\n' % begin_space)
            source_file.write(u'%s    if (!_JsonValue.IsArray()) return false;\n' % begin_space)
            source_file.write(u'%s    std::vector<TData> datas;\n' % begin_space)
            source_file.write(u'%s    const GLTFCharConstArray& json_array = _JsonValue.GetArray();\n' % begin_space)
            source_file.write(u'%s    size_t len = json_array.Size();\n' % begin_space)
            source_file.write(u'%s    if (len == 0) return true;\n' % begin_space)
            source_file.write(u'%s    datas.resize(len);\n' % begin_space)
            source_file.write(u'%s    for (size_t i = 0; i < len; ++i)\n' % begin_space)
            source_file.write(u'%s    {\n' % begin_space)
            source_file.write(u'%s        if (datas[i] << json_array[static_cast<rapidjson::SizeType>(i)]) continue;\n' % begin_space)
            source_file.write(u'%s        return false;\n' % begin_space)
            source_file.write(u'%s    }\n' % begin_space)
            source_file.write(u'%s    _vDatas = datas;\n' % begin_space)
            source_file.write(u'%s    return true;\n' % begin_space)
            source_file.write(u'%s}\n' % begin_space)
            source_file.write(u'\n')

            source_file.write(u'%stemplate<typename TData>\n' % begin_space)
            source_file.write(u'%sbool operator>>(const std::vector<TData>& _vDatas, GLTFCharValue& _JsonValue)\n' % begin_space)
            source_file.write(u'%s{\n' % begin_space)
            source_file.write(u'%s    if (_vDatas.empty() || !g_json_doc_ptr) return false;\n' % begin_space)
            source_file.write(u'%s    GLTFCharValue& json_array = _JsonValue.SetArray();\n' % begin_space)
            source_file.write(u'%s    for (const TData& data : _vDatas)\n' % begin_space)
            source_file.write(u'%s    {\n' % begin_space)
            source_file.write(u'%s        GLTFCharValue json_value;\n' % begin_space)
            source_file.write(u'%s        if (!(data >> json_value)) return false;\n' % begin_space)
            source_file.write(u'%s        json_array.PushBack(json_value, g_json_doc_ptr->GetAllocator());\n' % begin_space)
            source_file.write(u'%s    }\n' % begin_space)
            source_file.write(u'%s    return true;\n' % begin_space)
            source_file.write(u'%s}\n' % begin_space)
            source_file.write(u'\n')

            source_file.write(u'%stemplate<typename TData>\n' % begin_space)
            source_file.write(u'%sbool operator<<(std::map<GLTFString, TData>& _mDatas, const GLTFCharValue& _JsonValue)\n' % begin_space)
            source_file.write(u'%s{\n' % begin_space)
            source_file.write(u'%s    if (!_JsonValue.IsObject()) return false;\n' % begin_space)
            source_file.write(u'%s    std::map<GLTFString, TData> datas;\n' % begin_space)
            source_file.write(u'%s    const GLTFCharConstObject& json_object = _JsonValue.GetObject();\n' % begin_space)
            source_file.write(u'%s    for (GLTFCharConstObject::ConstMemberIterator cit = json_object.MemberBegin(); cit != json_object.MemberEnd(); ++cit)\n' % begin_space)
            source_file.write(u'%s    {\n' % begin_space)
            source_file.write(u'%s        TData data;\n' % begin_space)
            source_file.write(u'%s        if (!(data << cit->value)) return false;\n' % begin_space)
            source_file.write(u'%s        datas.insert(std::make_pair(cit->name.GetString(), data));\n' % begin_space)
            source_file.write(u'%s    }\n' % begin_space)
            source_file.write(u'%s    _mDatas = datas;\n' % begin_space)
            source_file.write(u'%s    return true;\n' % begin_space)
            source_file.write(u'%s}\n' % begin_space)
            source_file.write(u'\n')

            source_file.write(u'%stemplate<typename TData>\n' % begin_space)
            source_file.write(u'%sbool operator>>(const std::map<GLTFString, TData>& _mDatas, GLTFCharValue& _JsonValue)\n' % begin_space)
            source_file.write(u'%s{\n' % begin_space)
            source_file.write(u'%s    if (_mDatas.empty() || !g_json_doc_ptr) return false;\n' % begin_space)
            source_file.write(u'%s    _JsonValue.SetObject();\n' % begin_space)
            source_file.write(u'%s    for (const std::pair<GLTFString, TData>& data : _mDatas)\n' % begin_space)
            source_file.write(u'%s    {\n' % begin_space)
            source_file.write(u'%s        GLTFCharValue json_value;\n' % begin_space)
            source_file.write(u'%s        if (!(data.second >> json_value)) return false;\n' % begin_space)
            source_file.write(u'%s        GLTFCharValue json_key(data.first.c_str(), g_json_doc_ptr->GetAllocator());\n' % begin_space)
            source_file.write(u'%s        _JsonValue.AddMember(json_key, json_value, g_json_doc_ptr->GetAllocator());\n' % begin_space)
            source_file.write(u'%s    }\n' % begin_space)
            source_file.write(u'%s    return true;\n' % begin_space)
            source_file.write(u'%s}\n' % begin_space)
            source_file.write(u'\n')

            for key in self.c11Types:
                c11_type = self.c11Types[key]
                if not isinstance(c11_type, C11TypeStruct):
                    continue

                code_lines = c11_type.codeParserSource()
                for code_line in code_lines:
                    if not code_line:
                        source_file.write(u'\n')
                    else:
                        source_file.write(u'%s%s\n' % (begin_space, code_line))
                source_file.write(u'\n')

            if nameSpace != None:
                source_file.write(u'}\n')

        return (0, u'')

def JSONSchemaToC11(argv):
    parser = argparse.ArgumentParser(description=u'Generate c11 code by json schema.')
    parser.add_argument(u'configFile', metavar=u'config_file', type=file, help=u'The configuration file')
    args = parser.parse_args(argv)

    if not args.configFile:
        return (1, u'Can\'t find the configuration file')

    os.environ['CD'] = os.path.dirname(os.path.abspath(args.configFile.name))

    config = SafeConfigParser(os.environ)
    config.readfp(args.configFile)

    schema_directory = config.get(u'glTF', u'schema_directory')
    code_file_name = config.get(u'glTF', u'code_file_name')
    extensions_schema_directories = []
    for extensions_schema_directory in config.get(u'glTF', u'extensions_schema_directories').split(u','):
        extensions_schema_directories.append(extensions_schema_directory.replace(u'\n', u''))

    major_version = config.get(u'glTF', u'major_version')
    minor_version = config.get(u'glTF', u'minor_version')
    patch_version = config.get(u'glTF', u'patch_version')

    output_header_path = config.get(u'output', u'output_header_path')
    output_source_path = config.get(u'output', u'output_source_path')
    namespace = config.get(u'output', u'namespace')

    if output_header_path != None and not os.path.exists(output_header_path):
        return (1, u'Invalid output header path')
    if output_source_path != None and not os.path.exists(output_source_path):
        return (1, u'Invalid output source path')
    if output_header_path == None:
        output_header_path = u'./'
    if output_source_path == None:
        output_source_path = u'./'

    c11_type_library = C11TypeLibrary()
    c11_type_library.setVersion(major_version, minor_version, patch_version)
    (error_code, error_message) = c11_type_library.addSchemaDirectory(schema_directory, config)
    if error_code != 0:
        return (error_code, error_message)
    for extensions_schema_directory in extensions_schema_directories:
        (error_code, error_message) = c11_type_library.addSchemaDirectory(extensions_schema_directory, config)
        if error_code != 0:
            print error_code, error_message
            return (error_code, error_message)
    (error_code, error_message) = c11_type_library.preprocess()
    if error_code != 0:
        return (error_code, error_message)
    (error_code, error_message) = c11_type_library.generate(code_file_name, outputHeaderPath=output_header_path, outputSourcePath=output_source_path, nameSpace=namespace)
    if error_code != 0:
        return (error_code, error_message)
    return (0, u'')

if __name__ == u'__main__':
    (error_code, error_message) = JSONSchemaToC11(sys.argv[1:])
    if error_code != 0:
        logger.error(error_message)
        sys.exit(error_code)
    else:
        logger.info(u'Success')
    exit(error_code)

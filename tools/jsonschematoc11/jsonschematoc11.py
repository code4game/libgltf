import sys
import os
import argparse
import logging
import json

from c11types import BuildC11Type
from c11types.c11typestruct import C11TypeStruct

logging.basicConfig(level=logging.DEBUG, format=u'%(asctime)s [%(name)s][%(levelname)s](%(filename)s:%(lineno)d) %(message)s')
logger = logging.getLogger(name=u'JSONSchemaToC11')

class C11TypeLibrary(object):
    def __init__(self):
        self.c11Types = dict()

    def addC11Type(self, schemaName, c11Type):
        if schemaName in self.c11Types:
            return (1, u'already have %s in c11 type library' % schemaName)
        self.c11Types[schemaName] = c11Type
        return (0, u'')

    def addSchema(self, rootPath, schemaFileName):
        schema_file_path = os.path.join(rootPath, schemaFileName)
        with open(schema_file_path, u'r') as schema_file:
            schema = json.load(schema_file)
            if schema is None:
                return (1, u'Can\'t parse the schema file %s' % schema_file_path)
            (c11_type, error_code, error_message) = BuildC11Type(schemaFileName, schema, isSchema=True)
            if error_code != 0:
                return (error_code, u'Has error when build - %s' % error_message)
            (error_code, error_message) = self.addC11Type(schemaFileName, c11_type)
            if error_code != 0:
                return (error_code, u'Has error when add - %s' % error_message)
        return (0, u'')

    def addSchemaDirectory(self, schemaDirectory):
        if not os.path.isdir(schemaDirectory):
            return (1, u'The schema directory (%s) is not a valid directory' % schemaDirectory)

        for root_path, _, file_names in os.walk(schemaDirectory, topdown=False):
            for file_name in file_names:
                if not file_name.endswith(u'schema.json'):
                    continue
                (error_code, error_message) = self.addSchema(root_path, file_name)
                if error_code != 0:
                    return (error_code, error_message)
        return (0, u'')

    @classmethod
    def codeHeaderParser(self):
        code_lines = []
        code_lines.append(u'struct SGlTF;')
        code_lines.append(u'bool operator<<(std::shared_ptr<SGlTF>& _pGlTF, const GLTFString& _sContent);')
        code_lines.append(u'bool operator>>(const std::shared_ptr<SGlTF>& _pGlTF, GLTFString& _sContent);')
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
            if nameSpace is not None:
                header_file.write(u'namespace %s\n' % nameSpace)
                header_file.write(u'{\n')
                begin_space = u'    '

            header_file.write(u'#if defined(PLATFORM_WINDOWS)\n')
            header_file.write(u'#   if defined(UNICODE)\n')
            header_file.write(u'%s%s\n' % (begin_space, u'typedef std::wstring                                        GLTFString;'))
            header_file.write(u'#   else\n')
            header_file.write(u'%s%s\n' % (begin_space, u'typedef std::string                                         GLTFString;'))
            header_file.write(u'#   endif\n')
            header_file.write(u'#elif defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS) || defined(PLATFORM_ANDROID) || defined(PLATFORM_IOS)\n')
            header_file.write(u'%s%s\n' % (begin_space, u'typedef std::string                                         GLTFString;'))
            header_file.write(u'#else\n')
            header_file.write(u'#error Sorry, not support your platform.\n')
            header_file.write(u'#endif\n')
            header_file.write(u'\n')

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

            source_file.write(u'%sbool operator>>(bool _rData, GLTFCharValue& _JsonValue)\n' % begin_space)
            source_file.write(u'%s{\n' % begin_space)
            source_file.write(u'%s    //TODO:\n' % begin_space)
            source_file.write(u'%s    return false;\n' % begin_space)
            source_file.write(u'%s}\n' % begin_space)
            source_file.write(u'\n')

            source_file.write(u'%sbool operator<<(int32_t& _rData, const GLTFCharValue& _JsonValue)\n' % begin_space)
            source_file.write(u'%s{\n' % begin_space)
            source_file.write(u'%s    if (!_JsonValue.IsInt()) return false;\n' % begin_space)
            source_file.write(u'%s    _rData = _JsonValue.GetInt();\n' % begin_space)
            source_file.write(u'%s    return true;\n' % begin_space)
            source_file.write(u'%s}\n' % begin_space)
            source_file.write(u'\n')

            source_file.write(u'%sbool operator>>(int32_t _rData, GLTFCharValue& _JsonValue)\n' % begin_space)
            source_file.write(u'%s{\n' % begin_space)
            source_file.write(u'%s    //TODO:\n' % begin_space)
            source_file.write(u'%s    return false;\n' % begin_space)
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

            source_file.write(u'%sbool operator>>(float _rData, GLTFCharValue& _JsonValue)\n' % begin_space)
            source_file.write(u'%s{\n' % begin_space)
            source_file.write(u'%s    //TODO:\n' % begin_space)
            source_file.write(u'%s    return false;\n' % begin_space)
            source_file.write(u'%s}\n' % begin_space)
            source_file.write(u'\n')

            source_file.write(u'%sbool operator<<(GLTFString& _rData, const GLTFCharValue& _JsonValue)\n' % begin_space)
            source_file.write(u'%s{\n' % begin_space)
            source_file.write(u'%s    if (!_JsonValue.IsString()) return false;\n' % begin_space)
            source_file.write(u'%s    _rData = _JsonValue.GetString();\n' % begin_space)
            source_file.write(u'%s    return true;\n' % begin_space)
            source_file.write(u'%s}\n' % begin_space)
            source_file.write(u'\n')

            source_file.write(u'%sbool operator>>(GLTFString _rData, GLTFCharValue& _JsonValue)\n' % begin_space)
            source_file.write(u'%s{\n' % begin_space)
            source_file.write(u'%s    //TODO:\n' % begin_space)
            source_file.write(u'%s    return false;\n' % begin_space)
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
            source_file.write(u'%s    for (size_t i = 0; i < len; ++i) if (!(datas[i] << json_array[static_cast<rapidjson::SizeType>(i)])) return false;\n' % begin_space)
            source_file.write(u'%s    _vDatas = datas;\n' % begin_space)
            source_file.write(u'%s    return true;\n' % begin_space)
            source_file.write(u'%s}\n' % begin_space)
            source_file.write(u'\n')

            source_file.write(u'%stemplate<typename TData>\n' % begin_space)
            source_file.write(u'%sbool operator>>(const std::vector<TData>& _vDatas, GLTFCharValue& _JsonValue)\n' % begin_space)
            source_file.write(u'%s{\n' % begin_space)
            source_file.write(u'%s    //TODO:\n' % begin_space)
            source_file.write(u'%s    return false;\n' % begin_space)
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
            source_file.write(u'%s    //TODO:\n' % begin_space)
            source_file.write(u'%s    return false;\n' % begin_space)
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
    parser.add_argument(u'schemaDirectory', metavar=u'schema_directory', type=type(u''), help=u'The directory contains all schema files')
    parser.add_argument(u'codeFileName', metavar=u'code_file_name', type=type(u''), help=u'The output filename of c11 code')
    parser.add_argument(u'--output_header_path', type=type(u''), help=u'Set the output header path')
    parser.add_argument(u'--output_source_path', type=type(u''), help=u'Set the output source path')
    parser.add_argument(u'--namespace', type=type(u''), help=u'Set the namespace')
    args = parser.parse_args(argv)

    if args.output_header_path != None and not os.path.exists(args.output_header_path):
        return (1, u'Invalid output header path')
    if args.output_source_path != None and not os.path.exists(args.output_source_path):
        return (1, u'Invalid output source path')
    if args.output_header_path == None:
        args.output_header_path = u'./'
    if args.output_source_path == None:
        args.output_source_path = u'./'

    c11_type_library = C11TypeLibrary()
    (error_code, error_message) = c11_type_library.addSchemaDirectory(args.schemaDirectory)
    if error_code != 0:
        return (error_code, error_message)
    (error_code, error_message) = c11_type_library.preprocess()
    if error_code != 0:
        return (error_code, error_message)
    (error_code, error_message) = c11_type_library.generate(args.codeFileName, outputHeaderPath=args.output_header_path, outputSourcePath=args.output_source_path, nameSpace=args.namespace)
    if error_code != 0:
        return (error_code, error_message)
    return (0, u'')

if __name__ == u'__main__':
    (error_code, error_message) = JSONSchemaToC11(sys.argv[1:])
    if error_code != 0:
        logger.error(error_message)
    else:
        logger.info(u'Success')
    exit(error_code)

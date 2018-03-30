'''struct type'''

from c11type import C11Type
from c11typebool import C11TypeBool
from c11typeinteger import C11TypeInteger
from c11typenumber import C11TypeNumber
from c11typestring import C11TypeString
from c11typearray import C11TypeArray
from c11variable import C11Variable

class C11TypeStruct(C11Type):
    '''struct type'''

    def __init__(self):
        C11Type.__init__(self)
        self.parents = dict()
        self.variables = dict()
        self.c11Type = None

    def setSchema(self, schemaName, schemaValue):
        C11Type.setSchema(self, schemaName, schemaValue)
        if u'type' in schemaValue:
            schemaValueType = schemaValue[u'type']
            if schemaValueType == u'bool' or schemaValueType == u'boolean':
                self.c11Type = C11TypeBool()
            elif schemaValueType == u'integer':
                self.c11Type = C11TypeInteger()
            elif schemaValueType == u'number':
                self.c11Type = C11TypeNumber()
            elif schemaValueType == u'string':
                self.c11Type = C11TypeString()
            elif schemaValueType == u'array':
                self.c11Type = C11TypeArray()
        if self.c11Type == None:
            if u'allOf' in schemaValue:
                schemaValueAllOf = schemaValue[u'allOf']
                for schemaValueAllOfItem in schemaValueAllOf:
                    if u'$ref' in schemaValueAllOfItem:
                        self.parents[schemaValueAllOfItem[u'$ref']] = None
            if u'properties' in schemaValue:
                schemaValueProperties = schemaValue[u'properties']
                for key in schemaValueProperties:
                    self.variables[key] = C11Variable(key, schemaValueProperties[key])

    def haveParents(self):
        return len(self.parents) > 0

    def getParentTypeNames(self, recursion=True, withDeclare=False, asVariable=False, withObject=False):
        parentTypeNames = []
        for key in self.parents:
            parent = self.parents[key]
            if recursion:
                for parentParentTypeName in parent.getParentTypeNames():
                    parentTypeNames.append(parentParentTypeName)
            parentTypeNames.append(parent.codeTypeName(withDeclare, asVariable))
        if  withObject and len(parentTypeNames) <= 0:
            parentTypeNames.append(u'SObject')
        return parentTypeNames

    def getVariableKeys(self, includeParent=False):
        variableKeys = []
        for key in self.variables:
            variableKeys.append(key)
        for key in self.parents:
            parent = self.parents[key]
            parentVariableKeys = parent.getVariableKeys(True)
            for parentVariableKey in parentVariableKeys:
                variableKeys.append(parentVariableKey)
        return variableKeys

    def getVariables(self, includeParent=False):
        variables = self.variables
        for parentKey in self.parents:
            parent = self.parents[parentKey]
            parentVariableKeys = parent.getVariableKeys(True)
            if len(parentVariableKeys) > 0:
                variables = {key: value for key, value in variables.items() if key not in parentVariableKeys }
            parentVariableKeys = None
        return variables.values()

    def revise(self, c11Types):
        for key in self.parents:
            if key not in c11Types:
                return (1, u'Can\'t find the parent %s in %s' % (key, self.codeTypeName()))
            self.parents[key] = c11Types[key]
        for key in self.variables:
            variable = self.variables[key]
            (errorCode, errorMessage) = variable.revise(c11Types)
            if errorCode != 0:
                return (errorCode, errorMessage)
        return (0, u'')

    def codeTypeName(self, withDeclare=False, asVariable=False):
        codeLine = self.typeName
        if self.typeName == None:
            #TODO:
            print u'dd'
        if withDeclare:
            codeLine = u'struct ' + codeLine
        if asVariable:
            codeLine = u'std::shared_ptr<%s>' % codeLine
        return codeLine

    def codeInheritTypes(self):
        if len(self.parents) <= 0:
            return u' : SObject'
        code = u''
        for parent in self.parents:
            code = code + u', %s' % self.parents[parent].codeTypeName()
        code = u' :' + code[1:]
        return code

    def codeHeader(self, codeTypeNames):
        codeLines = []
        if self.codeTypeName() in codeTypeNames:
            return codeLines
        for key in self.parents:
            parent = self.parents[key]
            if parent.codeTypeName() in codeTypeNames:
                continue
            for parentCodeLine in parent.codeHeader(codeTypeNames):
                codeLines.append(parentCodeLine)
            codeLines.append(u'')

        codeLines.append(u'/*!')
        codeLines.append(u' * struct: %s' % self.codeTypeName())
        if hasattr(self, u'description'):
            codeLines.append(u' * ' + self.description)
        codeLines.append(u' */')
        codeLines.append(u'struct %s%s' % (self.codeTypeName(), self.codeInheritTypes()))
        codeLines.append(u'{')
        codeLines.append(u'    %s();' % self.codeTypeName())
        codeLines.append(u'')
        codeLines.append(u'    // Check valid')
        codeLines.append(u'    operator bool() const;')

        if self.c11Type != None:
            codeLines.append(u'')
            codeLines.append(u'    operator %s() const;' % self.c11Type.codeTypeName())
            codeLines.append(u'')
            codeLines.append(u'    %s %sValue;' % (self.c11Type.codeTypeName(), self.c11Type.codeTypeName()))

        variables = self.getVariables()
        if variables is not None and len(variables) > 0:
            codeLines.append(u'')
            for variable in variables:
                if variable.hasComment():
                    codeLines.append(u'    // %s' % variable.codeComment())
                codeLines.append(u'    %s;' % variable.codeDeclare())

        if self.manualCodeHeaders is not None and len(self.manualCodeHeaders) > 0:
            codeLines.append(u'')
            codeLines.append(u'    // Manual code lines')
            for code_header_line in self.manualCodeHeaders:
                codeLines.append(u'    %s' % code_header_line.replace(u'\n', u''))

        codeLines.append(u'};')
        return codeLines

    def codeSource(self, codeTypeNames):
        codeLines = []
        if self.codeTypeName() in codeTypeNames:
            return codeLines
        for key in self.parents:
            parent = self.parents[key]
            if parent.codeTypeName() in codeTypeNames:
                continue
            for parentCodeLine in parent.codeSource(codeTypeNames):
                codeLines.append(parentCodeLine)
            codeLines.append(u'')

        codeLines.append(u'%s::%s()' % (self.codeTypeName(), self.codeTypeName()))
        beginConstructorDefault = True
        parentTypeNames = self.getParentTypeNames(recursion=False, withObject=True)
        for parentTypeName in parentTypeNames:
            if beginConstructorDefault:
                codeLines.append(u'    : %s()' % parentTypeName)
                beginConstructorDefault = False
            else:
                codeLines.append(u'    , %s()' % parentTypeName)
        if self.c11Type != None:
            if beginConstructorDefault:
                codeLines.append(u'    : %sValue(%s)' % (self.c11Type.codeTypeName(), self.c11Type.codeDefaultValue(None)))
                beginConstructorDefault = False
            else:
                codeLines.append(u'    , %sValue(%s)' % (self.c11Type.codeTypeName(), self.c11Type.codeDefaultValue(None)))
        variables = self.getVariables()
        for variable in variables:
            if beginConstructorDefault:
                codeLines.append(u'    : %s' % variable.codeConstructorDefault())
                beginConstructorDefault = False
            else:
                codeLines.append(u'    , %s' % variable.codeConstructorDefault())
        if self.manualCodeSourcesVariable is not None and len(self.manualCodeSourcesVariable) > 0:
            codeLines.append(u'    // Manual code lines')
            for variable in self.manualCodeSourcesVariable:
                if beginConstructorDefault:
                    codeLines.append(u'    : %s' % variable)
                    beginConstructorDefault = False
                else:
                    codeLines.append(u'    , %s' % variable)

        codeLines.append(u'{')
        codeLines.append(u'    //')
        codeLines.append(u'}')
        codeLines.append(u'')
        codeLines.append(u'%s::operator bool() const' % self.codeTypeName())
        codeLines.append(u'{')
        codeLines.append(u'    //TODO:')
        codeLines.append(u'    return true;')
        codeLines.append(u'}')
        if self.c11Type != None:
            codeLines.append(u'')
            codeLines.append(u'%s::operator %s() const' % (self.codeTypeName(), self.c11Type.codeTypeName()))
            codeLines.append(u'{')
            codeLines.append(u'    return %sValue;' % (self.c11Type.codeTypeName()))
            codeLines.append(u'}')

        if self.manualCodeSourcesFunction is not None and len(self.manualCodeSourcesFunction) > 0:
            codeLines.append(u'')
            codeLines.append(u'// Manual code lines')
            for code_source_function_line in self.manualCodeSourcesFunction:
                codeLines.append(u'%s' % code_source_function_line.replace(u'\n', u''))

        return codeLines

    def codeParserHeader(self):
        codeLines = []
        codeLines.append(u'bool operator<<(%s& _rData, const GLTFCharValue& _JsonValue);' % (self.codeTypeName(withDeclare=True)))
        codeLines.append(u'bool operator>>(const %s& _rData, GLTFCharValue& _JsonValue);' % (self.codeTypeName(withDeclare=True)))
        codeLines.append(u'bool operator<<(%s& _pData, const GLTFCharValue& _JsonValue);' % (self.codeTypeName(withDeclare=True, asVariable=True)))
        codeLines.append(u'bool operator>>(const %s& _pData, GLTFCharValue& _JsonValue);' % (self.codeTypeName(withDeclare=True, asVariable=True)))
        codeLines.append(u'bool operator<<(std::vector<%s>& _vDatas, const GLTFCharValue& _JsonValue);' % (self.codeTypeName(withDeclare=True, asVariable=True)))
        codeLines.append(u'bool operator>>(const std::vector<%s>& _vDatas, GLTFCharValue& _JsonValue);' % (self.codeTypeName(withDeclare=True, asVariable=True)))
        return codeLines

    def codeParserSource(self):
        codeLines = []
        codeLines.append(u'bool operator<<(%s& _rData, const GLTFCharValue& _JsonValue)' % (self.codeTypeName()))
        codeLines.append(u'{')
        if self.manualCodeParsersFrom is not None and len(self.manualCodeParsersFrom) > 0:
            codeLines.append(u'    // Manual code lines')
            for code_parser in self.manualCodeParsersFrom:
                codeLines.append(u'    %s' % code_parser.replace(u'\n', u''))
        else:
            parentTypes = self.getParentTypeNames(recursion=False)
            for parentType in parentTypes:
                codeLines.append(u'    {')
                codeLines.append(u'        %s& super_ptr = _rData;' % parentType)
                codeLines.append(u'        if (!(super_ptr << _JsonValue)) return false;')
                codeLines.append(u'    }')

            if self.c11Type != None:
                codeLines.append(u'    if (!(_rData.%sValue << _JsonValue)) return false;' % self.c11Type.codeTypeName())
            else:
                variables = self.getVariables()
                for variable in variables:
                    codeParserLines = variable.codeParser()
                    for codeParserLine in codeParserLines:
                        codeLines.append(u'    %s' % codeParserLine)

            codeLines.append(u'    _rData.schemaType = GLTFTEXT("%s");' % (self.schemaName))
            codeLines.append(u'    return true;')
        codeLines.append(u'}')
        codeLines.append(u'')

        codeLines.append(u'bool operator>>(const %s& _rData, GLTFCharValue& _JsonValue)' % (self.codeTypeName()))
        codeLines.append(u'{')
        if self.manualCodeParsersTo is not None and len(self.manualCodeParsersTo) > 0:
            codeLines.append(u'    // Manual code lines')
            for code_parser in self.manualCodeParsersTo:
                codeLines.append(u'    %s' % code_parser.replace(u'\n', u''))
        else:
            codeLines.append(u'    if (!g_json_doc_ptr) return false;')
            codeLines.append(u'    _JsonValue.SetObject();')

            parentTypes = self.getParentTypeNames(recursion=False)
            for parentType in parentTypes:
                codeLines.append(u'    {')
                codeLines.append(u'        const %s& super_ptr = _rData;' % parentType)
                codeLines.append(u'        if (!(super_ptr >> _JsonValue)) return false;')
                codeLines.append(u'    }')

            if self.c11Type != None:
                codeLines.append(u'    if (!(_rData.%sValue >> _JsonValue)) return false;' % self.c11Type.codeTypeName())
            else:
                variables = self.getVariables()
                for variable in variables:
                    codeParserLines = variable.codeParser(isSet=False)
                    for codeParserLine in codeParserLines:
                        codeLines.append(u'    %s' % codeParserLine)

            codeLines.append(u'    return true;')
        codeLines.append(u'}')
        codeLines.append(u'')

        codeLines.append(u'bool operator<<(%s& _pData, const GLTFCharValue& _JsonValue)' % (self.codeTypeName(asVariable=True)))
        codeLines.append(u'{')
        codeLines.append(u'    %s data_ptr = !!_pData ? _pData : std::make_shared<%s>();' % (self.codeTypeName(asVariable=True), self.codeTypeName()))
        codeLines.append(u'    if (!(*data_ptr << _JsonValue)) return false;')
        codeLines.append(u'    _pData = data_ptr;')
        codeLines.append(u'    return true;')
        codeLines.append(u'}')
        codeLines.append(u'')

        codeLines.append(u'bool operator>>(const %s& _pData, GLTFCharValue& _JsonValue)' % (self.codeTypeName(asVariable=True)))
        codeLines.append(u'{')
        codeLines.append(u'    if (!_pData) return false;')
        codeLines.append(u'    return (*_pData >> _JsonValue);')
        codeLines.append(u'}')
        codeLines.append(u'')

        codeLines.append(u'bool operator<<(std::vector<%s>& _vDatas, const GLTFCharValue& _JsonValue)' % (self.codeTypeName(asVariable=True)))
        codeLines.append(u'{')
        codeLines.append(u'    return operator<< <%s>(_vDatas, _JsonValue);' % (self.codeTypeName(asVariable=True)))
        codeLines.append(u'}')
        codeLines.append(u'')

        codeLines.append(u'bool operator>>(const std::vector<%s>& _vDatas, GLTFCharValue& _JsonValue)' % (self.codeTypeName(asVariable=True)))
        codeLines.append(u'{')
        codeLines.append(u'    return operator>> <%s>(_vDatas, _JsonValue);' % (self.codeTypeName(asVariable=True)))
        codeLines.append(u'}')
        return codeLines

    def codeDefaultValue(self, schemaDefaultValue):
        return u'nullptr'

    def codeJsonCheck(self):
        if self.c11Type == None:
            return u'IsObject()'
        return self.c11Type.codeJsonCheck()

    def codeJsonSet(self, dataName, variableName):
        return u'if (!(%s.%s << _JsonValue[GLTFTEXT("%s")])) return false;' % (dataName, variableName, variableName)

    def codeJsonGet(self, dataName, variableName):
        return u'if (!(%s.%s >> _JsonValue[GLTFTEXT("%s")])) return false;' % (dataName, variableName, variableName)

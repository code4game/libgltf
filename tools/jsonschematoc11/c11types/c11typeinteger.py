from .c11type import C11Type

class C11TypeInteger(C11Type):
    def __init__(self):
        """construct and declare some vars."""
        C11Type.__init__(self)
        self.typeName = u'int32_t'

    def setSchema(self, schemaName, schemaValue):
        C11Type.setSchema(self, schemaName, schemaValue)
        self.typeName = u'int32_t'

    @classmethod
    def codeDefaultValue(self, schemaDefaultValue):
        if schemaDefaultValue != None:
            return u'%d' % schemaDefaultValue
        return u'0'

    @classmethod
    def codeDefaultValueArray(self, schemaDefaultValues):
        if schemaDefaultValues is None or isinstance(schemaDefaultValues, list) or len(schemaDefaultValues) <= 0:
            return u''
        code_default_value = u''
        for schema_value in schemaDefaultValues:
            if len(code_default_value) > 0:
                code_default_value = code_default_value + u', '
            code_default_value = code_default_value + u'%d' % schema_value
        return u'{ %s }' % code_default_value

    @classmethod
    def codeJsonCheck(self):
        return u'IsInt()'

    @classmethod
    def codeJsonSet(self, dataName, variableName):
        return u'%s.%s = _JsonValue[GLTFTEXT("%s")].GetInt();' % (dataName, variableName, variableName)

    @classmethod
    def codeJsonGet(self, dataName, variableName):
        return u'_JsonValue[GLTFTEXT("%s")].SetInt(%s.%s);' % (variableName, dataName, variableName)

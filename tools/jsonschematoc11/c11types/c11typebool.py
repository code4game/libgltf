from .c11type import C11Type

class C11TypeBool(C11Type):
    def __init__(self):
        """Construct and declare some vars."""
        C11Type.__init__(self)
        self.typeName = u'bool'

    def setSchema(self, schemaName, schemaValue):
        C11Type.setSchema(self, schemaName, schemaValue)

    def codeDefaultValue(self, schemaDefaultValue):
        if schemaDefaultValue is not None and schemaDefaultValue is True:
            return u'true'
        return u'false'

    def codeDefaultValueArray(self, schemaDefaultValues):
        if schemaDefaultValues is None\
            or not isinstance(schemaDefaultValues, list)\
            or len(schemaDefaultValues) <= 0:
            return u''
        code_default_value = u''
        for schema_value in schemaDefaultValues:
            if len(code_default_value) > 0:
                code_default_value = code_default_value + u', '
            if schema_value == True:
                code_default_value = code_default_value + u'true'
            else:
                code_default_value = code_default_value + u'false'
        return u'{ %s }' % code_default_value

    def codeJsonCheck(self):
        return u'IsBool()'

    def codeJsonSet(self, dataName, variableName):
        return u'%s.%s = _JsonValue[GLTFTEXT("%s")].GetBool();' % (dataName, variableName, variableName)

    def codeJsonGet(self, dataName, variableName):
        return u'_JsonValue[GLTFTEXT("%s")].SetBool(%s.%s);' % (variableName, dataName, variableName)

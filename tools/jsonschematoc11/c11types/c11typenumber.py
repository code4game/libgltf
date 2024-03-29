from .c11type import C11Type

class C11TypeNumber(C11Type):
    def __init__(self):
        """Construct and declare some vars."""
        C11Type.__init__(self)
        self.typeName = u'float'

    def setSchema(self, schemaName, schemaValue):
        C11Type.setSchema(self, schemaName, schemaValue)
        self.typeName = u'float'

    def codeDefaultValue(self, schemaDefaultValue):
        if schemaDefaultValue != None:
            return u'%ff' % schemaDefaultValue
        return u'0.0f'

    def codeDefaultValueArray(self, schemaDefaultValues):
        if schemaDefaultValues is None\
            or not isinstance(schemaDefaultValues, list)\
            or len(schemaDefaultValues) <= 0:
            return u''
        code_default_value = u''
        for schema_value in schemaDefaultValues:
            if len(code_default_value) > 0:
                code_default_value = code_default_value + u', '
            code_default_value = code_default_value + u'%ff' % schema_value
        return u'{ %s }' % code_default_value

    def codeJsonCheck(self):
        return u'IsNumber()'

    def codeJsonSet(self, dataName, variableName):
        return u'%s.%s = _JsonValue["%s"].GetFloat();' % (dataName, variableName, variableName)

    def codeJsonGet(self, dataName, variableName):
        return u'_JsonValue["%s"].SetFloat(%s.%s);' % (variableName, dataName, variableName)

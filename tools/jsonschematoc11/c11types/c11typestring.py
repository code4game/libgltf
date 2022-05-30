from .c11type import C11Type

class C11TypeString(C11Type):

    """strig type."""

    def __init__(self):
        """Construct and declare some vars."""
        C11Type.__init__(self)
        self.typeName = u'std::string'

    def setSchema(self, schemaName, schemaValue):
        C11Type.setSchema(self, schemaName, schemaValue)

    def codeDefaultValue(self, schemaDefaultValue):
        if schemaDefaultValue != None:
            return u'"%s"' % schemaDefaultValue
        return u'""'

    def codeDefaultValueArray(self, schemaDefaultValues):
        if schemaDefaultValues is None\
            or not isinstance(schemaDefaultValues, list)\
            or len(schemaDefaultValues) <= 0:
            return u''
        code_default_value = u''
        for schema_value in schemaDefaultValues:
            if len(code_default_value) > 0:
                code_default_value = code_default_value + u', '
            code_default_value = code_default_value + u'"%s"' % schema_value
        code_default_value = code_default_value[0:len(code_default_value) - 2]
        return u'{ %s }' % code_default_value

    def codeJsonCheck(self):
        return u'IsString()'

    def codeJsonSet(self, dataName, variableName):
        return u'%s.%s = _JsonValue["%s"].GetString();' % (dataName, variableName, variableName)

    def codeJsonGet(self, dataName, variableName):
        return u'_JsonValue["%s"].SetString(%s.%s.c_str(), _rData.doc->GetAllocator());' % (variableName, dataName, variableName)

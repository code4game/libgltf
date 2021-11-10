"""string"""

from .c11type import C11Type

class C11TypeString(C11Type):

    """strig type."""

    def __init__(self):
        """construct and declare some vars."""
        C11Type.__init__(self)
        self.typeName = u'string_t'

    def setSchema(self, schemaName, schemaValue):
        C11Type.setSchema(self, schemaName, schemaValue)

    @classmethod
    def codeDefaultValue(self, schemaDefaultValue):
        if schemaDefaultValue != None:
            return u'GLTFTEXT("%s")' % schemaDefaultValue
        return u'GLTFTEXT("")'

    @classmethod
    def codeDefaultValueArray(self, schemaDefaultValues):
        if schemaDefaultValues is None\
            or not isinstance(schemaDefaultValues, list)\
            or len(schemaDefaultValues) <= 0:
            return u''
        code_default_value = u''
        for schema_value in schemaDefaultValues:
            if len(code_default_value) > 0:
                code_default_value = code_default_value + u', '
            code_default_value = code_default_value + u'GLTFTEXT("%s")' % schema_value
        code_default_value = code_default_value[0:len(code_default_value) - 2]
        return u'{ %s }' % code_default_value

    @classmethod
    def codeJsonCheck(self):
        return u'IsString()'

    @classmethod
    def codeJsonSet(self, dataName, variableName):
        return u'%s.%s = _JsonValue[GLTFTEXT("%s")].GetString();' % (dataName, variableName, variableName)

    @classmethod
    def codeJsonGet(self, dataName, variableName):
        return u'_JsonValue[GLTFTEXT("%s")].SetString(%s.%s.c_str(), _rData.doc->GetAllocator());' % (variableName, dataName, variableName)

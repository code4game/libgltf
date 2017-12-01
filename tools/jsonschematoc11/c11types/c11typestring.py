'''string'''

from c11type import C11Type

class C11TypeString(C11Type):
    '''strig type'''

    def __init__(self):
        C11Type.__init__(self)
        self.typeName = u'std::wstring'

    def setSchema(self, schemaName, schemaValue):
        C11Type.setSchema(self, schemaName, schemaValue)

    def codeDefaultValue(self, schemaDefaultValue):
        if schemaDefaultValue != None:
            return u'L"%s"' % schemaDefaultValue
        return u'L""'

    def codeDefaultValueArray(self, schemaDefaultValue):
        if schemaDefaultValue is None or isinstance(schemaDefaultValue) != list or len(schemaDefaultValue) <= 0:
            return u''
        code_default_value = u''
        for i in range(0, len(schemaDefaultValue)):
            code_default_value = code_default_value + u'L"%s", ' % schemaDefaultValue[i]
        code_default_value = code_default_value[0:len(code_default_value) - 2]
        return u'{ %s }' % code_default_value

    @classmethod
    def codeJsonCheck(cls):
        return u'IsString()'

    def codeJsonSet(self, dataName, variableName):
        return u'%s->%s = _JsonValue[L"%s"].GetString();' % (dataName, variableName, variableName)

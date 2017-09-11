from c11type import C11Type

class C11TypeString(C11Type):
    def __init__(self):
        C11Type.__init__(self)
        self.typeName = u'std::wstring'

    def setSchema(self, schemaName, schemaValue):
        C11Type.setSchema(self, schemaName, schemaValue)

    def codeDefaultValue(self):
        return u'L""'

    def codeJsonCheck(self):
        return u'IsString()'

    def codeJsonSet(self, dataName, variableName):
        return u'%s->%s = _JsonValue[L"%s"].GetString();' % (dataName, variableName, variableName)

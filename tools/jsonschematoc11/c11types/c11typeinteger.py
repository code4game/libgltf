from c11type import C11Type

class C11TypeInteger(C11Type):
    def __init__(self):
        C11Type.__init__(self)
        self.typeName = u'int32_t'

    def setSchema(self, schemaName, schemaValue):
        C11Type.setSchema(self, schemaName, schemaValue)
        self.typeName = u'int32_t'

    def codeDefaultValue(self):
        return u'0'

    def codeJsonCheck(self):
        return u'IsInt()'

    def codeJsonSet(self, dataName, variableName):
        return u'%s->%s = _JsonValue[L"%s"].GetInt();' % (dataName, variableName, variableName)

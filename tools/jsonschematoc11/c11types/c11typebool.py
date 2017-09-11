from c11type import C11Type

class C11TypeBool(C11Type):
    def __init__(self):
        C11Type.__init__(self)
        self.typeName = u'bool'

    def setSchema(self, schemaName, schemaValue):
        C11Type.setSchema(self, schemaName, schemaValue)

    def codeDefaultValue(self):
        return u'false'

    def codeJsonCheck(self):
        return u'IsBool()'

    def codeJsonSet(self, dataName, variableName):
        return u'%s->%s = _JsonValue[L"%s"].GetBool();' % (dataName, variableName, variableName)

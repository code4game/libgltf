from c11type import C11Type

class C11TypeNumber(C11Type):
    def __init__(self):
        C11Type.__init__(self)
        self.typeName = u'float'

    def setSchema(self, schemaName, schemaValue):
        C11Type.setSchema(self, schemaName, schemaValue)
        self.typeName = u'float'

    def codeDefaultValue(self):
        return u'0.0f'

    def codeJsonCheck(self):
        return u'IsFloat()'

    def codeJsonSet(self, dataName, variableName):
        return u'%s->%s = _JsonValue[L"%s"].GetFloat();' % (dataName, variableName, variableName)

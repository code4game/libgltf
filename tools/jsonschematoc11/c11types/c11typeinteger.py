from c11type import C11Type

class C11TypeInteger(C11Type):
    def __init__(self):
        C11Type.__init__(self)
        self.typeName = u'int32_t'

    def setSchema(self, schemaName, schemaValue):
        C11Type.setSchema(self, schemaName, schemaValue)
        self.typeName = u'int32_t'

    def codeDefaultValue(self, schemaDefaultValue):
        if schemaDefaultValue != None:
            return u'%d' % schemaDefaultValue
        return u'0'

    @classmethod
    def codeDefaultValueArray(cls, schemaDefaultValue):
        if schemaDefaultValue is None or isinstance(schemaDefaultValue) != list or len(schemaDefaultValue) <= 0:
            return u''
        codeDefauleValue = u''
        for i in range(0, len(schemaDefaultValue)):
            codeDefauleValue = codeDefauleValue + u'%d, ' % schemaDefaultValue[i]
        codeDefauleValue = codeDefauleValue[0:len(codeDefauleValue) - 2]
        return u'{ %s }' % codeDefauleValue

    @classmethod
    def codeJsonCheck(cls):
        return u'IsInt()'

    @classmethod
    def codeJsonSet(cls, dataName, variableName):
        return u'%s->%s = _JsonValue[L"%s"].GetInt();' % (dataName, variableName, variableName)

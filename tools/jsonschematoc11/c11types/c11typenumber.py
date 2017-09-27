from c11type import C11Type

class C11TypeNumber(C11Type):
    def __init__(self):
        C11Type.__init__(self)
        self.typeName = u'float'

    def setSchema(self, schemaName, schemaValue):
        C11Type.setSchema(self, schemaName, schemaValue)
        self.typeName = u'float'

    def codeDefaultValue(self, schemaDefaultValue):
        if schemaDefaultValue != None:
            return u'%ff' % schemaDefaultValue
        return u'0.0f'

    def codeDefaultValueArray(self, schemaDefaultValue):
        if schemaDefaultValue == None or type(schemaDefaultValue) != list or len(schemaDefaultValue) <= 0:
            return u''
        codeDefauleValue = u''
        for i in range(0, len(schemaDefaultValue)):
            codeDefauleValue = codeDefauleValue + u'%ff, ' % schemaDefaultValue[i]
        codeDefauleValue = codeDefauleValue[0:len(codeDefauleValue) - 2]
        return u'{ %s }' % codeDefauleValue

    def codeJsonCheck(self):
        return u'IsFloat()'

    def codeJsonSet(self, dataName, variableName):
        return u'%s->%s = _JsonValue[L"%s"].GetFloat();' % (dataName, variableName, variableName)

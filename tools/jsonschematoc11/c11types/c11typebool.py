from c11type import C11Type

class C11TypeBool(C11Type):
    def __init__(self):
        C11Type.__init__(self)
        self.typeName = u'bool'

    def setSchema(self, schemaName, schemaValue):
        C11Type.setSchema(self, schemaName, schemaValue)

    def codeDefaultValue(self, schemaDefaultValue):
        if schemaDefaultValue != None and schemaDefaultValue == True:
            return u'true'
        return u'false'

    def codeDefaultValueArray(self, schemaDefaultValue):
        if schemaDefaultValue == None or type(schemaDefaultValue) != list or len(schemaDefaultValue) <= 0:
            return u''
        codeDefauleValue = u''
        for i in range(0, len(schemaDefaultValue)):
            if schemaDefaultValue[i] == True:
                codeDefauleValue = codeDefauleValue + u'true, '
            else:
                codeDefauleValue = codeDefauleValue + u'false, '
        codeDefauleValue = codeDefauleValue[0:len(codeDefauleValue) - 2]
        return u'{ %s }' % codeDefauleValue

    def codeJsonCheck(self):
        return u'IsBool()'

    def codeJsonSet(self, dataName, variableName):
        return u'%s->%s = _JsonValue[L"%s"].GetBool();' % (dataName, variableName, variableName)

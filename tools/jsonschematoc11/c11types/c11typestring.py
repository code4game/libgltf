from c11type import C11Type

class C11TypeString(C11Type):
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
        if schemaDefaultValue == None or type(schemaDefaultValue) != list or len(schemaDefaultValue) <= 0:
            return u''
        codeDefauleValue = u''
        for i in range(0, len(schemaDefaultValue)):
            codeDefauleValue = codeDefauleValue + u'L"%s", ' % schemaDefaultValue[i]
        codeDefauleValue = codeDefauleValue[0:len(codeDefauleValue) - 2]
        return u'{ %s }' % codeDefauleValue

    def codeJsonCheck(self):
        return u'IsString()'

    def codeJsonSet(self, dataName, variableName):
        return u'%s->%s = _JsonValue[L"%s"].GetString();' % (dataName, variableName, variableName)

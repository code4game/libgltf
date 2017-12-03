from c11type import C11Type

class C11TypeBool(C11Type):
    def __init__(self):
        C11Type.__init__(self)
        self.typeName = u'bool'

    def setSchema(self, schemaName, schemaValue):
        C11Type.setSchema(self, schemaName, schemaValue)

    @classmethod
    def codeDefaultValue(cls, schemaDefaultValue):
        if schemaDefaultValue is not None and schemaDefaultValue is True:
            return u'true'
        return u'false'

    @classmethod
    def codeDefaultValueArray(cls, schemaDefaultValue):
        if schemaDefaultValue is None\
            or isinstance(schemaDefaultValue) is not list\
            or len(schemaDefaultValue) <= 0:
            return u''
        codeDefauleValue = u''
        for i in range(0, len(schemaDefaultValue)):
            if schemaDefaultValue[i] == True:
                codeDefauleValue = codeDefauleValue + u'true, '
            else:
                codeDefauleValue = codeDefauleValue + u'false, '
        codeDefauleValue = codeDefauleValue[0:len(codeDefauleValue) - 2]
        return u'{ %s }' % codeDefauleValue

    @classmethod
    def codeJsonCheck(cls):
        return u'IsBool()'

    @classmethod
    def codeJsonSet(cls, dataName, variableName):
        return u'%s->%s = _JsonValue[L"%s"].GetBool();' % (dataName, variableName, variableName)

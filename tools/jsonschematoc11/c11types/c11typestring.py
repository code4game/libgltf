'''string'''

from c11type import C11Type

class C11TypeString(C11Type):
    '''strig type'''

    def __init__(self):
        C11Type.__init__(self)
        self.typeName = u'GLTFString'

    def setSchema(self, schemaName, schemaValue):
        C11Type.setSchema(self, schemaName, schemaValue)

    @classmethod
    def codeDefaultValue(cls, schemaDefaultValue):
        if schemaDefaultValue != None:
            return u'GLTFTEXT("%s")' % schemaDefaultValue
        return u'GLTFTEXT("")'

    @classmethod
    def codeDefaultValueArray(self, schemaDefaultValue):
        if schemaDefaultValue is None\
            or not isinstance(schemaDefaultValue, list)\
            or len(schemaDefaultValue) <= 0:
            return u''
        code_default_value = u''
        for i in range(0, len(schemaDefaultValue)):
            code_default_value = code_default_value + u'GLTFTEXT("%s"), ' % schemaDefaultValue[i]
        code_default_value = code_default_value[0:len(code_default_value) - 2]
        return u'{ %s }' % code_default_value

    @classmethod
    def codeJsonCheck(cls):
        return u'IsString()'

    @classmethod
    def codeJsonSet(cls, dataName, variableName):
        return u'%s.%s = _JsonValue[GLTFTEXT("%s")].GetString();' % (dataName, variableName, variableName)

    @classmethod
    def codeJsonGet(cls, dataName, variableName):
        return u'_JsonValue[GLTFTEXT("%s")].SetString(%s.%s.c_str(), g_json_doc_ptr->GetAllocator());' % (variableName, dataName, variableName)

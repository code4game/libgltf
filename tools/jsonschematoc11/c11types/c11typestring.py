from c11type import C11Type

class C11TypeString(C11Type):
    def __init__(self):
        C11Type.__init__(self)
        self.typeName = u'std::wstring'

    def setSchema(self, schemaName, schemaValue):
        C11Type.setSchema(self, schemaName, schemaValue)

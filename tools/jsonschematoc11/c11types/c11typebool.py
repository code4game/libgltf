from c11type import C11Type

class C11TypeBool(C11Type):
    def __init__(self):
        C11Type.__init__(self)
        self.typeName = u'bool'

    def setSchema(self, schemaName, schemaValue):
        C11Type.setSchema(self, schemaName, schemaValue)

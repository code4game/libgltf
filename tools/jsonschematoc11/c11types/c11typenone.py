from c11type import C11Type

class C11TypeNone(C11Type):
    def __init__(self):
        C11Type.__init__(self)
        self.typeName = u'none'

    def setSchema(self, schemaName, schemaValue):
        C11Type.setSchema(self, schemaName, schemaValue)
        self.typeName = u'none'

    def codeTypeName(self, withDeclare=False, asVariable=False):
        codeLine = self.typeName
        if withDeclare:
            codeLine = u'struct ' + codeLine
        if asVariable:
            codeLine = u'std::shared_ptr<%s>' % codeLine
        return codeLine

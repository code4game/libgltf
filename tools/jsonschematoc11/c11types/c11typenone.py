from .c11type import C11Type

class C11TypeNone(C11Type):
    def __init__(self):
        """Construct and declare some vars."""
        C11Type.__init__(self)
        self.typeName = u'none'

    def setSchema(self, schemaName, schemaValue):
        C11Type.setSchema(self, schemaName, schemaValue)
        self.typeName = u'none'

    def codeTypeName(self, withDeclare=False, asVariable=False, withDocument=False):
        codeLine = self.typeName
        if withDeclare:
            if withDocument:
                codeLine = u'TDataDoc<struct %s>' % codeLine
            else:
                codeLine = u'struct %s' % codeLine
        else:
            if withDocument:
                codeLine = u'TDataDoc<%s>' % codeLine
        if asVariable:
            if withDocument:
                codeLine = u'TDataDoc<std::shared_ptr<%s>>' % codeLine
            else:
                codeLine = u'std::shared_ptr<%s>' % codeLine
        return codeLine

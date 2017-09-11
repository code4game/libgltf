class C11Type(object):
    def __init__(self):
        self.schemaName = None
        self.schemaValue = None
        self.typeName = None

    def setSchema(self, schemaName, schemaValue):
        self.schemaName = schemaName
        self.schemaValue = schemaValue
        self.typeName = schemaValue[u'title']
        self.typeName = self.typeName.replace(u' ', u'')
        self.typeName = self.typeName.replace(u'.', u'')
        self.typeName = u'S' + self.typeName[:1].upper() + self.typeName[1:]
        if u'description' in schemaValue:
            self.description = schemaValue[u'description']

    def revise(self, c11Types):
        return (0, u'')

    def codeTypeName(self, withDeclare=False, asVariable=False):
        return self.typeName

    def codeDefineVariable(self, variableName, withDeclare=False, withDefault=False):
        pass

    def codeSetDefaultInConstructor(self):
        pass

    def codeHeader(self, codeTypeNames):
        return [u'']

    def codeSource(self, codeTypeNames):
        return [u'']

    def codeDefaultValue(self):
        return u''

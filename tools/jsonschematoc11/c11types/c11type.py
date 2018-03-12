'''basic type'''

class C11Type(object):
    '''basic type'''

    def __init__(self):
        self.schemaName = None
        self.schemaValue = None
        self.typeName = None

    def setCodes(self, manualCodeHeaders, manualCodeSourcesVariable, manualCodeSourcesFunction, manualCodeParsersFrom, manualCodeParsersTo):
        self.manualCodeHeaders = manualCodeHeaders
        self.manualCodeSourcesVariable = manualCodeSourcesVariable
        self.manualCodeSourcesFunction = manualCodeSourcesFunction
        self.manualCodeParsersFrom = manualCodeParsersFrom
        self.manualCodeParsersTo = manualCodeParsersTo

    def setSchema(self, schemaName, schemaValue):
        self.schemaName = schemaName
        self.schemaValue = schemaValue
        self.typeName = schemaValue[u'title']
        self.typeName = self.typeName.replace(u' ', u'')
        self.typeName = self.typeName.replace(u'.', u'')
        self.typeName = u'S' + self.typeName[:1].upper() + self.typeName[1:]
        if u'description' in schemaValue:
            self.description = schemaValue[u'description']

    @classmethod
    def revise(cls, c11Types):
        return (0, u'')

    def codeTypeName(self, withDeclare=False, asVariable=False):
        return self.typeName

    def codeDefineVariable(self, variableName, withDeclare=False, withDefault=False):
        pass

    def codeSetDefaultInConstructor(self):
        pass

    @classmethod
    def codeHeader(cls, codeTypeNames):
        return [u'']

    @classmethod
    def codeSource(cls, codeTypeNames):
        return [u'']

    @classmethod
    def codeDefaultValue(cls, schemaDefaultValue):
        return u''

    @classmethod
    def codeDefaultValueArray(cls, schemaDefaultValue):
        return u''

    @classmethod
    def codeJsonCheck(cls):
        return None

    @classmethod
    def codeJsonSet(cls, dataName, variableName):
        return None

    @classmethod
    def codeJsonGet(cls, dataName, variableName):
        return None

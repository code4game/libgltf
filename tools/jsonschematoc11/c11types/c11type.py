"""basic type."""

class C11Type(object):

    """basic type."""

    def __init__(self):
        """construct and declare some vars."""
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
        self.typeName = self.typeName.replace(u'/', u'')
        self.typeName = u'S' + self.typeName[:1].upper() + self.typeName[1:]
        if u'description' in schemaValue:
            self.description = schemaValue[u'description']

    @classmethod
    def revise(self, c11Types):
        return (0, u'')

    @classmethod
    def codeTypeName(self, withDeclare=False, asVariable=False, withDocument=False):
        return self.typeName

    @classmethod
    def codeDefineVariable(self, variableName, withDeclare=False, withDefault=False):
        pass

    @classmethod
    def codeSetDefaultInConstructor(self):
        pass

    @classmethod
    def codeHeader(self, codeTypeNames):
        return [u'']

    @classmethod
    def codeSource(self, codeTypeNames):
        return [u'']

    @classmethod
    def codeDefaultValue(self, schemaDefaultValue):
        return u''

    @classmethod
    def codeDefaultValueArray(self, schemaDefaultValue):
        return u''

    @classmethod
    def codeJsonCheck(self):
        return None

    @classmethod
    def codeJsonSet(self, dataName, variableName):
        return None

    @classmethod
    def codeJsonGet(self, dataName, variableName):
        return None

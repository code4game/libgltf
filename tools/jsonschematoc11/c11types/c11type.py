"""basic type"""

class C11Type(object):
    """basic type"""

    def __init__(self):
        """construct and declare some vars"""
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

    def revise(self, c11Types):
        return (0, u'')

    def codeTypeName(self, withDeclare=False, asVariable=False, withDocument=False):
        return self.typeName

    def codeDefineVariable(self, variableName, withDeclare=False, withDefault=False):
        pass

    def codeSetDefaultInConstructor(self):
        pass

    def codeHeader(self, codeTypeNames):
        return [u'']

    def codeSource(self, codeTypeNames):
        return [u'']

    def codeDefaultValue(self, schemaDefaultValue):
        return u''

    def codeDefaultValueArray(self, schemaDefaultValue):
        return u''

    def codeJsonCheck(self):
        return None

    def codeJsonSet(self, dataName, variableName):
        return None

    def codeJsonGet(self, dataName, variableName):
        return None

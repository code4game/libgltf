import abc

class C11Type(object):

    """basic type."""

    def __init__(self):
        """Construct and declare some vars."""
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

    @abc.abstractmethod
    def revise(self, c11Types):
        return (0, u'')

    @abc.abstractmethod
    def codeTypeName(self, withDeclare=False, asVariable=False, withDocument=False):
        return self.typeName

    @abc.abstractmethod
    def codeDefineVariable(self, variableName, withDeclare=False, withDefault=False):
        pass

    @abc.abstractmethod
    def codeSetDefaultInConstructor(self):
        pass

    @abc.abstractmethod
    def codeHeader(self, codeTypeNames):
        return [u'']

    @abc.abstractmethod
    def codeSource(self, codeTypeNames):
        return [u'']

    @abc.abstractmethod
    def codeDefaultValue(self, schemaDefaultValue):
        return u''

    @abc.abstractmethod
    def codeDefaultValueArray(self, schemaDefaultValue):
        return u''

    @abc.abstractmethod
    def codeJsonCheck(self):
        return None

    @abc.abstractmethod
    def codeJsonSet(self, dataName, variableName):
        return None

    @abc.abstractmethod
    def codeJsonGet(self, dataName, variableName):
        return None

from c11type import C11Type
from c11typenone import C11TypeNone
from c11typebool import C11TypeBool
from c11typeinteger import C11TypeInteger
from c11typenumber import C11TypeNumber
from c11typestring import C11TypeString
from c11typearray import C11TypeArray
from c11variable import C11Variable

class C11TypeStruct(C11Type):
    def __init__(self):
        C11Type.__init__(self)
        self.parents = dict()
        self.variables = dict()

    def setSchema(self, schemaName, schemaValue):
        C11Type.setSchema(self, schemaName, schemaValue)
        if u'allOf' in schemaValue:
            schemaValueAllOf = schemaValue[u'allOf']
            for schemaValueAllOfItem in schemaValueAllOf:
                if u'$ref' in schemaValueAllOfItem:
                    self.parents[schemaValueAllOfItem[u'$ref']] = None
        if u'properties' in schemaValue:
            schemaValueProperties = schemaValue[u'properties']
            for key in schemaValueProperties:
                self.variables[key] = C11Variable(key, schemaValueProperties[key])

    def haveParents(self):
        return len(self.parents) > 0

    def getParentTypeNames(self):
        parentTypeNames = []
        for key in self.parents:
            parent = self.parents[key]
            for parentParentTypeName in parent.getParentTypeNames():
                parentTypeNames.append(parentParentTypeName)
            parentTypeNames.append(parent.codeTypeName())
        return parentTypeNames

    def getVariableKeys(self, includeParent=False):
        variableKeys = []
        for key in self.variables:
            variableKeys.append(key)
        for key in self.parents:
            parent = self.parents[key]
            parentVariableKeys = parent.getVariableKeys(True)
            for parentVariableKey in parentVariableKeys:
                variableKeys.append(parentVariableKey)
        return variableKeys

    def getVariables(self, includeParent=False):
        variables = self.variables
        for parentKey in self.parents:
            parent = self.parents[parentKey]
            parentVariableKeys = parent.getVariableKeys(True)
            if len(parentVariableKeys) > 0:
                variables = {key: value for key, value in variables.items() if key not in parentVariableKeys }
            parentVariableKeys = None
        return variables.values()

    def revise(self, c11Types):
        for key in self.parents:
            if key not in c11Types:
                return (1, u'Can\'t find the parent %s in %s' % (key, self.codeTypeName()))
            self.parents[key] = c11Types[key]
        for key in self.variables:
            variable = self.variables[key]
            (errorCode, errorMessage) = variable.revise(c11Types)
            if errorCode != 0:
                return (errorCode, errorMessage)
        return (0, u'')

    def codeTypeName(self, withDeclare=False, asVariable=False):
        codeLine = self.typeName
        if self.typeName == None:
            print u'dd'
        if withDeclare:
            codeLine = u'struct ' + codeLine
        if asVariable:
            codeLine = u'std::shared_ptr<%s>' % codeLine
        return codeLine

    def codeInheritTypes(self):
        if len(self.parents) <= 0:
            return u''
        code = u''
        for parent in self.parents:
            code = code + u', %s' % self.parents[parent].codeTypeName()
        code = u' :' + code[1:]
        return code

    def codeHeader(self, codeTypeNames):
        codeLines = []
        if self.codeTypeName() in codeTypeNames:
            return codeLines
        for key in self.parents:
            parent = self.parents[key]
            if parent.codeTypeName() in codeTypeNames:
                continue
            for parentCodeLine in parent.codeHeader(codeTypeNames):
                codeLines.append(parentCodeLine)

        codeLines.append(u'/*!')
        codeLines.append(u' * struct: %s' % self.codeTypeName())
        if hasattr(self, u'description'):
            codeLines.append(u' * ' + self.description)
        codeLines.append(u' */')
        codeLines.append(u'struct %s%s' % (self.codeTypeName(), self.codeInheritTypes()))
        codeLines.append(u'{')
        codeLines.append(u'    %s();' % self.codeTypeName())
        codeLines.append(u'')
        for variable in self.getVariables():
            if variable.hasComment():
                codeLines.append(u'    // %s' % variable.codeComment())
            codeLines.append(u'    %s;' % variable.codeDeclare())
        codeLines.append(u'};')
        return codeLines

    def codeSource(self, codeTypeNames):
        codeLines = []
        codeLines.append(u'%s::%s()' % (self.codeTypeName(), self.codeTypeName()))
        codeLines.append(u'{')
        codeLines.append(u'    //')
        codeLines.append(u'}')
        return codeLines

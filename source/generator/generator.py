import os
import io
import json
import jsonschema

class CppType(object):
    def __init__(self):
        self.title = None
        self.type = None

    def codeTypeName(self, withDeclare=False):
        declare = u''
        if withDeclare and hasattr(self, u'type'):
            if self.type == u'object':
                declare = u'struct '
        structName = self.title[0:1].upper() + self.title[1:]
        structName = structName.replace(u' ', u'')
        structName = structName.replace(u'.', u'')
        return declare + u'S' + structName

    def codeParents(self):
        if hasattr(self, u'parents'):
            return self.parents
        return []

    def codeInheritParents(self):
        parents = self.codeParents()
        if len(parents) <= 0:
            return u''
        code = u' '
        for parent in parents:
            code = code + u', %s' % parent
        code = u' :' + code[2:]
        return code

    def codeDeclareAsParameter(self, parameter, withDeclare=False):
        return u'std::shared_ptr<%s> %s' % (self.codeTypeName(withDeclare), parameter[u'name'])

    def codeSetDefaultAsParameterInConstruct(self, parameter):
        pass

    def codeSetDefaultAsParameter(self, parameter):
        pass

    def codeDeclare(self):
        headerCode = u'struct %s%s\n' % (self.codeTypeName(), self.codeInheritParents())
        headerCode = headerCode + u'{\n'
        headerCode = headerCode + u'    %s();\n' % self.codeTypeName()
        headerCode = headerCode + u'\n'
        headerCode = headerCode + u'    bool IsValid() const;\n'
        headerCode = headerCode + u'\n'
        if hasattr(self, u'parameters'):
            for parameter in self.parameters:
                headerCode = headerCode + u'    %s;\n' % parameter[u'cppType'].codeDeclareAsParameter(parameter, True)
        headerCode = headerCode + u'};'
        return headerCode

    def codeImplement(self):
        return u''

class CppTypeDummy(CppType):
    def __init__(self):
        self.title = u'dummy'
        self.type = u'object'

def parseSchema(rootPath, filePath):
    with open(os.path.join(rootPath, filePath), u'r') as file_schema:
        schema = json.load(file_schema)
        cppType = CppType()
        cppType.title = schema[u'title']
        if u'type' in schema:
            cppType.type = schema[u'type']

        if u'description' in schema:
            cppType.description = schema[u'description']

        if u'allOf' in schema:
            cppType.parents = []
            for parent in schema[u'allOf']:
                cppType.parents.append(parent[u'$ref'])

        if u'properties' in schema:
            cppType.parameters = []
            properties = schema[u'properties']
            for key in properties:
                cppType.parameters.append({ u'name': key, u'value': properties[key], u'cppType': CppTypeDummy() })

        print cppType.codeDeclare()
        print cppType.codeImplement()

for rootPath, directionNames, fileNames in os.walk(u'./external/glTF/specification/2.0/schema/', topdown=True):
    for fileName in fileNames:
        parseSchema(rootPath, fileName)

print "test"

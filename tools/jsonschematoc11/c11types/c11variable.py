"""variable."""

from .c11typebool import C11TypeBool
from .c11typeinteger import C11TypeInteger
from .c11typenumber import C11TypeNumber
from .c11typestring import C11TypeString
from .c11typearray import C11TypeArray
from .c11typemap import C11TypeMap
from .c11typenone import C11TypeNone

class C11Variable(object):

    """variable."""

    def __init__(self, name, schemaValue):
        """construct and declare some vars."""
        self.schemaValue = schemaValue
        self.typeName = ""
        self.c11Type = C11TypeNone()
        self.name = name
        self.comment = None
        if u'description' in schemaValue:
            self.comment = schemaValue[u'description']

    def revise(self, c11Types):
        variableSchemaValue = self.schemaValue
        if u'$ref' in self.schemaValue:
            schemaValueType = self.schemaValue[u'$ref']
        elif u'type' in self.schemaValue:
            schemaValueType = self.schemaValue[u'type']
            if schemaValueType == u'object' and u'additionalProperties' in self.schemaValue:
                schemaValueType = u'map'
                variableSchemaValue = self.schemaValue[u'additionalProperties']
        elif u'allOf' in self.schemaValue and len(self.schemaValue[u'allOf']) > 0 and u'$ref' in self.schemaValue[u'allOf'][0]:
            schemaValueType = self.schemaValue[u'allOf'][0][u'$ref']
            #schemaValueType = u'array'
            #variableSchemaValue = self.schemaValue[u'allOf'][0]
        elif u'anyOf' in self.schemaValue and len(self.schemaValue[u'anyOf']) > 0:
            for schemaValueAnyOf in self.schemaValue[u'anyOf']:
                if u'type' not in schemaValueAnyOf:
                    continue
                schemaValueType = schemaValueAnyOf[u'type']
                break
        else:
            return (0, None)

        if schemaValueType == u'bool' or schemaValueType == u'boolean':
            self.c11Type = C11TypeBool()
            self.typeName = u'bool'
        elif schemaValueType == u'integer':
            self.c11Type = C11TypeInteger()
            self.typeName = u'integer'
        elif schemaValueType == u'number':
            self.c11Type = C11TypeNumber()
            self.typeName = u'number'
        elif schemaValueType == u'string':
            self.c11Type = C11TypeString()
            self.typeName = u'string'
        elif schemaValueType == u'array':
            self.c11Type = C11TypeArray()
            self.typeName = u'array'
            self.c11Type.setItemSchema(variableSchemaValue)
        elif schemaValueType == u'map':
            self.c11Type = C11TypeMap()
            self.typeName = u'map'
            self.c11Type.setItemSchema(variableSchemaValue)
        else:
            if schemaValueType not in c11Types:
                if u'additionalProperties' in self.schemaValue:
                    schemaValueAdditionalProperties = self.schemaValue[u'additionalProperties']
                    if u'$ref' in schemaValueAdditionalProperties:
                        schemaValueType = schemaValueAdditionalProperties[u'$ref']
            if schemaValueType in c11Types:
                self.c11Type = c11Types[schemaValueType]
                self.typeName = u'struct'
            else:
                self.c11Type = C11TypeNone()
        self.c11Type.revise(c11Types)
        return (0, None)

    def hasComment(self):
        return self.comment != None

    def codeComment(self):
        return self.comment

    def codeDeclare(self):
        return u'%s %s' % (self.c11Type.codeTypeName(withDeclare=True, asVariable=True), self.name)

    def codeConstructorDefault(self):
        schemaDefaultValue = None
        if u'default' in self.schemaValue:
            schemaDefaultValue = self.schemaValue[u'default']
        return u'%s(%s)' % (self.name, self.c11Type.codeDefaultValue(schemaDefaultValue))

    def codeParser(self, isSet=True):
        codeLines = []
        codeCheckLine = self.c11Type.codeJsonCheck()
        if isSet:
            if codeCheckLine is None or len(codeCheckLine) <= 0:
                codeLines.append(u'if (_JsonValue.HasMember(GLTFTEXT("%s")))' % self.name)
            else:
                codeLines.append(u'if (_JsonValue.HasMember(GLTFTEXT("%s")) && _JsonValue[GLTFTEXT("%s")].%s)' % (self.name, self.name, codeCheckLine))
        else:
            if self.typeName == u'array' or self.typeName == u'map':
                codeLines.append(u'if (!_rData.data.%s.empty())' % self.name)
            elif self.typeName == u'struct':
                codeLines.append(u'if (!!_rData.data.%s)' % self.name)
        codeLines.append(u'{')
        if isSet:
            codeSetLine = self.c11Type.codeJsonSet(u'_rData', self.name)
            if codeSetLine != None and len(codeSetLine) > 0:
                codeLines.append(u'    %s' % (codeSetLine))
        else:
            codeLines.append(u'    GLTFCharValue json_value;')
            if self.typeName == u'struct':
                codeLines.append(u'    if (!(%s(*_rData.data.%s, _rData.doc) >> json_value)) return false;' % (self.c11Type.codeTypeName(withDocument=True), self.name))
            else:
                codeLines.append(u'    if (!(TDataDoc<%s>(_rData.data.%s, _rData.doc) >> json_value)) return false;' % (self.c11Type.codeTypeName(asVariable=True), self.name))
            codeLines.append(u'    _JsonValue.AddMember(GLTFTEXT("%s"), json_value, _rData.doc->GetAllocator());' % self.name)
        codeLines.append(u'}')
        return codeLines

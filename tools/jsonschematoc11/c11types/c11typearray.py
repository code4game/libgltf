from .c11type import C11Type

class C11TypeArray(C11Type):
    def __init__(self):
        """construct and declare some vars."""
        C11Type.__init__(self)
        self.typeName = u'std::vector'
        self.c11Type = None

    def buildC11Type(self, schemaValue):
        c11Type = None
        schemaValueType = None
        variableSchemaValue = schemaValue
        if u'type' in schemaValue:
            schemaValueType = schemaValue[u'type']
            if schemaValueType == u'object':
                if u'additionalProperties' in schemaValue:
                    schemaValueType = u'map'
                    variableSchemaValue = schemaValue[u'additionalProperties']
                elif u'allOf' in schemaValue and len(schemaValue[u'allOf']) > 0 and u'$ref' in schemaValue[u'allOf'][0]:
                    schemaValueType = schemaValue[u'allOf'][0][u'$ref']
                    #print(schemaValueType)
                    #schemaValueType = u'array'
                    #doallof = True
        elif u'$ref' in schemaValue:
            schemaValueType = schemaValue[u'$ref']

        if schemaValueType == u'bool' or schemaValueType == u'boolean':
            from .c11typebool import C11TypeBool
            c11Type = C11TypeBool()
        elif schemaValueType == u'integer':
            from .c11typeinteger import C11TypeInteger
            c11Type = C11TypeInteger()
        elif schemaValueType == u'number':
            from .c11typenumber import C11TypeNumber
            c11Type = C11TypeNumber()
        elif schemaValueType == u'string':
            from .c11typestring import C11TypeString
            c11Type = C11TypeString()
        elif schemaValueType == u'array':
            c11Type = C11TypeArray()
        elif schemaValueType == u'map':
            from .c11typemap import C11TypeMap
            c11Type = C11TypeMap()
            c11Type.setItemSchema(variableSchemaValue)
        if c11Type is None:
            from .c11typestruct import C11TypeStruct
            c11Type = C11TypeStruct()
        return (c11Type, 0, None)

    def setItemSchema(self, schemaValue):
        self.typeName = u'std::vector'
        self.schemaValue = schemaValue
        if u'items' in schemaValue:
            (c11_type, error_code, error_message) = self.buildC11Type(schemaValue[u'items'])
        elif u'$ref' in schemaValue:
            (c11_type, error_code, error_message) = self.buildC11Type(schemaValue[u'$ref'])
        if error_code != 0:
            print(error_message)
        self.c11Type = c11_type

    def revise(self, c11Types):
        from .c11typestruct import C11TypeStruct
        from .c11typemap import C11TypeMap
        if not isinstance(self.c11Type, C11TypeStruct) and not isinstance(self.c11Type, C11TypeMap):
            return (0, None)
        schemaValueType = None
        schemaValueItem = None
        if u'items' in self.schemaValue:
            schemaValueItem = self.schemaValue[u'items']
        if u'$ref' in self.schemaValue:
            schemaValueItem = self.schemaValue
        if schemaValueItem is None:
            return (1, u'Can\'t find the items in schema of array')
        elif u'type' in schemaValueItem and schemaValueItem[u'type'] == 'object' and u'allOf' in schemaValueItem and len(schemaValueItem[u'allOf']) > 0 and u'$ref' in schemaValueItem[u'allOf'][0]:
            schemaValueType = schemaValueItem[u'allOf'][0][u'$ref']
        elif u'$ref' in schemaValueItem:
            schemaValueType = schemaValueItem[u'$ref']
        if schemaValueType not in c11Types:
            if u'additionalProperties' in schemaValueItem:
                schemaValueAdditionalProperties = schemaValueItem[u'additionalProperties']
                if u'$ref' in schemaValueAdditionalProperties:
                    schemaValueType = schemaValueAdditionalProperties[u'$ref']
        if isinstance(self.c11Type, C11TypeMap):
            self.c11Type.revise(c11Types)
        elif schemaValueType in c11Types:
            self.c11Type = c11Types[schemaValueType]
        else:
            from .c11typenone import C11TypeNone
            self.c11Type = C11TypeNone()
        return (0, u'')

    def codeTypeName(self, withDeclare=False, asVariable=False, withDocument=False):
        #if withDocument:
        #    return u'TDataDoc<%s<%s>>' % (self.typeName, self.c11Type.codeTypeName(withDeclare=withDeclare, asVariable=True))
        return u'%s<%s>' % (self.typeName, self.c11Type.codeTypeName(withDeclare=withDeclare, asVariable=asVariable, withDocument=withDocument))

    def codeDefaultValue(self, schemaDefaultValue):
        if schemaDefaultValue is None:
            return u''
        return self.c11Type.codeDefaultValueArray(schemaDefaultValue)

    def codeJsonCheck(self):
        return u'IsArray()'

    def codeJsonSet(self, dataName, variableName):
        return u'if (!(%s.%s << _JsonValue[GLTFTEXT("%s")])) return false;' % (dataName, variableName, variableName)

    def codeJsonGet(self, dataName, variableName):
        return u'if (!(%s.%s >> _JsonValue[GLTFTEXT("%s")])) return false;' % (dataName, variableName, variableName)

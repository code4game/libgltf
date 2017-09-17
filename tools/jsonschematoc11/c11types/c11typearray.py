from c11type import C11Type

class C11TypeArray(C11Type):
    def __init__(self):
        C11Type.__init__(self)
        self.typeName = u'std::vector'
        self.c11Type = None

    def buildC11Type(self, schemaValue):
        c11Type = None
        schemaValueType = None
        if u'type' in schemaValue:
            schemaValueType = schemaValue[u'type']
        elif u'$ref' in schemaValue:
            schemaValueType = schemaValue[u'$ref']
        if schemaValueType == u'bool' or schemaValueType == u'boolean':
            from c11typebool import C11TypeBool
            c11Type = C11TypeBool()
        elif schemaValueType == u'integer':
            from c11typeinteger import C11TypeInteger
            c11Type = C11TypeInteger()
        elif schemaValueType == u'number':
            from c11typenumber import C11TypeNumber
            c11Type = C11TypeNumber()
        elif schemaValueType == u'string':
            from c11typestring import C11TypeString
            c11Type = C11TypeString()
        elif schemaValueType == u'array':
            c11Type = C11TypeArray()
        if c11Type == None:
            from c11typestruct import C11TypeStruct
            c11Type = C11TypeStruct()
        return (c11Type, 0, None)

    def setItemSchema(self, schemaValue):
        self.typeName = u'std::vector'
        self.schemaValue = schemaValue
        if u'items' in schemaValue:
            (c11Type, errorCode, errorMessage) = self.buildC11Type(schemaValue[u'items'])
        elif u'$ref' in schemaValue:
            (c11Type, errorCode, errorMessage) = self.buildC11Type(schemaValue[u'$ref'])
        self.c11Type = c11Type

    def revise(self, c11Types):
        from c11typestruct import C11TypeStruct
        if not isinstance(self.c11Type, C11TypeStruct):
            return (0, None)
        schemaValueType = None
        schemaValueItem = None
        if u'items' in self.schemaValue:
            schemaValueItem = self.schemaValue[u'items']
        if u'$ref' in self.schemaValue:
            schemaValueItem = self.schemaValue
        if schemaValueItem == None:
            return (1, u'Can\'t find the items in schema of array')
        if u'$ref' in schemaValueItem:
            schemaValueType = schemaValueItem[u'$ref']
        if schemaValueType not in c11Types:
            if u'additionalProperties' in schemaValueItem:
                schemaValueAdditionalProperties = schemaValueItem[u'additionalProperties']
                if u'$ref' in schemaValueAdditionalProperties:
                    schemaValueType = schemaValueAdditionalProperties[u'$ref']
        if schemaValueType in c11Types:
            self.c11Type = c11Types[schemaValueType]
        else:
            from c11typenone import C11TypeNone
            self.c11Type = C11TypeNone()
        return (0, u'')

    def codeTypeName(self, withDeclare=False, asVariable=False):
        return u'%s<%s>' % (self.typeName, self.c11Type.codeTypeName(withDeclare=withDeclare, asVariable=asVariable))

    def codeDefaultValue(self, schemaDefaultValue):
        if schemaDefaultValue == None:
            return u''
        return self.c11Type.codeDefaultValueArray(schemaDefaultValue)

    def codeJsonCheck(self):
        return u'IsArray()'

    def codeJsonSet(self, dataName, variableName):
        return u'if (!(%s->%s << _JsonValue[L"%s"])) return false;' % (dataName, variableName, variableName)

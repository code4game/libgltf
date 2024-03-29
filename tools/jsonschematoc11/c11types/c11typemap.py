from .c11type import C11Type

class C11TypeMap(C11Type):

    """map type."""

    def __init__(self):
        """Construct and declare some vars."""
        C11Type.__init__(self)
        self.typeName = u'std::map'
        self.c11Type = None

    def buildC11Type(self, schemaValue):
        c11Type = None
        if u'type' in schemaValue:
            schemaValueType = schemaValue[u'type']
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
                from .c11typearray import C11TypeArray
                c11Type = C11TypeArray()
        if c11Type is None:
            from .c11typestruct import C11TypeStruct
            c11Type = C11TypeStruct()
        return (c11Type, 0, None)

    def setItemSchema(self, schemaValue):
        self.typeName = u'std::map'
        self.schemaValue = schemaValue
        if u'items' in schemaValue:
            (c11Type, _, _) = self.buildC11Type(schemaValue[u'items'])
        elif u'$ref' in schemaValue:
            (c11Type, _, _) = self.buildC11Type(schemaValue[u'$ref'])
        self.c11Type = c11Type

    def revise(self, c11Types):
        from .c11typestruct import C11TypeStruct
        if not isinstance(self.c11Type, C11TypeStruct):
            return (0, None)
        schemaValueType = None
        if u'$ref' in self.schemaValue:
            schemaValueType = self.schemaValue[u'$ref']
        if schemaValueType in c11Types:
            self.c11Type = c11Types[schemaValueType]
        else:
            from .c11typenone import C11TypeNone
            self.c11Type = C11TypeNone()
        self.c11Type.revise(c11Types)
        return (0, u'')

    def codeTypeName(self, withDeclare=False, asVariable=False, withDocument=False):
        #if withDocument:
        #    return u'TDataDoc<%s<std::string, %s>>' % (self.typeName, self.c11Type.codeTypeName(withDeclare=withDeclare, asVariable=True))
        return u'%s<std::string, %s>' % (self.typeName, self.c11Type.codeTypeName(withDeclare=withDeclare, asVariable=asVariable, withDocument=withDocument))

    def codeJsonCheck(self):
        return u'IsObject()'

    def codeJsonSet(self, dataName, variableName):
        return u'if (!(%s.%s << _JsonValue["%s"])) return false;' % (dataName, variableName, variableName)

    def codeJsonGet(self, dataName, variableName):
        return u'if (!(%s.%s >> _JsonValue["%s"])) return false;' % (dataName, variableName, variableName)

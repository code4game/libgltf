from c11typenone import C11TypeNone
from c11typebool import C11TypeBool
from c11typeinteger import C11TypeInteger
from c11typenumber import C11TypeNumber
from c11typestring import C11TypeString
from c11typearray import C11TypeArray
from c11typemap import C11TypeMap
from c11typestruct import C11TypeStruct

def BuildC11Type(schemaName, schemaValue, isSchema=False):
    if u'title' not in schemaValue:
        return (None, 1, u'can\'t get the attribute `title` in %s' % schemaName)
    c11Type = None
    if not isSchema and u'type' in schemaValue:
        schemaValueType = schemaValue[u'type']
        if schemaValueType == u'bool' or schemaValueType == u'boolean':
            c11Type = C11TypeBool()
        elif schemaValueType == u'integer':
            c11Type = C11TypeInteger()
        elif schemaValueType == u'number':
            c11Type = C11TypeNumber()
        elif schemaValueType == u'string':
            c11Type = C11TypeString()
        elif schemaValueType == u'array':
            c11Type = C11TypeArray()
        #elif schemaValueType == u'object':
        #    c11Type = C11TypeMap()
    if c11Type == None:
        c11Type = C11TypeStruct()
    c11Type.setSchema(schemaName, schemaValue)
    return (c11Type, 0, None)

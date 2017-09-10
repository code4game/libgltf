import sys
import os
import argparse
import logging
import json

from c11types import BuildC11Type
from c11types.c11typestruct import C11TypeStruct

logging.basicConfig(level=logging.DEBUG, format=u'%(asctime)s [%(name)s][%(levelname)s](%(filename)s:%(lineno)d) %(message)s')
logger = logging.getLogger(name=u'JSONSchemaToC11')

class C11TypeLibrary(object):
    def __init__(self):
        self.c11Types = dict()

    def addC11Type(self, schemaName, c11Type):
        if schemaName in self.c11Types:
            return (1, u'already have %s in c11 type library' % schemaName)
        self.c11Types[schemaName] = c11Type
        return (0, u'')

    def addSchema(self, rootPath, schemaFileName):
        schemaFilePath = os.path.join(rootPath, schemaFileName)
        with open(schemaFilePath, u'r') as schemaFile:
            schema = json.load(schemaFile)
            if schema == None:
                return (1, u'Can\'t parse the schema file %s' % schemaFilePath)
            (c11Type, errorCode, errorMessage) = BuildC11Type(schemaFileName, schema)
            if errorCode != 0:
                return (errorCode, u'Has error when build - %s' % errorMessage)
            (errorCode, errorMessage) = self.addC11Type(schemaFileName, c11Type)
            if errorCode != 0:
                return (errorCode, u'Has error when add - %s' % errorMessage)
        return (0, u'')

    def addSchemaDirectory(self, schemaDirectory):
        if not os.path.isdir(schemaDirectory):
            return (1, u'The schema directory (%s) is not a valid directory' % schemaDirectory)
        
        for rootPath, directionNames, fileNames in os.walk(schemaDirectory, topdown=False):
            for fileName in fileNames:
                if not fileName.endswith(u'schema.json'):
                    continue
                (errorCode, errorMessage) = self.addSchema(rootPath, fileName)
                if errorCode != 0:
                    return (errorCode, errorMessage)
        return (0, u'')

    def preprocess(self):
        for key in self.c11Types:
            (errorCode, errorMessage) = self.c11Types[key].revise(self.c11Types)
            if errorCode != 0:
                return (errorCode, errorMessage)
        return (0, u'')

    def generate(self, headerFileName, sourceFileName):
        with open(headerFileName, u'w') as headerFile:
            headerFile.write(u'#pragma once\n')
            headerFile.write(u'\n')
            headerFile.write(u'#include <vector>\n')
            headerFile.write(u'#include <string>\n')
            parentTypeNames = []
            for key in self.c11Types:
                c11Type = self.c11Types[key]
                if c11Type.codeTypeName() in parentTypeNames:
                    continue

                headerFile.write(u'\n')
                headerFile.writelines(c11Type.codeHeader(parentTypeNames))

                parentTypeNames.append(c11Type.codeTypeName())
                if isinstance(c11Type, C11TypeStruct):
                    for c11TypeParentTypeName in c11Type.getParentTypeNames():
                        parentTypeNames.append(c11TypeParentTypeName)
        with open(sourceFileName, u'w') as sourceFile:
            sourceFile.write(u'#include "%s"\n' % headerFileName)
            sourceFile.write(u'\n')
            parentTypeNames = []
            for key in self.c11Types:
                c11Type = self.c11Types[key]
                if c11Type.codeTypeName() in parentTypeNames:
                    continue

                sourceFile.write(u'\n')
                sourceFile.writelines(c11Type.codeSource(parentTypeNames))

                parentTypeNames.append(c11Type.codeTypeName())
                if isinstance(c11Type, C11TypeStruct):
                    for c11TypeParentTypeName in c11Type.getParentTypeNames():
                        parentTypeNames.append(c11TypeParentTypeName)
        return (0, u'')

def JSONSchemaToC11(argv):
    parser = argparse.ArgumentParser(description=u'Generate c11 code by json schema.')
    parser.add_argument(u'schemaDirectory', metavar=u'schema_directory', type=type(u''), help=u'The directory contains all schema files')
    parser.add_argument(u'codeFileName', metavar=u'code_file_name', type=type(u''), help=u'The output filename of c11 code')
    args = parser.parse_args(argv)

    c11TypeLibrary = C11TypeLibrary()
    (errorCode, errorMessage) = c11TypeLibrary.addSchemaDirectory(args.schemaDirectory)
    if errorCode != 0:
        return (errorCode, errorMessage)
    (errorCode, errorMessage) = c11TypeLibrary.preprocess()
    if errorCode != 0:
        return (errorCode, errorMessage)
    (errorCode, errorMessage) = c11TypeLibrary.generate(u'%s.h' % args.codeFileName, u'%s.cpp' % args.codeFileName)
    if errorCode != 0:
        return (errorCode, errorMessage)
    return (0, u'')

if __name__ == u'__main__':
    (errorCode, errorMessage) = JSONSchemaToC11(sys.argv[1:])
    if errorCode != 0:
        logger.error(errorMessage)
    else:
        logger.info(u'Success')
    exit(errorCode)

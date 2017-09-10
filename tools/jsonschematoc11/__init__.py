import sys
from jsonschematoc11 import JSONSchemaToC11

if __name__ == u'__main__':
    (errorCode, errorMessage) = JSONSchemaToC11(sys.argv[1:])
    exit(errorCode)

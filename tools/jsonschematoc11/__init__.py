import sys
from .jsonschematoc11 import JSONSchemaToC11

if __name__ == u'__main__':
    (error_code, error_message) = JSONSchemaToC11(sys.argv[1:])
    if error_code != 0:
        print(u'Failed: ', error_message)
        sys.exit(error_code)
    else:
        print(u'Success')
    sys.exit(error_code)

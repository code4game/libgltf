#include "runtest.h"

#include <fstream>
#include <sstream>
#include <iosfwd>
#include <string>

#if defined(PLATFORM_WINDOWS)
#include <crtdbg.h>
#endif

int main(int _iArgc, char* _pcArgv[])
{
#if defined(PLATFORM_WINDOWS) && defined(_DEBUG)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    int error_code = 1;

    std::string input_file_path;
    for (int i = 1; i < _iArgc; ++i)
    {
        std::stringstream argument;
        argument << _pcArgv[i];
        if (argument.str() == "--coveralls")
        {
            error_code = 0;
        }
        else
        {
            input_file_path = argument.str();
        }
    }

    if (input_file_path.length() <= 0)
    {
        printf("Command line format: runtest [--test] input_file_path\n");
        return error_code;
    }

    std::wstringstream input_content;
    {
        std::wifstream input_file(input_file_path.c_str(), std::ios::in | std::ios::binary);
        if (!input_file.is_open())
        {
            printf("Can't open the file %s\n", input_file_path.c_str());
            return error_code;
        }

        input_content << input_file.rdbuf();

        input_file.close();
    }

    std::shared_ptr<libgltf::SGlTF> gltf_data;
    if (gltf_data << input_content.str())
    {
        printf("Success\n");
    }
    else
    {
        printf("Failed\n");
        return error_code;
    }

    return 0;
}

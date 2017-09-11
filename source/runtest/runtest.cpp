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

    if (_iArgc <= 1) return -1;

    std::wstringstream input_content;
    {
        std::wifstream input_file(_pcArgv[1], std::ios::in | std::ios::binary);
        if (!input_file.is_open())
        {
            return -1;
        }

        input_content << input_file.rdbuf();

        input_file.close();
    }

    std::shared_ptr<libgltf::SGlTF> gltf_data;
    if (libgltf::ParseByString(input_content.str(), gltf_data))
    {
        wprintf(L"Success\n");
    }
    else
    {
        wprintf(L"Failed\n");
    }

    return 0;
}

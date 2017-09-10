#include "runtest.h"

int32_t main(int32_t _iArgc, uint8_t* _pcArgv[])
{
    std::shared_ptr<libgltf::SGlTF> gltf_data;
    libgltf::ParseByString(L"", gltf_data);
    //
    return 0;
}

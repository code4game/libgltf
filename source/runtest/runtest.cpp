#include "runtest.h"

int main(int _iArgc, char** _pcArg)
{
    std::shared_ptr<libgltf::SGLTF> gltf_data;
    libgltf::ParseByString(L"", gltf_data);
    //
    return 0;
}

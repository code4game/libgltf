#pragma once

#include "libgltfpch.h"
#include "libgltf.h"

namespace libgltf
{
    bool operator<<(std::shared_ptr<SGlTF>& _pGlTF, const WCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SAsset>& _pAsset, const WCharValue& _JsonValue);
    bool operator<<(std::shared_ptr<SExtras>& _pExtras, const WCharValue& _JsonValue);
}

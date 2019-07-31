# Usage

Generate the `makefile` or `ninja` or `visual c++ project` or `xcode project` by [CMake].

For now, just build to a static library - `libgltf.(lib/a)`.

## How to use

### Load the glTF file

You can load the glTF file by the function - `libgltf::IglTFLoader::Create`, like this:

```cpp
std::shared_ptr<libgltf::IglTFLoader> gltf_loader = libgltf::IglTFLoader::Create("Monster.gltf");
std::shared_ptr<libgltf::SGlTF> loaded_gltf = gltf_loader->glTF().lock();
if (!loaded_gltf)
{
    // the glTF file is valid
    return false;
}
```

### Load the mesh data

And get the mesh data, like this:

```cpp
// get all indices of the triangle
libgltf::TDimensionVector<1, size_t> triangle_data;
std::shared_ptr<libgltf::TAccessorStream<libgltf::TDimensionVector<1, size_t> > > triangle_stream = std::make_shared<libgltf::TAccessorStream<libgltf::TDimensionVector<1, size_t> > >(triangle_data);
gltf_loader->GetOrLoadMeshPrimitiveIndicesData(0, 0, triangle_stream);

// get all points of the triangle
libgltf::TDimensionVector<3, float> position_data;
std::shared_ptr<libgltf::TAccessorStream<libgltf::TDimensionVector<3, float> > > position_stream = std::make_shared<libgltf::TAccessorStream<libgltf::TDimensionVector<3, float> > >(position_data);
gltf_loader->GetOrLoadMeshPrimitiveAttributeData(0, 0, L"position", position_stream);
```

### Load the image data

You can get the image (data and type) by `libgltf::IglTFLoader::GetOrLoadImageData`, like this:

```cpp
std::vector<uint8_t> image0_data;
libgltf::string_t image0_data_type;
gltf_loader->GetOrLoadImageData(0, image0_data, image0_data_type);
```

## Advance

### Regenerate new code by the glTF schema

> You can update the c++11 source code by `jsonschematoc11`.

Generate the c++11 code:

1. Run `tools/batch/update_parser_by_scheme.bat` (Windows) or `tools/batch/update_parser_by_scheme.sh` (Unix/Linux/MacOS)
2. Build your version by [CMake], [Ninja] or [VisualStudio].

### Character encoding

* default using UTF8, char and std::string
* set LIBGLTF_CHARACTOR_ENCODING in cmake command - UTF8, UTF16, UTF32 or UNICODE

### Supports Google's draco

You can update the Google's draco submodule in external/draco or pull the draco repo by yourself.

Set the `GOOGLE_DRACO_PATH_INCLUDE`, `GOOGLE_DRACO_PATH_BUILD`, `GOOGLE_DRACO_LIBRARY_DRACODEC_DEBUG`, `GOOGLE_DRACO_LIBRARY_DRACODEC_RELEASE`, `GOOGLE_DRACO_LIBRARY_DRACOENC_DEBUG` and `GOOGLE_DRACO_LIBRARY_DRACOENC_RELEASE`.

Check the `LIBGLTF_USE_GOOGLE_DRACO` or set `LIBGLTF_USE_GOOGLE_DRACO` is `TRUE`.

[CMake]: https://cmake.org
[Ninja]: https://ninja-build.org
[VisualStudio]: https://visualstudio.microsoft.com

# libgltf

[![glTF status](https://img.shields.io/badge/glTF-2%2E0-green.svg?style=flat)](https://github.com/KhronosGroup/glTF)
[![libgltf document](https://readthedocs.org/projects/libgltf/badge/?version=latest)](http://libgltf.rtfd.io/)
[![visit milestones](https://img.shields.io/badge/visit-milestones-blue.svg?style=flat)](https://github.com/code4game/libgltf/milestones)

[![build](https://github.com/code4game/libgltf/workflows/build/badge.svg)](https://github.com/code4game/libgltf/actions?query=workflow%3Abuild)
[![Coverage status from coveralls](https://coveralls.io/repos/github/code4game/libgltf/badge.svg?branch=master)](https://coveralls.io/github/code4game/libgltf?branch=master)
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/5480b79467244c528a22defcf1fd9c4f)](https://www.codacy.com/gh/code4game/libgltf/dashboard?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=code4game/libgltf&amp;utm_campaign=Badge_Grade)
[![Language grade: Python](https://img.shields.io/lgtm/grade/python/g/code4game/libgltf.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/code4game/libgltf/context:python)

[![FOSSA Status](https://app.fossa.com/api/projects/git%2Bgithub.com%2Fcode4game%2Flibgltf.svg?type=shield)](https://app.fossa.com/projects/git%2Bgithub.com%2Fcode4game%2Flibgltf?ref=badge_shield)
[![CII best practices](https://bestpractices.coreinfrastructure.org/projects/1434/badge)](https://bestpractices.coreinfrastructure.org/projects/1434)
[![support](https://img.shields.io/badge/support-buy%20a%20cup%20of%20coffee-orange.svg?style=flat)](https://c4gio.itch.io/libgltf-ue4)

The project includes a generator that can generate codes by glTF 2.0 JSON schema.

It is used in [glTFForUE4](https://github.com/code4game/glTFForUE4).

## Features

* [glTF 2.0]
* Load the gltf/embedded/glb file
* This is a static library
* Cross platform
* C++17
* Supports the Unicode and UTF8
* Supports some extensions
  * `KHR_draco_mesh_compression` - [Google's Draco]
  * `KHR_lights_punctual`
  * `KHR_materials_clearcoat`
  * `KHR_materials_ior`
  * `KHR_materials_sheen`
  * `KHR_materials_specular`
  * `KHR_materials_transmission`
  * `KHR_materials_unlit`
  * `KHR_materials_variants`
  * `KHR_materials_volume`
  * and more
* Platforms
  * Windows
    * Win32 (win32)
    * x64 (win64)
  * Linux (linux)
  * macOS (macos)
  * Android
    * armeabi-v7a
    * armeabi-v7a-with-neon
    * arm64-v8a
    * x86
    * x86_64
  * iOS
    * iOS (iphoneos)
    * watchOS (watchos)
    * simulator

## Getting Started

1. Update the submodule
    > Run `git submodule update --init`
2. Generate the project by [CMake]
    > Run `cmake -G "[GENERATOR BY YOUR SYSTEM]" [LIBGLTF FOLDER]`
3. Build the project and generate the static library `libgltf.lib` or `libgltf.a`
4. Include `libgltf/libgltf.h` in your project.
5. Link the static library `libgltf.lib` or `libgltf.a` in your project.
    > You have to link the static library `draco.lib` or `draco.a` with your project, if you want to support the [Google's Draco].
    > And you can find the draco in the external folder.

Code example:

```cpp
std::shared_ptr<libgltf::IglTFLoader> gltf_loader = libgltf::IglTFLoader::Create(/*a function to load the file by std::istream*/);
std::shared_ptr<libgltf::SGlTF> loaded_gltf = gltf_loader->glTF().lock();
if (!loaded_gltf)
{
    printf("failed to load your gltf file");
}
```

## License

This software is released under the MIT license.

[glTF 2.0]: https://www.khronos.org/gltf/
[Google's Draco]: https://github.com/google/draco
[CMake]: https://cmake.org/

[![FOSSA Status](https://app.fossa.com/api/projects/git%2Bgithub.com%2Fcode4game%2Flibgltf.svg?type=large)](https://app.fossa.com/projects/git%2Bgithub.com%2Fcode4game%2Flibgltf?ref=badge_large)

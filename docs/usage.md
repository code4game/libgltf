# Usage

Generate the `makefile` or `ninja` or `visual c++ project` or `xcode project` by [CMake].

For now, just build to a static library - `libgltf.(lib/a)`.

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

## Advance

### Generate new code by the official glTF schema

> You can update the c++11 source code by `jsonschematoc11`.

Generate the c++11 code:

1. Run `tools/batch/update_parser_by_scheme.bat` (Windows) or `tools/batch/update_parser_by_scheme.sh` (Unix/Linux/MacOS)
2. Build your version by [CMake], [Ninja] or [VisualStudio].

### String encode

* default using utf8, char and std::string
* set LIBGLTF_WITH_UNICODE as TRUE in cmake command, if you want using unicode
  * default using wchar_t and std::wstring
  * set LIBGLTF_USING_CHAR16 as TRUE in cmake command, if you wnat using utf16, char16_t and std::u16string
  * set LIBGLTF_USING_CHAR32 as TRUE in cmake command, if you want using utf32, char32_t and std::u32string

[CMake]: https://cmake.org
[Ninja]: https://ninja-build.org
[VisualStudio]: https://visualstudio.microsoft.com

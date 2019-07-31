#if defined(LIBGLTF_CHARACTOR_ENCODING_IS_UTF16)
#   define      GLTFTEXT(t)             u##t
    typedef std::u16string              string_t;
#elif defined(LIBGLTF_CHARACTOR_ENCODING_IS_UTF32)
#   define      GLTFTEXT(t)             U##t
    typedef std::u32string              string_t;
#elif defined(LIBGLTF_CHARACTOR_ENCODING_IS_UNICODE)
#   define      GLTFTEXT(t)             L##t
    typedef std::wstring                string_t;
#else
#   define      GLTFTEXT(t)             t
    typedef std::string                 string_t;
#endif

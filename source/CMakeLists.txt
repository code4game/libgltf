add_subdirectory(libgltf)

if (${LIBGLTF_PLATFORM_WINDOWS} OR ${LIBGLTF_PLATFORM_LINUX} OR ${LIBGLTF_PLATFORM_MACOS})
    add_subdirectory(runtest)
endif()

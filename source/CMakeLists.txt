add_subdirectory(libgltf)

if (${LIBGLTF_BUILD_RUNTEST})
    add_subdirectory(runtest)
endif()

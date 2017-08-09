add_subdirectory(libgltf)
add_subdirectory(runtest)

if(${LIBGLTF_BUILD_TOOLS})
    add_subdirectory(structgenerator)
endif()

set(SOURCE_FILE_PATH ${SOURCE_PATH}/runtest)

set(INCLUDE_PATH_LIST
    ${HEADER_PATH}
    ${SOURCE_FILE_PATH}
    )
include_directories(${INCLUDE_PATH_LIST})

set(LINK_PATH_LIST
    ${OUT_BIN_PATH}
    ${OUT_LIB_PATH}
    )
link_directories(${LINK_PATH_LIST})

set(HEADER_FILE_LIST
    ${SOURCE_FILE_PATH}/runtest.h
    )

set(SOURCE_FILE_LIST
    ${SOURCE_FILE_PATH}/runtest.cpp
    )

source_group("header" FILES ${HEADER_FILE_LIST})
source_group("source" FILES ${SOURCE_FILE_LIST})

if(${LIBGLTF_PLATFORM_WINDOWS})
    set(LIBRARY_PATH_LIST
        debug libgltf${CMAKE_DEBUG_POSTFIX}${STATIC_LIBRARY_EXTENSION}
        optimized libgltf${STATIC_LIBRARY_EXTENSION}
        )
elseif(${LIBGLTF_PLATFORM_LINUX} OR ${LIBGLTF_PLATFORM_MACOS})
    set(LIBRARY_PATH_LIST
        debug gltf${CMAKE_DEBUG_POSTFIX}${STATIC_LIBRARY_EXTENSION}
        optimized gltf${STATIC_LIBRARY_EXTENSION}
        )
endif()
link_libraries(${LIBRARY_PATH_LIST})

set(EXECUTABLE_OUTPUT_PATH ${INTERNAL_BIN_PATH})

add_executable(runtest
    ${HEADER_FILE_LIST}
    ${SOURCE_FILE_LIST}
    )

set(DEPENDANCY_LIST
    libgltf
    )
add_dependencies(runtest ${DEPENDANCY_LIST})

set_target_properties(runtest PROPERTIES FOLDER main)

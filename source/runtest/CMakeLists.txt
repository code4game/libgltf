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

add_executable(runtest
    ${HEADER_FILE_LIST}
    ${SOURCE_FILE_LIST}
    )

set_target_properties(runtest PROPERTIES FOLDER main)
set_target_properties(runtest PROPERTIES EXECUTABLE_OUTPUT_PATH ${OUT_BIN_PATH})
set_target_properties(runtest PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${OUT_BIN_PATH})
set_target_properties(runtest PROPERTIES LIBRARY_OUTPUT_DIRECTORY_RELEASE ${OUT_LIB_PATH})

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
target_link_libraries(runtest ${LIBRARY_PATH_LIST})

if(LIBGLTF_USE_GOOGLE_DRACO)
    if(LIBGLTF_USE_GOOGLE_DRACO_SUBMODULE)
        set(LIBRARY_PATH_LIST_GOOGLE_DRACO
            dracodec
            )
    else()
        set(LIBRARY_PATH_LIST_GOOGLE_DRACO
            debug ${GOOGLE_DRACO_LIBRARY_DRACODEC_DEBUG}
            optimized ${GOOGLE_DRACO_LIBRARY_DRACODEC_RELEASE}
            debug ${GOOGLE_DRACO_LIBRARY_DRACOENC_DEBUG}
            optimized ${GOOGLE_DRACO_LIBRARY_DRACOENC_RELEASE}
            )
    endif()
    target_link_libraries(runtest ${LIBRARY_PATH_LIST_GOOGLE_DRACO})
endif()

set(DEPENDANCY_LIST
    libgltf
    )
add_dependencies(runtest ${DEPENDANCY_LIST})

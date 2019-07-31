cmake_minimum_required(VERSION 2.8.11)

project(libgltf)

set(SOURCE_FILE_PATH ${SOURCE_PATH}/libgltf)

set(INCLUDE_PATH_LIST
    ${HEADER_PATH}/libgltf
    ${SOURCE_FILE_PATH}
    ${EXTERNAL_PATH}/rapidjson/include
    )
include_directories(${INCLUDE_PATH_LIST})

if(${GOOGLEDRACO_FOUND} AND ${LIBGLTF_USE_GOOGLE_DRACO})
    include_directories(${GOOGLE_DRACO_PATH_INCLUDE} ${GOOGLE_DRACO_PATH_BUILD})
    add_definitions(-DLIBGLTF_USE_GOOGLE_DRACO)
endif()

set(CODE_FILE_LIST)

set(HEADER_FILE_LIST
    ${HEADER_PATH}/libgltf/libgltf.h
    )
set(CODE_FILE_LIST
    ${CODE_FILE_LIST}
    ${HEADER_FILE_LIST}
    )
source_group("header" FILES ${HEADER_FILE_LIST})

set(SOURCE_FILE_LIST
    ${SOURCE_FILE_PATH}/libgltfpch.h
    ${SOURCE_FILE_PATH}/libgltf.cpp
    ${SOURCE_FILE_PATH}/libgltfparser.h
    ${SOURCE_FILE_PATH}/libgltfparser.cpp
    ${SOURCE_FILE_PATH}/libgltfloader.cpp
    ${SOURCE_FILE_PATH}/common.h
    ${SOURCE_FILE_PATH}/common.cpp
    ${SOURCE_FILE_PATH}/utility.h
    ${SOURCE_FILE_PATH}/utility.cpp
    ${SOURCE_FILE_PATH}/gltf_loader.h
    ${SOURCE_FILE_PATH}/gltf_loader.cpp
    ${SOURCE_FILE_PATH}/file_loader.h
    ${SOURCE_FILE_PATH}/file_loader.cpp
    )
set(CODE_FILE_LIST
    ${CODE_FILE_LIST}
    ${SOURCE_FILE_LIST}
    )
source_group("source" FILES ${SOURCE_FILE_LIST})

if(${GOOGLEDRACO_FOUND} AND ${LIBGLTF_USE_GOOGLE_DRACO})
set(SOURCE_FILE_LIST_DRACO
    ${SOURCE_FILE_PATH}/extensions/google_draco.h
    ${SOURCE_FILE_PATH}/extensions/google_draco.cpp
    )
set(CODE_FILE_LIST
    ${CODE_FILE_LIST}
    ${SOURCE_FILE_LIST_DRACO}
    )
source_group("source\\extensions" FILES ${SOURCE_FILE_LIST_DRACO})
endif()

if(${LIBGLTF_PLATFORM_ANDROID})
    set(ANDROID_ABI_PATH ${ANDROID_ABI})
    if(${ANDROID_ARM_NEON})
        set(ANDROID_ABI_PATH "${ANDROID_ABI}-with-neon")
    endif()
    string(TOLOWER ${ANDROID_ABI_PATH} ANDROID_ABI_PATH)
    set(OUT_BIN_PATH "${OUT_BIN_PATH}/${ANDROID_ABI_PATH}")
    set(OUT_LIB_PATH "${OUT_LIB_PATH}/${ANDROID_ABI_PATH}")
elseif(${LIBGLTF_PLATFORM_IOS})
    set(IOS_PLATFORM_PATH ${IOS_PLATFORM})
    string(TOLOWER ${IOS_PLATFORM_PATH} IOS_PLATFORM_PATH)
    set(OUT_BIN_PATH "${OUT_BIN_PATH}/${IOS_PLATFORM_PATH}")
    set(OUT_LIB_PATH "${OUT_LIB_PATH}/${IOS_PLATFORM_PATH}")
endif()

set(RUNTIME_OUTPUT_PATH ${OUT_BIN_PATH})
set(LIBRARY_OUTPUT_PATH ${OUT_LIB_PATH})

add_library(libgltf STATIC ${CODE_FILE_LIST})

if(${LIBGLTF_PLATFORM_WINDOWS})
    add_custom_command(TARGET libgltf
        POST_BUILD
        COMMAND xcopy ARGS "/D /S /Y \"${ROOT_PATH}/include\" \"${OUT_PATH}/include\\\""
        COMMENT "sync the head file"
        )
else()
    add_custom_command(TARGET libgltf
        POST_BUILD
        COMMAND rsync -a -delete ${ROOT_PATH}/include ${OUT_PATH}/
        COMMENT "sync the head file"
        )
endif()

set_target_properties(libgltf PROPERTIES FOLDER main)
set_target_properties(libgltf PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${OUT_BIN_PATH})
set_target_properties(libgltf PROPERTIES LIBRARY_OUTPUT_DIRECTORY_RELEASE ${OUT_LIB_PATH})

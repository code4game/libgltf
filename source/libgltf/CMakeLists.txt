set(SOURCE_FILE_PATH ${SOURCE_PATH}/libgltf)

set(INCLUDE_PATH_LIST
    ${HEADER_PATH}/libgltf
    ${SOURCE_FILE_PATH}
    ${EXTERNAL_PATH}/rapidjson/include
    )
include_directories(${INCLUDE_PATH_LIST})

set(LINK_PATH_LIST
    ${OUT_BIN_PATH}
    ${OUT_LIB_PATH}
    )
link_directories(${LINK_PATH_LIST})

set(HEADER_FILE_LIST
    ${HEADER_PATH}/libgltf/libgltf.h
    )

set(SOURCE_FILE_LIST
    ${SOURCE_FILE_PATH}/libgltf.cpp
    ${SOURCE_FILE_PATH}/libgltfparser.cpp
    )

source_group("header" FILES ${HEADER_FILE_LIST})
source_group("source" FILES ${SOURCE_FILE_LIST})

set(LIBRARY_OUTPUT_PATH ${OUT_BIN_PATH})

add_library(libgltf STATIC
    ${HEADER_FILE_LIST}
    ${SOURCE_FILE_LIST}
    )

if(MSVC)
    add_custom_command(TARGET libgltf
        POST_BUILD
        COMMAND xcopy ARGS "/D /S /Y \"${ROOT_PATH}/include\" \"${OUT_PATH}/include\\\""
        COMMENT "sync the head file")
elseif(UNIX)
    add_custom_command(TARGET libgltf
        POST_BUILD
        COMMAND rsync -a -delete ${ROOT_PATH}/include ${OUT_PATH}/
        COMMENT "sync the head file")
endif()

set_target_properties(libgltf PROPERTIES FOLDER main)
set_target_properties(libgltf PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${OUT_BIN_PATH})
set_target_properties(libgltf PROPERTIES LIBRARY_OUTPUT_DIRECTORY_RELEASE ${OUT_BIN_PATH})

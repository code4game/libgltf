set(SOURCE_FILE_PATH ${SOURCE_PATH}/structgenerator)

set(INCLUDE_PATH_LIST
    ${HEADER_PATH}
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
    ${SOURCE_FILE_PATH}/structgenerator.h
    )

set(SOURCE_FILE_LIST
    ${SOURCE_FILE_PATH}/structgenerator.cpp
    )

source_group("header" FILES ${HEADER_FILE_LIST})
source_group("source" FILES ${SOURCE_FILE_LIST})

set(EXECUTABLE_OUTPUT_PATH ${INTERNAL_BIN_PATH})

add_executable(structgenerator
    ${HEADER_FILE_LIST}
    ${SOURCE_FILE_LIST}
    )

set(DEPENDANCY_LIST
    libgltf
    )
add_dependencies(structgenerator ${DEPENDANCY_LIST})

set_target_properties(structgenerator PROPERTIES FOLDER main)

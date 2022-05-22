project(runtest)

set(SOURCE_FILE_PATH ${SOURCE_PATH}/runtest)

set(INCLUDE_PATH_LIST
    ${HEADER_PATH}
    ${SOURCE_FILE_PATH}
    ${OUT_PATH}/include
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

set_target_properties(runtest PROPERTIES
    FOLDER main
    EXECUTABLE_OUTPUT_PATH ${OUT_BIN_PATH}
    RUNTIME_OUTPUT_DIRECTORY ${OUT_BIN_PATH}
    LIBRARY_OUTPUT_DIRECTORY ${OUT_LIB_PATH}
    )

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        libgltf
    )

if(LIBGLTF_USE_GOOGLE_DRACO)
    target_link_libraries(${PROJECT_NAME}
        PRIVATE
            draco
            draco_core
        )
endif()

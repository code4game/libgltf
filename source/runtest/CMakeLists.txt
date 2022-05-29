project(runtest)

set(SOURCE_FILE_PATH ${SOURCE_PATH}/runtest)

set(HEADER_FILE_LIST
    ${SOURCE_FILE_PATH}/runtest.h
    )
source_group("src" FILES ${HEADER_FILE_LIST})

set(SOURCE_FILE_LIST
    ${SOURCE_FILE_PATH}/runtest.cpp
    )
source_group("src" FILES ${SOURCE_FILE_LIST})

add_executable(runtest
    ${HEADER_FILE_LIST}
    ${SOURCE_FILE_LIST}
    )

set(INCLUDE_PATH_LIST
    ${HEADER_PATH}
    ${SOURCE_FILE_PATH}
    ${CMAKE_BINARY_DIR}/include
    )
target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${INCLUDE_PATH_LIST}
    )

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        libgltf
    )

set_target_properties(runtest
    PROPERTIES
        FOLDER main
    )

install(
    TARGETS ${PROJECT_NAME}
    EXPORT  ${PROJECT_NAME}-targets
    RUNTIME DESTINATION bin
    )

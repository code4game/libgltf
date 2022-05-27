project(libgltf)

add_library(${PROJECT_NAME})

set(SOURCE_FILE_PATH ${SOURCE_PATH}/libgltf)

configure_file(libgltf.h.cmakein ${OUT_PATH}/include/libgltf/libgltf.h @ONLY)
set(HEADER_FILE_LIST
    ${OUT_PATH}/include/libgltf/libgltf.h
    )
source_group("header" FILES ${HEADER_FILE_LIST})
target_sources(${PROJECT_NAME}
    PUBLIC
        ${HEADER_FILE_LIST}
    )

set(SOURCE_FILE_LIST
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
    )
source_group("source" FILES ${SOURCE_FILE_LIST})
target_sources(${PROJECT_NAME}
    PRIVATE
        ${SOURCE_FILE_LIST}
    )

if(LIBGLTF_WITH_GOOGLE_DRACO)
    set(SOURCE_FILE_LIST_DRACO
        ${SOURCE_FILE_PATH}/extensions/google_draco.h
        ${SOURCE_FILE_PATH}/extensions/google_draco.cpp
        )
    source_group("source\\extensions" FILES ${SOURCE_FILE_LIST_DRACO})
    target_sources(${PROJECT_NAME}
        PRIVATE
            ${SOURCE_FILE_LIST_DRACO}
        )
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

target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<INSTALL_INTERFACE:include>${OUT_PATH}/include
    PRIVATE
        ${HEADER_PATH}
        ${SOURCE_FILE_PATH}
        ${EXTERNAL_PATH}/rapidjson/include
    )

if(LIBGLTF_WITH_GOOGLE_DRACO)
    target_include_directories(${PROJECT_NAME}
        PRIVATE
            ${GOOGLE_DRACO_PATH_INCLUDE}
            ${GOOGLE_DRACO_PATH_BUILD}
        )
    target_compile_definitions(${PROJECT_NAME}
        PRIVATE
            LIBGLTF_WITH_GOOGLE_DRACO
        )
    target_link_libraries(${PROJECT_NAME}
        PUBLIC
            draco
            draco_core
        )
endif()

set_target_properties(${PROJECT_NAME}
    PROPERTIES
        FOLDER main
        ARCHIVE_OUTPUT_DIRECTORY ${OUT_LIB_PATH}
        LIBRARY_OUTPUT_DIRECTORY ${OUT_LIB_PATH}
        RUNTIME_OUTPUT_DIRECTORY ${OUT_BIN_PATH}
    )

install(
    TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}-targets
    ARCHIVE       DESTINATION lib
    LIBRARY       DESTINATION lib
    RUNTIME       DESTINATION bin
    )

install(FILES ${HEADER_FILE_LIST} DESTINATION include/${PROJECT_NAME})

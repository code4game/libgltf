project(libgltf)

set(LIBGLTF_INCLUDE_DIRS ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR})
configure_file(${PROJECT_NAME}-config.cmake.cmakein ${CMAKE_BINARY_DIR}/${PROJECT_NAME}-config.cmake @ONLY)
configure_file(${PROJECT_NAME}.h.cmakein ${CMAKE_BINARY_DIR}/include/libgltf/libgltf.h @ONLY)

set(CODE_FILE_LIST)
set(CODE_FILE_LIST_TEMP
    ${CMAKE_BINARY_DIR}/include/libgltf/libgltf.h
    )
source_group("include" FILES ${CODE_FILE_LIST_TEMP})
list(APPEND CODE_FILE_LIST ${CODE_FILE_LIST_TEMP})

set(CODE_FILE_LIST_TEMP
    libgltf.cpp
    libgltfparser.h
    libgltfparser.cpp
    libgltfloader.cpp
    common.h
    common.cpp
    utility.h
    utility.cpp
    gltf_loader.h
    gltf_loader.cpp
    )
source_group("src" FILES ${CODE_FILE_LIST_TEMP})
list(APPEND CODE_FILE_LIST ${CODE_FILE_LIST_TEMP})

if(LIBGLTF_WITH_GOOGLE_DRACO)
    set(CODE_FILE_LIST_TEMP
        extensions/google_draco.h
        extensions/google_draco.cpp
        )
    source_group("source\\extensions" FILES ${CODE_FILE_LIST_TEMP})
    list(APPEND CODE_FILE_LIST ${CODE_FILE_LIST_TEMP})
endif()

add_library(${PROJECT_NAME} STATIC ${CODE_FILE_LIST})

if(${LIBGLTF_PLATFORM_ANDROID})
    set(ANDROID_ABI_PATH ${ANDROID_ABI})
    if(${ANDROID_ARM_NEON})
        set(ANDROID_ABI_PATH "${ANDROID_ABI}-with-neon")
    endif()
    string(TOLOWER ${ANDROID_ABI_PATH} ANDROID_ABI_PATH)
elseif(${LIBGLTF_PLATFORM_IOS})
    set(IOS_PLATFORM_PATH ${IOS_PLATFORM})
    string(TOLOWER ${IOS_PLATFORM_PATH} IOS_PLATFORM_PATH)
endif()

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${CMAKE_BINARY_DIR}/include
    PRIVATE
        ${HEADER_PATH}
        ${PROJECT_SOURCE_DIR}
    )

if(RapidJSON_FOUND)
    target_include_directories(${PROJECT_NAME}
        PRIVATE
            $<BUILD_INTERFACE:${RapidJSON_INCLUDE_DIRS}>
        )
elseif(TARGET RapidJSON)
    get_target_property(RapidJSON_INCLUDE_DIRS RapidJSON INTERFACE_INCLUDE_DIRECTORIES)
    target_include_directories(${PROJECT_NAME}
        PRIVATE
            $<BUILD_INTERFACE:${RapidJSON_INCLUDE_DIRS}>
        )
else()
    target_include_directories(${PROJECT_NAME}
        PRIVATE
            ${EXTERNAL_PATH}/rapidjson/include
        )
endif()

if(LIBGLTF_WITH_GOOGLE_DRACO)
    if(draco_FOUND)
        target_include_directories(${PROJECT_NAME}
            PRIVATE
                $<BUILD_INTERFACE:${draco_INCLUDE_DIRS}>
            )
        target_link_libraries(${PROJECT_NAME}
            PUBLIC
                $<BUILD_INTERFACE:${draco_LIBRARIES}>
            )
    elseif(TARGET draco)
        target_include_directories(${PROJECT_NAME}
            PRIVATE
                ${GOOGLE_DRACO_PATH_INCLUDE}
                ${GOOGLE_DRACO_PATH_BUILD}
            )
        target_link_libraries(${PROJECT_NAME}
            PUBLIC
                ${LIBDRACO_NAME}
            )
    else()
        target_include_directories(${PROJECT_NAME}
            PRIVATE
                ${GOOGLE_DRACO_PATH_INCLUDE}
                ${GOOGLE_DRACO_PATH_BUILD}
            )
        target_link_libraries(${PROJECT_NAME}
            PUBLIC
                ${LIBDRACO_NAME}
            )
    endif()
endif()

set_target_properties(${PROJECT_NAME}
    PROPERTIES
        FOLDER main
    )

install(
    TARGETS ${PROJECT_NAME}
    EXPORT  ${PROJECT_NAME}-targets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
    )

install(DIRECTORY ${CMAKE_BINARY_DIR}/include
    DESTINATION "."
    )
install(FILES ${CMAKE_BINARY_DIR}/${PROJECT_NAME}-config.cmake
    DESTINATION "share/${PROJECT_NAME}"
    )

install(FILES ${ROOT_PATH}/LICENSE.md
    DESTINATION "."
    RENAME LICENSE.md
    )

if(NOT draco_FOUND)
    install(FILES ${EXTERNAL_PATH}/rapidjson/license.txt
        DESTINATION "."
        RENAME LICENSE-rapidjson.txt
        )
endif()

if(LIBGLTF_WITH_GOOGLE_DRACO)
    if(NOT draco_FOUND)
        install(FILES ${EXTERNAL_PATH}/draco/LICENSE
            DESTINATION "."
            RENAME LICENSE-draco
            )
    endif()
endif()

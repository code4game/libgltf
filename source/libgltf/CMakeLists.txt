project(libgltf)

add_library(${PROJECT_NAME})

set(LIBGLTF_INCLUDE_DIRS ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR})
configure_file(${PROJECT_NAME}-config.cmake.cmakein ${CMAKE_BINARY_DIR}/${PROJECT_NAME}-config.cmake @ONLY)
configure_file(${PROJECT_NAME}.h.cmakein ${CMAKE_BINARY_DIR}/include/libgltf/libgltf.h @ONLY)

set(HEADER_FILE_LIST
    ${CMAKE_BINARY_DIR}/include/libgltf/libgltf.h
    )
source_group("include" FILES ${HEADER_FILE_LIST})
target_sources(${PROJECT_NAME}
    PUBLIC
        ${HEADER_FILE_LIST}
    )

set(SOURCE_FILE_LIST
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
source_group("src" FILES ${SOURCE_FILE_LIST})
target_sources(${PROJECT_NAME}
    PRIVATE
        ${SOURCE_FILE_LIST}
    )

set(CMAKE_FILE_LIST
    ${PROJECT_NAME}-config.cmake.cmakein
    ${PROJECT_NAME}.h.cmakein
    )
source_group("cmake" FILES ${CMAKE_FILE_LIST})
target_sources(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_FILE_LIST}
    )

if(LIBGLTF_WITH_GOOGLE_DRACO)
    set(SOURCE_FILE_LIST_DRACO
        extensions/google_draco.h
        extensions/google_draco.cpp
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
elseif(${LIBGLTF_PLATFORM_IOS})
    set(IOS_PLATFORM_PATH ${IOS_PLATFORM})
    string(TOLOWER ${IOS_PLATFORM_PATH} IOS_PLATFORM_PATH)
endif()

target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<INSTALL_INTERFACE:include>${CMAKE_BINARY_DIR}/include
    PRIVATE
        ${HEADER_PATH}
        ${SOURCE_FILE_PATH}
        ${EXTERNAL_PATH}/rapidjson/include
    )

#target_link_libraries(${PROJECT_NAME}
#    PRIVATE
#        rapidjson
#    )

if(LIBGLTF_WITH_GOOGLE_DRACO)
    target_include_directories(${PROJECT_NAME}
        PRIVATE
            ${GOOGLE_DRACO_PATH_INCLUDE}
            ${GOOGLE_DRACO_PATH_BUILD}
        )
    target_link_libraries(${PROJECT_NAME}
        PUBLIC
            draco
            #draco_core
        )
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

install(FILES ${ROOT_PATH}/LICENSE.md
    DESTINATION .
    RENAME LICENSE.md
    )
install(FILES ${HEADER_FILE_LIST}
    DESTINATION include/${PROJECT_NAME}
    )
install(FILES ${CMAKE_BINARY_DIR}/${PROJECT_NAME}-config.cmake
    DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_DATAROOTDIR}/cmake
    )

if(LIBGLTF_WITH_GOOGLE_DRACO)
    install(FILES ${EXTERNAL_PATH}/draco/LICENSE
        DESTINATION .
        RENAME draco-LICENSE
        )
endif()

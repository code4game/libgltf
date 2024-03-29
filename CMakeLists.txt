cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

project(libgltf CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
#enable_testing()

set(CMAKE_DEBUG_POSTFIX "d")
set(CMAKE_CXX_STANDARD 17)

# define the platform
set(LIBGLTF_PLATFORM_WINDOWS  FALSE)
set(LIBGLTF_PLATFORM_LINUX    FALSE)
set(LIBGLTF_PLATFORM_MACOS    FALSE)
set(LIBGLTF_PLATFORM_ANDROID  FALSE)
set(LIBGLTF_PLATFORM_IOS      FALSE)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows" AND MSVC)
    set(LIBGLTF_WITH_MSVC_MT OFF CACHE BOOL "Build MSVC with 'MultiThreading(MT)' or 'MultiThreadingDLL(MD)'?")
    if(${LIBGLTF_WITH_MSVC_MT})
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    else()
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
    endif()
    set(LIBGLTF_PLATFORM_WINDOWS TRUE)
    add_definitions(-DLIBGLTF_PLATFORM_WINDOWS)
    if((CMAKE_EXE_LINKER_FLAGS STREQUAL "/machine:X86") OR (CMAKE_EXE_LINKER_FLAGS STREQUAL " /machine:X86"))
        set(TARGET_ARCHITECTURES_X86 TRUE)
        set(PLATFORM_NAME "win32")
    elseif((CMAKE_EXE_LINKER_FLAGS STREQUAL "/machine:x64") OR (CMAKE_EXE_LINKER_FLAGS STREQUAL " /machine:x64"))
        set(TARGET_ARCHITECTURES_X64 TRUE)
        set(PLATFORM_NAME "win64")
    endif()
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(LIBGLTF_PLATFORM_LINUX TRUE)
    add_definitions(-DLIBGLTF_PLATFORM_LINUX)
    set(PLATFORM_NAME "linux")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wshadow -Wconversion -Wno-long-long -pedantic")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -Wextra -Wshadow -Wconversion -Wsign-conversion -Wno-long-long -pedantic")
    set(LIBGLTF_BUILD_COVERAGE OFF CACHE BOOL "Coverage gcov (debug, Linux builds only)")
    if(LIBGLTF_BUILD_COVERAGE)
        add_definitions(-DLIBGLTF_BUILD_COVERAGE)
        set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fprofile-arcs -ftest-coverage -fPIC")
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fprofile-arcs -ftest-coverage -fPIC")
        set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fprofile-arcs -ftest-coverage -lgcov")
    endif()
elseif(CMAKE_SYSTEM_NAME STREQUAL "Android")
    set(LIBGLTF_PLATFORM_ANDROID TRUE)
    add_definitions(-DLIBGLTF_PLATFORM_ANDROID)
    set(PLATFORM_NAME "android")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wshadow -Wconversion -Wno-long-long -pedantic")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -Wshadow -Wconversion -Wsign-conversion -Wno-long-long -pedantic")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin" AND (NOT ${LIBGLTF_PLATFORM_IOS}))
    set(LIBGLTF_PLATFORM_MACOS TRUE)
    add_definitions(-DLIBGLTF_PLATFORM_MACOS)
    set(PLATFORM_NAME "macos")
    set(CMAKE_C_FLAGS "-Wall -Wextra -Wshadow -Wconversion -Wno-long-long -pedantic ${CMAKE_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra -Wshadow -Wconversion -Wsign-conversion -Wno-long-long -pedantic ${CMAKE_CXX_FLAGS}")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin" AND ${LIBGLTF_PLATFORM_IOS})
    set(LIBGLTF_PLATFORM_IOS TRUE)
    add_definitions(-DLIBGLTF_PLATFORM_IOS)
    set(PLATFORM_NAME "ios")
    set(CMAKE_C_FLAGS "-Wall -Wextra -Wshadow -Wconversion -Wno-long-long -pedantic ${CMAKE_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra -Wshadow -Wconversion -Wsign-conversion -Wno-long-long -pedantic ${CMAKE_CXX_FLAGS}")
endif()

if((NOT ${LIBGLTF_PLATFORM_WINDOWS}) AND (NOT ${LIBGLTF_PLATFORM_LINUX}) AND (NOT ${LIBGLTF_PLATFORM_ANDROID}) AND (NOT ${LIBGLTF_PLATFORM_MACOS}) AND (NOT ${LIBGLTF_PLATFORM_IOS}))
    message(FATAL_ERROR "Sorry, don't support your system ${CMAKE_SYSTEM_NAME}!")
endif()

set(ROOT_PATH     "${PROJECT_SOURCE_DIR}")
set(SOURCE_PATH   "${ROOT_PATH}/source")
set(TOOLS_PATH    "${ROOT_PATH}/tools")
set(EXTERNAL_PATH "${ROOT_PATH}/external")

find_package(RapidJSON CONFIG)
find_package(draco CONFIG)

# integrate the Google's draco
set(LIBGLTF_WITH_GOOGLE_DRACO OFF CACHE BOOL "Use Google's Draco?")
if(LIBGLTF_WITH_GOOGLE_DRACO)
    if(MSVC)
        set(LIBDRACO_NAME draco)
    else()
        set(LIBDRACO_NAME draco_static)
    endif()
    if(NOT draco_FOUND)
        #set(DRACO_GLTF TRUE CACHE BOOL "")
        add_subdirectory(${EXTERNAL_PATH}/draco)
        set_target_properties(${LIBDRACO_NAME}                               PROPERTIES FOLDER externals/draco)
        set_target_properties(draco_animation                                PROPERTIES FOLDER externals/draco)
        set_target_properties(draco_animation_dec                            PROPERTIES FOLDER externals/draco)
        set_target_properties(draco_animation_enc                            PROPERTIES FOLDER externals/draco)
        set_target_properties(draco_attributes                               PROPERTIES FOLDER externals/draco)
        set_target_properties(draco_compression_attributes_dec               PROPERTIES FOLDER externals/draco)
        set_target_properties(draco_compression_attributes_enc               PROPERTIES FOLDER externals/draco)
        set_target_properties(draco_compression_attributes_pred_schemes_dec  PROPERTIES FOLDER externals/draco)
        set_target_properties(draco_compression_attributes_pred_schemes_enc  PROPERTIES FOLDER externals/draco)
        set_target_properties(draco_compression_bit_coders                   PROPERTIES FOLDER externals/draco)
        set_target_properties(draco_compression_decode                       PROPERTIES FOLDER externals/draco)
        set_target_properties(draco_compression_encode                       PROPERTIES FOLDER externals/draco)
        set_target_properties(draco_compression_entropy                      PROPERTIES FOLDER externals/draco)
        set_target_properties(draco_compression_mesh_dec                     PROPERTIES FOLDER externals/draco)
        set_target_properties(draco_compression_mesh_enc                     PROPERTIES FOLDER externals/draco)
        set_target_properties(draco_compression_mesh_traverser               PROPERTIES FOLDER externals/draco)
        set_target_properties(draco_compression_point_cloud_dec              PROPERTIES FOLDER externals/draco)
        set_target_properties(draco_compression_point_cloud_enc              PROPERTIES FOLDER externals/draco)
        set_target_properties(draco_core                                     PROPERTIES FOLDER externals/draco)
        set_target_properties(draco_dec_config                               PROPERTIES FOLDER externals/draco)
        set_target_properties(draco_decoder                                  PROPERTIES FOLDER externals/draco)
        set_target_properties(draco_enc_config                               PROPERTIES FOLDER externals/draco)
        set_target_properties(draco_encoder                                  PROPERTIES FOLDER externals/draco)
        set_target_properties(draco_io                                       PROPERTIES FOLDER externals/draco)
        set_target_properties(draco_mesh                                     PROPERTIES FOLDER externals/draco)
        set_target_properties(draco_metadata                                 PROPERTIES FOLDER externals/draco)
        set_target_properties(draco_metadata_dec                             PROPERTIES FOLDER externals/draco)
        set_target_properties(draco_metadata_enc                             PROPERTIES FOLDER externals/draco)
        set_target_properties(draco_point_cloud                              PROPERTIES FOLDER externals/draco)
        set_target_properties(draco_points_dec                               PROPERTIES FOLDER externals/draco)
        set_target_properties(draco_points_enc                               PROPERTIES FOLDER externals/draco)
        set(GOOGLE_DRACO_PATH_INCLUDE ${EXTERNAL_PATH}/draco/src)
        set(GOOGLE_DRACO_PATH_BUILD ${CMAKE_BINARY_DIR})
    else()
        #
    endif()
endif()

set(LIBGLTF_BUILD_RUNTEST FALSE CACHE BOOL "Build runtest?")

add_subdirectory(docs)
add_subdirectory(source)
add_subdirectory(tools)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT libgltf)

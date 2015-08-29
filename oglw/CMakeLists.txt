# sets up those variable
# - OGLW_INCLUDE_DIRS : the include headers in order to use the library
# - OGLW_LIBRARIES : libraries to link

cmake_minimum_required(VERSION 2.8)
project(OGLW)

option(OGLW_BUILD_SHARED "Build OGLW as a shared library" OFF)
option(OGLW_BUILD_SHARED_WITH_JNI "Build OGLW as a shared library with JNIs" OFF)

# CXX Flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-mismatched-tags -Wno-unused-function -std=c++1y")

# Utility functions
function(find_sources_and_include_directories HEADERS_PATH SOURCES_PATH)
    include_recursive_dirs(${HEADERS_PATH})
    file(GLOB_RECURSE FOUND_SOURCES ${SOURCES_PATH})

    set(SOURCES
        ${SOURCES}
        ${FOUND_SOURCES}
        CACHE INTERNAL "sources" FORCE)
    list(REMOVE_DUPLICATES SOURCES)
endfunction(find_sources_and_include_directories)

function(include_recursive_dirs HEADERS_PATH)
    file(GLOB_RECURSE FOUND_HEADERS ${HEADERS_PATH})

    set(INCLUDES ${INCLUDES} "")
    foreach(_headerFile ${FOUND_HEADERS})
        get_filename_component(_dir ${_headerFile} PATH)
        list(APPEND INCLUDES ${_dir})
    endforeach()
    list(REMOVE_DUPLICATES INCLUDES)

    include_directories(${INCLUDES})

    set(HEADERS
        ${HEADERS}
        ${FOUND_HEADERS}
        CACHE INTERNAL "headers" FORCE)
    list(REMOVE_DUPLICATES HEADERS)
endfunction(include_recursive_dirs)

set(INCLUDE_DIRS "")
set(LIBRARIES "")

message(STATUS "Initializing third party submodules")
execute_process(
        COMMAND git submodule update --init --recursive
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})

# GLFW
message(STATUS "Add GLFW")
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Build the GLFW example programs")
set(GLFW_BUILD_TESTS OFF CACHE BOOL "Build the GLFW test programs")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "Build the GLFW documentation")
set(GLFW_INSTALL OFF CACHE BOOL "Generate installation target")

add_subdirectory(${PROJECT_SOURCE_DIR}/3rdparty/glfw)
include_directories(${PROJECT_SOURCE_DIR}/3rdparty/glfw/include)
set(INCLUDE_DIRS ${INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/3rdparty/glfw/include)
set(LIBRARIES ${LIBRARIES} glfw ${GLFW_LIBRARIES})

# GLM
message(STATUS "Include GLM")
set(INCLUDE_DIRS ${INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/3rdparty/glm)
include_directories(${PROJECT_SOURCE_DIR}/3rdparty/glm)

# Tiny obj loader
message(STATUS "Add tinyobjloader")
set(INCLUDE_DIRS ${INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/3rdparty/tinyobjloader)
add_library(tinyobjloader ${PROJECT_SOURCE_DIR}/3rdparty/tinyobjloader/tiny_obj_loader.cc)
set(LIBRARIES ${LIBRARIES} tinyobjloader)

# GLEW
message(STATUS "Add glew")
set(glew-cmake_BUILD_SHARED OFF CACHE BOOL "Build glew shared")
set(LIBRARIES ${LIBRARIES} libglew_static)
add_subdirectory(${PROJECT_SOURCE_DIR}/3rdparty/glew-cmake)
include_directories(${PROJECT_SOURCE_DIR}/3rdparty/glew-cmake/include)
set(INCLUDE_DIRS ${INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/3rdparty/glew-cmake/include)

# STB
message(STATUS "Include stb")
include_directories(${PROJECT_SOURCE_DIR}/3rdparty/stb)
set(INCLUDE_DIRS ${INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/3rdparty/stb)

# Add gamma
message(STATUS "Add Gamma")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/3rdparty/Gamma/cmake_modules)
add_subdirectory(${PROJECT_SOURCE_DIR}/3rdparty/Gamma)
include_directories(${PROJECT_SOURCE_DIR}/3rdparty/Gamma)
set(INCLUDE_DIRS ${INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/3rdparty/Gamma)
set(LIBRARIES ${LIBRARIES} Gamma)

# OGLW Include dirs
set(INCLUDE_DIRS
    ${INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/gl
    ${PROJECT_SOURCE_DIR}/text
    ${PROJECT_SOURCE_DIR}/core
    ${PROJECT_SOURCE_DIR}/)

# Add OGLW files and include directories
find_sources_and_include_directories(gl/*.h gl/*.cpp)
find_sources_and_include_directories(core/*.h core/*.cpp)
find_sources_and_include_directories(text/*.h text/*.cpp)
include_directories(${PROJECT_SOURCE_DIR}/)

if (OGLW_BUILD_SHARED OR OGLW_BUILD_SHARED_WITH_JNI)
    if (OGLW_BUILD_SHARED_WITH_JNI)
        # load jni
        find_package(JNI REQUIRED)
        include_directories(${JNI_INCLUDE_DIRS})

        if (NOT ${JNI_FOUND})
            message(STATUS "JNI library not found")
            return()
        endif()

        message(STATUS "Shared options with JNI exports chosen for OGLW")
        set(JNI_SOURCES ${PROJECT_SOURCE_DIR}/java/jni/jniExports.cpp)
        add_library(OGLW SHARED ${SOURCES} ${HEADERS} ${JNI_SOURCES} ${JNI_LIBRARIES})
    else()
        message(STATUS "Shared options chosen for OGLW")
        add_library(OGLW SHARED ${SOURCES} ${HEADERS})
    endif()
    target_link_libraries(OGLW ${LIBRARIES})
else()
    message(STATUS "Static options chosen for OGLW")
    add_library(OGLW STATIC ${SOURCES} ${HEADERS})
endif()

# Output variables
set(OGLW_INCLUDE_DIRS ${INCLUDE_DIRS} CACHE INTERNAL "OGLW include directories" FORCE)
set(OGLW_LIBRARIES ${LIBRARIES} CACHE INTERNAL "OGLW libraries" FORCE)

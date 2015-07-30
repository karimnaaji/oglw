cmake_minimum_required(VERSION 2.8)
project(testApp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++1y")
set(EXECUTABLE_NAME testApp)

find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
find_package(GLEW REQUIRED)

if(NOT GLFW_FOUND)
    message(SEND_ERROR "GLFW not found")
    return()
endif()

if(NOT GLEW_FOUND)
    message(SEND_ERROR "GLEW not found")
    return()
endif()

message(STATUS "Found GLFW ${GLFW_PREFIX}")
message(STATUS "Found GLEW ${GLEW_PREFIX}")

include_directories(${GLFW_INCLUDE_DIRS})
include_directories(${GLEW_INCLUDE_DIRS})

add_subdirectory(${PROJECT_SOURCE_DIR}/oglw/)
include_directories(${OGLW_INCLUDE_DIRS})

file(GLOB_RECURSE RESOURCES ${PROJECT_SOURCE_DIR}/test/resources/*)

add_executable(${EXECUTABLE_NAME} MACOSX_BUNDLE test/main.cpp ${RESOURCES})

# add resource files and property list
set_target_properties(${EXECUTABLE_NAME} PROPERTIES 
    MACOSX_BUNDLE_INFO_PLIST "${PROJECT_SOURCE_DIR}/test/resources/info.plist" 
    RESOURCE "${RESOURCES}")

set(OGL "-framework OpenGL")

target_link_libraries(${EXECUTABLE_NAME} OGLW ${OGLW_DEPS} ${GLFW_LDFLAGS} ${GLEW_LIBRARY} ${OGL})

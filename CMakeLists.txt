cmake_minimum_required(VERSION 2.8)
project(testApp)

# cmake output configuration
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(EXECUTABLE_NAME testApp)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++1y")

add_subdirectory(${PROJECT_SOURCE_DIR}/oglw/)

# include OGLW headers
include_directories(${OGLW_INCLUDE_DIRS})

# find resources
file(GLOB_RECURSE RESOURCES ${PROJECT_SOURCE_DIR}/test/resources/*)

# create executable
if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    add_executable(${EXECUTABLE_NAME} MACOSX_BUNDLE test/main.cpp ${RESOURCES})
    set_target_properties(${EXECUTABLE_NAME} PROPERTIES RESOURCE "${RESOURCES}")    
else()
    add_executable(${EXECUTABLE_NAME} test/main.cpp)
    file(GLOB_RECURSE RESOURCES ${PROJECT_SOURCE_DIR}/test/resources/*)
    foreach(_resource ${RESOURCES})
        file(COPY ${_resource} DESTINATION ${PROJECT_SOURCE_DIR}/build/bin)
    endforeach()
endif()

target_link_libraries(${EXECUTABLE_NAME} OGLW ${OGLW_LIBRARIES})

cmake_minimum_required(VERSION 2.8)
project(testApp)

set(EXECUTABLE_NAME testApp)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++1y")

add_subdirectory(${PROJECT_SOURCE_DIR}/oglw/)

# include OGLW headers
include_directories(${OGLW_INCLUDE_DIRS})

# find resources
file(GLOB_RECURSE RESOURCES ${PROJECT_SOURCE_DIR}/test/resources/*)

# create executable
add_executable(${EXECUTABLE_NAME} MACOSX_BUNDLE test/main.cpp ${RESOURCES})
set_target_properties(${EXECUTABLE_NAME} PROPERTIES RESOURCE "${RESOURCES}")
target_link_libraries(${EXECUTABLE_NAME} OGLW ${OGLW_LIBRARIES})

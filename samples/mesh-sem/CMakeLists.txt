cmake_minimum_required(VERSION 2.8)
project(mesh_sem)

# cmake output configuration
set(EXECUTABLE_NAME mesh_sem)

# find resources
file(GLOB_RECURSE RESOURCES ${PROJECT_SOURCE_DIR}/resources/*)

# create executable
if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    add_executable(${EXECUTABLE_NAME} MACOSX_BUNDLE ${PROJECT_SOURCE_DIR}/main.cpp ${RESOURCES})
    set_target_properties(${EXECUTABLE_NAME} PROPERTIES RESOURCE "${RESOURCES}")    
else()
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${EXECUTABLE_NAME})
    add_executable(${EXECUTABLE_NAME} ${PROJECT_SOURCE_DIR}/main.cpp)
    file(GLOB_RECURSE RESOURCES ${PROJECT_SOURCE_DIR}/resources/*)
    foreach(_resource ${RESOURCES})
        file(COPY ${_resource} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    endforeach()
endif()

target_link_libraries(${EXECUTABLE_NAME} OGLW ${OGLW_LIBRARIES})

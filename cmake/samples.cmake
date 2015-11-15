
# load an oglw sample and create an executable for it
macro(load_oglw_sample SAMPLE_NAME)
    # cmake output configuration
    set(EXECUTABLE_NAME ${SAMPLE_NAME})

    file(GLOB_RECURSE RESOURCES FOLLOW_SYMLINKS ${PROJECT_SOURCE_DIR}/resources/*)
    file(GLOB_RECURSE FOUND_HEADERS ${PROJECT_SOURCE_DIR}/*.h)
    file(GLOB_RECURSE FOUND_SOURCES ${PROJECT_SOURCE_DIR}/*.cpp)

    set(INCLUDES ${INCLUDES} "")
    foreach(_headerFile ${FOUND_HEADERS})
        get_filename_component(_dir ${_headerFile} PATH)
        list(APPEND INCLUDES ${_dir})
    endforeach()
    list(REMOVE_DUPLICATES INCLUDES)
    include_directories(${INCLUDES})

    # create executable
    if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
        add_executable(${EXECUTABLE_NAME} MACOSX_BUNDLE ${FOUND_SOURCES} ${RESOURCES})
        set_target_properties(${EXECUTABLE_NAME} PROPERTIES RESOURCE "${RESOURCES}")
    else()
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${EXECUTABLE_NAME})
        add_executable(${EXECUTABLE_NAME} ${FOUND_SOURCES})
        foreach(_resource ${RESOURCES})
            add_custom_command(
                TARGET ${EXECUTABLE_NAME}
                COMMAND POST_BUILD cp -P ${_resource} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
        endforeach()
    endif()

    target_link_libraries(${EXECUTABLE_NAME} OGLW ${OGLW_LIBRARIES})
endmacro(load_oglw_sample)

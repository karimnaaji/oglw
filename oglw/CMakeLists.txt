cmake_minimum_required(VERSION 2.8)
project(OGLW)

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

    set(INCLUDE_DIRS "")
    foreach(_headerFile ${FOUND_HEADERS})
        get_filename_component(_dir ${_headerFile} PATH)
        list(APPEND INCLUDE_DIRS ${_dir})
    endforeach()
    list(REMOVE_DUPLICATES INCLUDE_DIRS)

    include_directories(${INCLUDE_DIRS})

    set(HEADERS
        ${HEADERS}
        ${FOUND_HEADERS}
        CACHE INTERNAL "headers" FORCE)
    list(REMOVE_DUPLICATES HEADERS)
endfunction(include_recursive_dirs)

set(OGLW_INCLUDE_DIRS
    ${PROJECT_SOURCE_DIR}/deps
    ${PROJECT_SOURCE_DIR}/glm
    ${PROJECT_SOURCE_DIR}/obj
    ${PROJECT_SOURCE_DIR}/utils
    ${PROJECT_SOURCE_DIR}/gl
    ${PROJECT_SOURCE_DIR}/stb
    ${PROJECT_SOURCE_DIR}/text
    ${PROJECT_SOURCE_DIR}/
    CACHE INTERNAL "include directories" FORCE)

include_directories(${OGLW_INCLUDE_DIRS})

# deps
set(OGLW_DEPS
    ${PROJECT_SOURCE_DIR}/deps/Gamma/libGamma.a
    ${PROJECT_SOURCE_DIR}/deps/Gamma/libportaudio.a
    ${PROJECT_SOURCE_DIR}/deps/Gamma/libsndfile.a
    "-framework AudioUnit"
    "-framework AudioToolbox"
    "-framework CoreAudio"
    "-framework Carbon"
    CACHE INTERNAL "oglw deps" FORCE)

find_sources_and_include_directories(gl/*.h gl/*.cpp)
find_sources_and_include_directories(obj/*.h obj/*.cpp)
find_sources_and_include_directories(utils/*.h utils/*.cpp)

add_library(OGLW STATIC ${SOURCES} ${HEADERS})

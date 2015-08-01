|Platform|Build Status|
|--------|------|
|OSX & Linux|[![Build Status](https://travis-ci.org/karimnaaji/oglw.svg?branch=master)](https://travis-ci.org/karimnaaji/oglw)|


# oglw
OpenGL 3.2 wrapper utilities and more

build
=====

```
brew tap homebrew/versions
brew install cmake Portaudio LibSndFile
```

Creating an app
===============
Minimal cmake configuration:
```cmake
cmake_minimum_required(VERSION 2.8)
project(OGLWApp)

# c++14 flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++1y")

# add OGLW library
add_subdirectory(${PROJECT_SOURCE_DIR}/oglw/)

# include OGLW headers
include_directories(${OGLW_INCLUDE_DIRS})

# find resources
file(GLOB_RECURSE RESOURCES ${PROJECT_SOURCE_DIR}/OGLWApp/resources/*)

# create an executable bundled with resources (OS X)
add_executable(OGLWApp MACOSX_BUNDLE OGLWApp/main.cpp ${RESOURCES})
set_target_properties(${EXECUTABLE_NAME} PROPERTIES RESOURCE "${RESOURCES}")
target_link_libraries(${EXECUTABLE_NAME} OGLW ${OGLW_LIBRARIES})
````


Building the test app
=====================
```
mkdir build && cd build
cmake .. && make
open testApp.app
```

![00](img/test-app.png)

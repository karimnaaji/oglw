|Platform|Build Status|
|--------|------|
|OSX & Linux|[![Build Status](https://travis-ci.org/karimnaaji/oglw.svg?branch=master)](https://travis-ci.org/karimnaaji/oglw)|


# OGLW
OpenGL 3.2 wrapper utilities and more

Build the demo
==============

Linux
-----

```
sudo apt-get install portaudio19-dev libsndfile-dev
mkdir build && cd build
cmake .. && make
```
OS X
----

```
brew install cmake portaudio libsndfile
mkdir build && cd build
cmake .. && make
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

## Experiments

| Screenshot  | Name |
| ------------- | ------------- |
| [![](img/capture0.png)](/blocks)| [code](https://github.com/karimnaaji/vectiler/tree/master/renderer) - |
| [![](img/capture1.png)](/blocks)| [code](https://github.com/karimnaaji/oglw/tree/master/samples/tile) - |
| [![](img/capture2.png)](/blocks)| [code](https://github.com/karimnaaji/oglw/tree/master/samples/terrain) - |
| [![](img/capture3.png)](/blocks)| [code]() - |
| [![](img/capture4.png)](/blocks)| [code](https://github.com/karimnaaji/oglw/tree/master/samples/debugdraw) - |
| [![](img/capture5.png)](/blocks)| [code](https://github.com/karimnaaji/oglw/tree/master/samples/mesh-sem) - |
| [![](img/capture6.png)](/blocks)| [code](https://github.com/karimnaaji/oglw/tree/master/samples/debugdraw) - |

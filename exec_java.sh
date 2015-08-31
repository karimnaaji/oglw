#!/usr/bin/env bash
mkdir -p build
cd build
cmake ..
make
cd lib
javac ../../oglw/java/Main.java
mv ../../oglw/java/Main.class .
java -XstartOnFirstThread Main
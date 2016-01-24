#!/bin/bash

echo -n "Sample name > "
read sample_name
echo "Creating sample: samples/$sample_name"
mkdir samples/$sample_name
mkdir samples/$sample_name/resources
sed 's/$project_name/'$sample_name'/g' template/main.txt > samples/$sample_name/main.cpp
sed 's/$project_name/'$sample_name'/g' template/cmake.txt > samples/$sample_name/CMakeLists.txt


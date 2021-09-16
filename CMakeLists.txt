cmake_minimum_required(VERSION 3.10)

# set the project name
project(fakeos VERSION 1.0)

# add subdirectory
add_subdirectory(tools)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
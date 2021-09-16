cmake_minimum_required(VERSION 3.10)

# set the project name
project(make_boot VERSION 1.0)

# add the executable
add_executable(make_boot make_boot.cpp)

configure_file(tools_config.h.in tools_config.h)

target_include_directories(make_boot PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           )

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
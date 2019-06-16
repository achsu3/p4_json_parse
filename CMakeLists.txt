cmake_minimum_required(VERSION 3.5)
project(p4aig VERSION 0.0.1 LANGUAGES CXX)

# add the modules
list(INSERT CMAKE_MODULE_PATH 0 ${PROJECT_SOURCE_DIR}/cmake)

# get the dependencies
find_package(RapidJSON REQUIRED)

# add the src subdirectory that will contain everything we will need to define, including out tests
add_subdirectory(src)
add_subdirectory(test)


cmake_minimum_required(VERSION 3.5)
project(p4aig VERSION 0.0.1 LANGUAGES CXX)

# add the src subdirectory that will contain everything we will need to define, including out tests
add_subdirectory(src)

add_subdirectory(test)


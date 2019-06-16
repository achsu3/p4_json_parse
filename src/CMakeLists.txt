# P4aig

# create the parser library
add_library(libparser
    parser/parser.cpp
    parser/handlers.cpp)

target_include_directories(libparser
    PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_options(libparser PRIVATE -Wall -Werror)
target_compile_features(libparser PRIVATE cxx_std_11)

# create the circuits library
add_library(libcircuits
    circuits/circuits.cpp)

target_include_directories(libcircuits
    PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_options(libcircuits PRIVATE -Wall -Werror)
target_compile_features(libcircuits PRIVATE cxx_std_11)

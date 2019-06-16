# p4aig

add_executable(test src/test_main.cpp)
target_include_directories(test
    PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>)

target_compile_options(test PRIVATE -Wall -Werror)
target_compile_features(test PRIVATE cxx_std_11)

target_link_libraries(test PUBLIC libp4aig libparser libcircuits)

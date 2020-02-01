cmake_minimum_required(VERSION 2.8.11)
project(TestGmock)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

enable_testing()

add_executable(forwarder_test forwarder_test.cpp)
target_link_libraries(forwarder_test ${CONAN_LIBS})

add_test(forwarder bin/forwarder_test)

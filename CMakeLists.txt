cmake_minimum_required (VERSION 3.15)

project (MatrixMul)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

add_executable(MatrixMul main.cpp)

target_link_libraries(MatrixMul PRIVATE Threads::Threads)
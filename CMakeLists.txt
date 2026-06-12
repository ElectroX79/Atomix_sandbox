

cmake_minimum_required(VERSION 3.15)
project(Atomix VERSION 0.1 LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-O3 -march=native -Wall -Wextra)

add_library(AtomixCore INTERFACE)
target_include_directories(AtomixCore INTERFACE include)

add_executable(atomix src/main.cpp)
target_link_libraries(atomix PRIVATE AtomixCore)


add_executable(unit_test 
external/catch2/catch_amalgamated.cpp 
test/tabular_data_test.cpp
)

target_include_directories(unit_test PRIVATE include external/catch2)
target_link_libraries(unit_test PRIVATE AtomixCore)
cmake_minimum_required(VERSION 3.17...3.22)
set(CMAKE_CXX_STANDARD 17)
project(MPIDebug VERSION 1.0.0 LANGUAGES CXX)

find_package(MPI REQUIRED)
add_executable(main main.cpp)

target_link_libraries(main PRIVATE MPI::MPI_CXX)

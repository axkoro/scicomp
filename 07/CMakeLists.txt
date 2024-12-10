cmake_minimum_required(VERSION 3.15...3.31)
project(WissenschaftlichesRechnen VERSION 1.0 LANGUAGES CXX)

add_executable(ParallelMergeSort main.cpp)
target_compile_features(ParallelMergeSort PUBLIC cxx_std_20)

find_package(MPI REQUIRED)
target_link_libraries(ParallelMergeSort PRIVATE MPI::MPI_CXX)

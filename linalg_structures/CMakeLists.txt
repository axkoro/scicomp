cmake_minimum_required(VERSION 3.15...3.31)
project(WissenschaftlichesRechnen VERSION 1.0 LANGUAGES CXX)

add_executable(Blatt3 main.cpp structures/DenseVector.cpp structures/CSRMatrix.cpp)
target_compile_features(Blatt3 PUBLIC cxx_std_20)
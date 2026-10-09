cmake_minimum_required(VERSION 3.15)
project(src)

set(CMAKE_CXX_STANDARD 14)

add_executable(src main.cpp MatrixCalculator.cpp MatrixCalculator.h Matrix.cpp Matrix.h ECommand.h CommandFactory.cpp CommandFactory.h MatrixFactory.cpp MatrixFactory.h DenseMatrix.cpp DenseMatrix.h SparseMatrix.cpp SparseMatrix.h)
# Scientific Computing Projects

A collection of projects I worked on during my Scientific Computing coursework using C++, Python, and MPI.

## Projects

### Linear Algebra Data Structures
I implemented fundamental linear algebra building blocks including a `DenseVector` class with standard operations and a `CSRMatrix` class for efficiently storing sparse matrices using [Compressed Sparse Row format](https://de.wikipedia.org/wiki/Compressed_Row_Storage).

### Sparse Matrix Operations
I extended my linear algebra structures with sparse matrix-matrix multiplication ([SpGEMM](https://arxiv.org/abs/2002.11273)) using a Sparse Accumulator technique. This significantly improved performance for sparse matrix operations compared to naive approaches.

### Linear System Solvers
I implemented two solvers for linear systems:
- A [Conjugate Gradient](https://en.wikipedia.org/wiki/Conjugate_gradient_method) solver for large sparse matrices
- A Gaussian Elimination solver for comparison (the school way of solving linear systems)

### MPI Ring Sum
My first MPI project - a simple distributed sum algorithm using a ring topology. Each process generates a random number, then passes messages around to compute their collective sum. It was a great introduction to distributed computing patterns.

### MPI Parallel Sort
A distributed sorting implementation using MPI. The algorithm divides data across available processes, sorts each chunk locally, and then merges everything back together.

### Poisson Solver
A Python script that solves the 2D [Poisson equation](https://en.wikipedia.org/wiki/Poisson%27s_equation) using finite differences. The program creates an animation showing how the solution evolves over time (though there's still a bug in there somewhere that I haven't tracked down yet).

## Setup

For C++ projects:
```bash
cd project_directory
mkdir build && cd build
cmake ..
make
./project_executable
```

For the Poisson solver:
```bash
cd poisson_solver
python poissson_solver.py
```

## Tech Stack
- C++ (C++11)
- Python with NumPy and Matplotlib
- MPI for parallel computing
- CMake for building
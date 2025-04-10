#include <mpi.h>

#include <iostream>
#include <random>
#include <vector>

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rows_per_proc = 3;
    int num_columns = 100;
    std::vector<std::vector<double> > matrix_part(rows_per_proc, std::vector<double>(num_columns));

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(-100, 100);

    for (auto &row : matrix_part) {
        std::generate(row.begin(), row.end(), [&]() { return dist(gen); });
    }

    // Local computation

    std::vector<double> row_sums_part(rows_per_proc);
    for (int i = 0; i < rows_per_proc; i++) {
        double row_sum = 0.0;
        for (int j = 0; j < num_columns; j++) {
            row_sum += matrix_part[i][j];
        }
        row_sums_part[i] = row_sum;
    }

    // Communication

    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<double> row_sums;
    if (rank == 0) row_sums.resize(rows_per_proc * size);

    MPI_Gather(row_sums_part.data(), rows_per_proc, MPI_DOUBLE, row_sums.data(), rows_per_proc,
               MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        int i = 0;
        for (double row_sum : row_sums) {
            std::cout << "Row sum in row " << i << ": " << row_sum << std::endl;
            ++i;
        }
    }

    MPI_Finalize();
}
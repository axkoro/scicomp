#include <mpi.h>

#include <iostream>
#include <random>

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    unsigned seed = 42 + rank;
    std::default_random_engine gen(seed);
    std::uniform_int_distribution<int> dist(1, 100);
    int r = dist(gen);

    std::cout << "Proc. " << rank << ": r=" << r << "\n";

    int s;
    if (rank == 0) {
        // send r to right neighbour
        MPI_Send(&r, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);

        // receive result from left neighbour
        MPI_Recv(&s, 1, MPI_INT, size - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        // send final result to right neighbour
        MPI_Send(&s, 1, MPI_INT, 1, 1, MPI_COMM_WORLD);

        int dummy;
        MPI_Recv(&dummy, 1, MPI_INT, size - 1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    } else {
        // propagate (modified) intermediate result
        MPI_Recv(&s, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        s += r;
        MPI_Send(&s, 1, MPI_INT, (rank + 1) % size, 0, MPI_COMM_WORLD);

        // propagate final result
        MPI_Recv(&s, 1, MPI_INT, rank - 1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Send(&s, 1, MPI_INT, (rank + 1) % size, 1, MPI_COMM_WORLD);
    }

    // Make each process wait for its turn to print (prevent interleaved output)
    for (int i = 0; i < size; i++) {
        MPI_Barrier(MPI_COMM_WORLD);
        if (rank == i) {
            std::cout << "Proc. " << rank << ": s=" << s << std::endl;
        }
    }

    MPI_Finalize();
    return 0;
}
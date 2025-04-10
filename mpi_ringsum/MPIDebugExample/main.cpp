#include <iostream>
#include <mpi.h>

int main(int argc, char *argv[]) {
  MPI_Init(&argc, &argv);

  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  // uncomment for debugging
  // bool wait_debug = true;
  // while (wait_debug)
  //   ;

  int num = rank * 3;
  int rec_num;

  std::cout << "[rank " << rank << "] sending: " << num << "\n";

  if (rank == 0) {
    MPI_Sendrecv(&num, 1, MPI_INT, 1, 0, &rec_num, 1, MPI_INT, 1, 0,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  }
  if (rank == 1) {
    MPI_Sendrecv(&num, 1, MPI_INT, 0, 0, &rec_num, 1, MPI_INT, 0, 0,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  }

  std::cout << "[rank " << rank << "] rec_num: " << rec_num << "\n";

  MPI_Finalize();
  return 0;
}
#include <mpi.h>

#include <iostream>
#include <vector>

void sort_parallel(std::vector<int> &vec, int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, group_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &group_size);

    int local_vec_size = vec.size() / group_size;
    std::vector<int> sendcounts(group_size, local_vec_size);
    int remainder = vec.size() % group_size;
    for (int i = 0; i < remainder; i++) {  // fill remaining size to front vectors
        sendcounts[i]++;
    }

    std::vector<int> offsets(group_size, 0);
    for (int i = 1; i < offsets.size(); i++) {
        offsets[i] = offsets[i - 1] + sendcounts[i - 1];
    }

    int my_size;
    MPI_Scatter(sendcounts.data(), 1, MPI_INT, &my_size, 1, MPI_INT, 0, MPI_COMM_WORLD);

    std::vector<int> local_vec(my_size);
    MPI_Scatterv(vec.data(), sendcounts.data(), offsets.data(), MPI_INT, local_vec.data(), my_size,
                 MPI_INT, 0, MPI_COMM_WORLD);

    std::sort(local_vec.begin(), local_vec.end());

    MPI_Gatherv(local_vec.data(), my_size, MPI_INT, vec.data(), sendcounts.data(), offsets.data(),
                MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        for (int i = 1; i < group_size - 1; i++) {
            auto middle = vec.begin() + offsets[i];
            auto last = vec.begin() + offsets[i + 1];
            std::inplace_merge(vec.begin(), middle, last);
        }
        auto middle = vec.begin() + offsets[group_size - 1];
        std::inplace_merge(vec.begin(), middle, vec.end());
    }

    MPI_Finalize();
}

int main(int argc, char *argv[]) {
    std::vector<int> vec = {3, 2, 9, -2, 45, 7, 5, 3};  // TODO: Random Vektoren generieren
    sort_parallel(vec, argc, argv);

    if (std::is_sorted(vec.begin(), vec.end())) {
        std::cout << "Successfully sorted!" << '\n';
    } else {
        std::cout << "Not sorted!" << '\n';
    }

    std::cout << "[ ";
    for (auto &&val : vec) {
        std::cout << val << ' ';
    }
    std::cout << "]\n";

    return 0;
}

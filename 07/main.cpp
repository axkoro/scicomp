#include <mpi.h>

#include <chrono>
#include <iostream>
#include <random>
#include <vector>

void sort_parallel(std::vector<int> &vec, int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, group_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &group_size);

    int local_chunk_size = vec.size() / group_size;
    std::vector<int> sendcounts(group_size, local_chunk_size);
    int remainder = vec.size() % group_size;
    for (int i = 0; i < remainder; i++) {  // fill remaining size into front vectors
        sendcounts[i]++;
    }

    std::vector<int> offsets(group_size, 0);
    for (int i = 1; i < offsets.size(); i++) {  // prefix sum
        offsets[i] = offsets[i - 1] + sendcounts[i - 1];
    }

    int my_sendcount;
    MPI_Scatter(sendcounts.data(), 1, MPI_INT, &my_sendcount, 1, MPI_INT, 0, MPI_COMM_WORLD);

    std::vector<int> vec_chunk(my_sendcount);
    MPI_Scatterv(vec.data(), sendcounts.data(), offsets.data(), MPI_INT, vec_chunk.data(),
                 my_sendcount, MPI_INT, 0, MPI_COMM_WORLD);

    std::sort(vec_chunk.begin(), vec_chunk.end());

    MPI_Gatherv(vec_chunk.data(), my_sendcount, MPI_INT, vec.data(), sendcounts.data(),
                offsets.data(), MPI_INT, 0, MPI_COMM_WORLD);

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
    // Generate random vector
    int seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 gen(seed);
    std::uniform_int_distribution<int> dist(0, 50);

    int len = dist(gen);
    std::vector<int> vec(len);

    dist = std::uniform_int_distribution<int>(-999, 999);
    for (int i = 0; i < len; i++) {
        vec[i] = dist(gen);
    }

    // Sort
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

#include <mpi.h>
#include <iostream>
#include <vector>

const int ROOT = 0;

static int lab_2nd(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 4) {
        if (rank == ROOT) {
            std::cerr << "This program requires exactly 4 processes." << std::endl;
        }
        MPI_Finalize();
        return 1;
    }

    const int rowsA = 8, colsA = 5, rowsB = 5, colsB = 3;

    int A[rowsA][colsA];
    int B[rowsB][colsB];
    if (rank == ROOT) {
        int tempA[rowsA][colsA] = {
            {1, 2, 3, 4, 5},
            {6, 7, 8, 9, 10},
            {11, 12, 13, 14, 15},
            {16, 17, 18, 19, 20},
            {21, 22, 23, 24, 25},
            {26, 27, 28, 29, 30},
            {31, 32, 33, 34, 35},
            {36, 37, 38, 39, 40}
        };
        int tempB[rowsB][colsB] = {
            {1, 2, 3},
            {4, 5, 6},
            {7, 8, 9},
            {10, 11, 12},
            {13, 14, 15}
        };

        std::copy(&tempA[0][0], &tempA[0][0] + rowsA * colsA, &A[0][0]);
        std::copy(&tempB[0][0], &tempB[0][0] + rowsB * colsB, &B[0][0]);
    }

    int localA[2][colsA];
    int localB[colsA][colsB];
    int localC[2][colsB] = { {0} };

    if (rank == ROOT) {
        for (int i = 0; i < 4; i++) {
            if (i == ROOT) {
                std::copy(&A[i * 2][0], &A[i * 2][0] + 2 * colsA, &localA[0][0]);
            }
            else {
                MPI_Send(&A[i * 2][0], 2 * colsA, MPI_INT, i, 0, MPI_COMM_WORLD);
            }
        }
    }
    else {
        MPI_Recv(localA, 2 * colsA, MPI_INT, ROOT, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    MPI_Bcast(B, rowsB * colsB, MPI_INT, ROOT, MPI_COMM_WORLD);

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < colsB; j++) {
            localC[i][j] = 0;
            for (int k = 0; k < colsA; k++) {
                localC[i][j] += localA[i][k] * B[k][j];
            }
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);

    int C[rowsA][colsB];
    MPI_Gather(localC, 2 * colsB, MPI_INT, C, 2 * colsB, MPI_INT, ROOT, MPI_COMM_WORLD);

    if (rank == ROOT) {
        std::cout << "Result matrix:" << std::endl;
        for (int i = 0; i < rowsA; i++) {
            for (int j = 0; j < colsB; j++) {
                std::cout << C[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }

    MPI_Finalize();
    return 0;
}

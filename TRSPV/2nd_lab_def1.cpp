#include <mpi.h>
#include <iostream>
#include <vector>

const int ROOT = 0;
const int blockSize = 5;
const int globalSize = 9;

static int lab_2nd(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    MPI_Comm myComm; // интра-коммуникатор локальной подгруппы
    MPI_Comm myFirstComm; // интер-коммуникатор
    MPI_Comm mySecondComm; // второй интер-коммуникатор (группа 1 только)


    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 10) {
        if (rank == ROOT) {
            std::cerr << "This program requires exactly 10 processes." << std::endl;
        }
        MPI_Finalize();
        return 1;
    }

    int membershipKey = rank % 2;
    // Построение интракоммуникатора для локальной подгруппы
    MPI_Comm_split(MPI_COMM_WORLD, membershipKey, rank, &myComm);
    // Построение интер-коммуникатора. Метки жестко закодированные
    if (rank == ROOT) {
        // Группа 0 связывается с группой 1
        MPI_Intercomm_create(myComm, 0, MPI_COMM_WORLD, 0, 1, &myFirstComm);
        MPI_Intercomm_create(myComm, 0, MPI_COMM_WORLD, 2, 12, &mySecondComm);


    }
    


    if (rank == ROOT) {
        int A[globalSize][blockSize][blockSize] = {};
        for (int i = 0; i < globalSize; i++) {
            for (int j = 0; j < blockSize; j++) {
                for (int n = 0; n < blockSize; n++) {
                    A[i][j][n] = rand();
                }
            }
        }


    }

    /*MPI_Scatter(A, colsA * 2, MPI_INT, localA, colsA * 2, MPI_INT, ROOT, MPI_COMM_WORLD);

    MPI_Bcast(B, rowsB * colsB, MPI_INT, ROOT, MPI_COMM_WORLD);

    MPI_Gather(localC, 2 * colsB, MPI_INT, C, 2 * colsB, MPI_INT, ROOT, MPI_COMM_WORLD);

    if (rank == ROOT) {
        std::cout << "Result matrix:" << std::endl;
        for (int i = 0; i < rowsA; i++) {
            for (int j = 0; j < colsB; j++) {
                std::cout << C[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }*/

    MPI_Finalize();
    return 0;
}

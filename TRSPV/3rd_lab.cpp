#include <mpi.h>
#include <iostream>
#include <cstdlib>

const int MS3 = 4;
using namespace std;

static int* getCol(int matrix[MS3][MS3], int coln) {
    static int col[MS3]; // Используем static для возврата корректного указателя

    for (int i = 0; i < MS3; i++) {
        col[i] = matrix[i][coln];
    }

    return col;
}

static int* getRow(int matrix[MS3][MS3], int rown) {
    return matrix[rown];
}

static int multiply(int col[MS3], int row[MS3]) {
    int summ = 0;

    for (int i = 0; i < MS3; i++) {
        summ += col[i] * row[i];
    }

    return summ;
}

static int lab_3rd(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != MS3 * MS3) {
        if (rank == 0) {
            cerr << "This program requires exactly " << MS3 * MS3 << " processes." << endl;
        }
        MPI_Finalize();
        return 1;
    }

    int matrix[MS3][MS3];

    if (rank == 0) {
        cout << "Start matrix:" << endl;
        for (int i = 0; i < MS3; i++) {
            for (int j = 0; j < MS3; j++) {
                matrix[i][j] = rand() % 100;
                cout << matrix[i][j] << " ";
            }
            cout << endl;
        }
    }

    MPI_Bcast(matrix, MS3 * MS3, MPI_INT, 0, MPI_COMM_WORLD);

    const int ndims = 2;
    int dims[ndims] = { MS3, MS3 };
    int periods[ndims] = { 0, 0 };
    int reorder = 0;
    MPI_Comm cart_comm;

    MPI_Cart_create(MPI_COMM_WORLD, ndims, dims, periods, reorder, &cart_comm);

    int coords[ndims];
    int cart_rank;
    MPI_Cart_get(cart_comm, ndims, dims, periods, coords);
    MPI_Cart_rank(cart_comm, coords, &cart_rank);

    int local_result = multiply(getCol(matrix, coords[0]), getRow(matrix, coords[1]));
    MPI_Barrier(cart_comm);

    cout << "Local result: " << local_result << "    " << rank << endl;

    int result[MS3 * MS3];
    MPI_Allgather(&local_result, 1, MPI_INT, result, 1, MPI_INT, cart_comm);

    if (cart_rank == 0) {
        cout << "Result matrix:" << endl;
        for (int i = 0; i < MS3; i++) {
            for (int j = 0; j < MS3; j++) {
                cout << result[i * MS3 + j] << " ";
            }
            cout << endl;
        }
    }

    MPI_Finalize();
    return 0;
}

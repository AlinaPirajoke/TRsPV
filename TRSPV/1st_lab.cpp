#include <mpi.h>
#include <iostream>
#include <vector>

using namespace std;
const int N = 4;

static double diagonalProduct(double matrix[3][3], int i) {
    double product = 1.0;
    if (i < 3) {
        for (int j = 0; j < 3; ++j) product *= matrix[j][(i + j) % 3];
    }
    else {
        for (int j = 0; j < 3; ++j) product *= matrix[2 - j][(i + j) % 3];
        product *= -1;
    }
    return product;
}

static void printMatrix(double matrix[N - 1][N - 1]) {
    for (int i = 0; i < N - 1; i++) {
        for (int j = 0; j < N - 1; j++) {
            cout<< matrix[i][j]<<" ";
        }
        cout << "\n";
    }
    cout << endl;
}

static double getDeterminant(double m[3][3], double k) {
    double determinant = 0.0;

    for (int i = 0; i < 6; i++) {
        determinant += diagonalProduct(m, i);
    }
    determinant *= k;
    return determinant;
}

static void getSubmatrix(double matrix[N][N], int i, int j, double submatrix[N - 1][N - 1]) {
    int row = 0, col = 0;

    for (int m = 0; m < N; m++) {
        if (m == i) continue;

        col = 0;
        for (int n = 0; n < N; n++) {
            if (n == j) continue;

            submatrix[row][col] = matrix[m][n];
            col++;
        }
        row++;
    }
}

static void printReady(int rank) {
    cout << "Procces "<<rank<< " ready" << endl;
}

static void printRecived(int rank, double matrix[N - 1][N - 1]) {
    cout << "Procces " << rank << " recived matrix:\n";
    printMatrix(matrix);
}

static int lab_1st(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 5) {
        if (rank == 0) {
            cout << "This program requires 5 processes.\n";
        }
        MPI_Finalize();
        return 1;
    }

    double matrix[4][4] = {
        {1, 2, 3, 4},
        {5, 7, 7, 8},
        {9, 9, 11, 12},
        {13, 14, 15, 11}
    };
    double subMatrix[3][3] = {};
    double localResult = 0.0;

    switch (rank) {
        case 0: {
            for (int i = 0; i < size-1; i++) {
                //getSubmatrix(matrix, 0, i, subMatrix);
                int j = 0;
                int row = 0, col = 0;

                for (int m = 0; m < N; m++) {
                    if (m == i) continue;

                    col = 0;
                    for (int n = 0; n < N; n++) {
                        if (n == j) continue;

                        subMatrix[row][col] = matrix[m][n];
                        col++;
                    }
                    row++;
                }
                
                MPI_Send(subMatrix, 9, MPI_DOUBLE, i+1, 123, MPI_COMM_WORLD);
            
            }
            break;
        }
        case 1: {
            double localMatrix[3][3] = {};
            double k = matrix[0][0];
            MPI_Status status;
            MPI_Recv(localMatrix, 9, MPI_DOUBLE, 0, 123, MPI_COMM_WORLD, &status);
            printRecived(rank, localMatrix);
            localResult = getDeterminant(localMatrix, k);
            break;
        }
        case 2: {
            double localMatrix[3][3] = {};
            double k = matrix[0][1];
            MPI_Status status;
            MPI_Recv(localMatrix, 9, MPI_DOUBLE, 0, 123, MPI_COMM_WORLD, &status);
            printRecived(rank, localMatrix);
            localResult = getDeterminant(localMatrix, k);
            break;
        }
        case 3: {
            double localMatrix[3][3] = {};
            double k = matrix[0][2];
            MPI_Status status;
            MPI_Recv(localMatrix, 9, MPI_DOUBLE, 0, 123, MPI_COMM_WORLD, &status);
            printRecived(rank, localMatrix);
            localResult = getDeterminant(localMatrix, k);
            break;
        }
        case 4: {
            double localMatrix[3][3] = {};
            double k = matrix[0][3];
            MPI_Status status;
            MPI_Recv(localMatrix, 9, MPI_DOUBLE, 0, 123, MPI_COMM_WORLD, &status);
            printRecived(rank, localMatrix);
            localResult = getDeterminant(localMatrix, k);
            break;
        }
    }

    double globalResults[5] = { 0 };
    MPI_Gather(&localResult, 1, MPI_DOUBLE, globalResults, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        double determinant = globalResults[1] + globalResults[2] + globalResults[3] + globalResults[4];
        cout << "Determinant submatrix 1: " << globalResults[1] << endl;
        cout << "Determinant submatrix 2: " << globalResults[2] << endl;
        cout << "Determinant submatrix 3: " << globalResults[3] << endl;
        cout << "Determinant submatrix 4: " << globalResults[4] << endl;
        cout << "Determinant: " << determinant << endl;
    }

    MPI_Finalize();
    return 1;
}
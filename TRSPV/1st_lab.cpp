#include <mpi.h>
#include <iostream>
#include <vector>

using namespace std;

// Функция для вычисления произведения элементов в одной из диагоналей
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

//  Я называю это кибер преступностью
static double getDeterminant1(double m[4][4]) {
    double k = m[0][0];
    double determinant = 0.0;
    double newMatrix[3][3] = {
            {m[1][1], m[1][2], m[1][3]},
            {m[2][1], m[2][2], m[2][3]},
            {m[3][1], m[3][2], m[3][3]}
    };

    for (int i = 0; i < 6; i++) {
        determinant += diagonalProduct(newMatrix, i);
    }
    determinant *= k;
    return determinant;
}

static double getDeterminant2(double m[4][4]) {
    double k = m[0][1];
    double determinant = 0.0;
    double newMatrix[3][3] = {
            {m[1][0], m[1][2], m[1][3]},
            {m[2][0], m[2][2], m[2][3]},
            {m[3][0], m[3][2], m[3][3]}
    };

    for (int i = 0; i < 6; i++) {
        determinant += diagonalProduct(newMatrix, i);
    }
    return determinant *= -1 * k;
}

static double getDeterminant3(double m[4][4]) {
    double k = m[0][2];
    double determinant = 0.0;
    double newMatrix[3][3] = {
            {m[1][0], m[1][1], m[1][3]},
            {m[2][0], m[2][1], m[2][3]},
            {m[3][0], m[3][1], m[3][3]}
    };

    for (int i = 0; i < 6; i++) {
        determinant += diagonalProduct(newMatrix, i);
    }
    determinant *= k;
    return determinant;
}

static double getDeterminant4(double m[4][4]) {
    double k = m[0][0];
    double determinant = 0.0;
    double newMatrix[3][3] = {
            {m[1][0], m[1][1], m[1][2]},
            {m[2][0], m[2][1], m[2][2]},
            {m[3][0], m[3][1], m[3][2]}
    };

    for (int i = 0; i < 6; i++) {
        determinant += diagonalProduct(newMatrix, i);
    }
    determinant *= k;
    return determinant;
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

    // Исходная матрица 4x4
    double matrix[4][4] = {
        {1, 2, 3, 4},
        {5, 7, 7, 8},
        {9, 9, 11, 12},
        {13, 14, 15, 11}
    };

    double localResult = 0.0;

    // Процессы с рангом от 1 до 8 вычисляют свои произведения
    switch (rank) {
        case 1: {
            localResult = getDeterminant1(matrix);
            break;
        }
        case 2: {
            localResult = getDeterminant2(matrix);
            break;
        }
        case 3: {
            localResult = getDeterminant3(matrix);
            break;
        }
        case 4: {
            localResult = getDeterminant4(matrix);
            break;
        }
    }

    // Главный процесс собирает результаты от остальных процессов
    double globalResults[5] = { 0 };
    MPI_Gather(&localResult, 1, MPI_DOUBLE, globalResults, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Главный процесс вычисляет итоговый определитель
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
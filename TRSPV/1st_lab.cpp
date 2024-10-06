#include <mpi.h>
#include <iostream>
#include <vector>

using namespace std;

// Функция для вычисления произведения элементов в одной из диагоналей
static double diagonal_product(double matrix[4][4], int diag) {
    double product = 1.0;
    switch (diag) {
    case 0:  // Главная диагональ
        for (int i = 0; i < 4; ++i) product *= matrix[i][i];
        break;
    case 1:  // Первая побочная диагональ
        for (int i = 0; i < 4; ++i) product *= matrix[i][(i + 1) % 4];
        break;
    case 2:  // Вторая побочная диагональ
        for (int i = 0; i < 4; ++i) product *= matrix[i][(i + 2) % 4];
        break;
    case 3:  // Третья побочная диагональ
        for (int i = 0; i < 4; ++i) product *= matrix[i][(i + 3) % 4];
        break;
    case 4:  // Главная диагональ с отрицательными элементами
        for (int i = 0; i < 4; ++i) product *= matrix[3 - i][i];
        break;
    case 5:  // Первая побочная диагональ с отрицательными элементами
        for (int i = 0; i < 4; ++i) product *= matrix[3 - i][(i + 1) % 4];
        break;
    case 6:  // Вторая побочная диагональ с отрицательными элементами
        for (int i = 0; i < 4; ++i) product *= matrix[3 - i][(i + 2) % 4];
        break;
    case 7:  // Третья побочная диагональ с отрицательными элементами
        for (int i = 0; i < 4; ++i) product *= matrix[3 - i][(i + 3) % 4];
        break;
    }
    cout << "Product " << diag << ": " << product << endl;
    return product;
}

static int lab_1st(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 9) {
        if (rank == 0) {
            cout << "This program requires 9 processes.\n";
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

    double local_result = 0.0;

    // Процессы с рангом от 1 до 8 вычисляют свои произведения
    if (rank > 0 && rank <= 8) {
        local_result = diagonal_product(matrix, rank - 1);
    }

    // Главный процесс собирает результаты от остальных процессов
    double global_results[9] = { 0 };
    MPI_Gather(&local_result, 1, MPI_DOUBLE, global_results, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Главный процесс вычисляет итоговый определитель
    if (rank == 0) {
        double determinant = 0.0;
        determinant = (global_results[1] + global_results[2] + global_results[3] + global_results[4]) -
            (global_results[5] + global_results[6] + global_results[7] + global_results[8]);
        cout << "first " << (global_results[0] + global_results[1] + global_results[2] + global_results[3]) << endl;
        cout << "second " << (global_results[4] + global_results[5] + global_results[6] + global_results[7]) << endl;
        cout << "Determinant: " << determinant << endl;
    }

    MPI_Finalize();
    return 1;
}
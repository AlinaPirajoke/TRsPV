#include <mpi.h>
#include <iostream>
#include <vector>
#include <string>
#include <windows.h>
#include "utils.cpp"
using namespace std;

static string getN(int rank) {
    return "[" + to_string(rank) + "] :";
}

const int PROCESSES_102 = 4;
const int MAX_STEPS_102 = 50;
const int STEP_TIME_102 = 100;

static void processJobN(int n) {
    int secret = n;
    int p = n;
    int known[PROCESSES_102] = { secret };

    bool recived[PROCESSES_102] = { false };
    bool recivInit[PROCESSES_102] = { false };
    int recivFlag[PROCESSES_102] = { 0 };
    MPI_Request rReq[PROCESSES_102];

    for (int n = 0; n < PROCESSES_102; n++) {
        known[n] = secret;
        recived[n] = false;
        recivInit[n] = false;
        recivFlag[n] = 0;
        rReq[n] = *(new MPI_Request);
    }

    recived[p] = true;

    int steps = 0;
    bool c = true;
    cout << "Started " << endl;

    for (int i = 0; i < PROCESSES_102; i++) {
        if (i == p) continue;
        MPI_Send(&secret, 1, MPI_INT, i, 123, MPI_COMM_WORLD);
    }

    while (c) {
        c = false;
        for (int i = 0; i < PROCESSES_102; i++) {
            if (i == p) continue;

            if (!recived[i]) {
                MPI_Status status;
                MPI_Iprobe(i, 123, MPI_COMM_WORLD, &recivFlag[i], &status);
                cout << getN(p) << "Pinged " << i << endl;

                if (recivFlag[i]) {
                    recived[i] = true;
                    MPI_Irecv(&known[i], 1, MPI_INT, i, 123, MPI_COMM_WORLD, &rReq[i]);
                    cout << getN(p) << "Recived " << known[i] << " from " << i << endl;
                }
            }
            c = c || !recived[i];
        }

        if (++steps >= MAX_STEPS_102) {
            break;
        }
        Sleep(STEP_TIME_102);
    }

    std::cout << getN(p) << "recived numbers: ";
    for (int n = 0; n < PROCESSES_102; n++) {
        std::cout << known[n] << ", ";
    }
    std::cout << "\n";
}

static void processJob0(int n) {
    int number;
    MPI_Status status;
    MPI_Recv(&number, 1, MPI_INT, 0, 123, MPI_COMM_WORLD, &status);
    cout << getN(n) << "Recived " << number << endl;
    MPI_Send(&n, 1, MPI_INT, 0, 123, MPI_COMM_WORLD);
}

static int lab_1st_def_2(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);


    if (size != PROCESSES_102) {
        if (rank == 0) {
            std::cout << "This program requires 4 processes.\n";
        }
        MPI_Finalize();
        return 1;
    }

    processJobN(rank);

    MPI_Finalize();
    return 1;
}
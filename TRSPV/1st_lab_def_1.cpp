#include <mpi.h>
#include <iostream>
#include <vector>
#include <string>
#include <windows.h>
using namespace std;

const int PROCESSES = 4;
const int MAX_STEPS = 50;
const int STEP_TIME = 100;

static string getN(int rank) {
    return "[" + to_string(rank) + "] :";
}

static void processJob1() {
    int p = 0;
    int secret = 0;
    int known[PROCESSES] = { secret };

    bool recived[PROCESSES] = { false };
    bool recivInit[PROCESSES] = { false };
    int recivFlag[PROCESSES] = { 0 };
    MPI_Request rReq[PROCESSES];

    bool sended[PROCESSES] = { false };
    MPI_Request sReq[PROCESSES];

    for (int n = 0; n < PROCESSES; n++) {
        known[n] = secret;
        recived[n] = false;
        recivInit[n] = false;
        recivFlag[n] = 0;
        sended[n] = false;
        rReq[n] = *(new MPI_Request);
        sReq[n] = *(new MPI_Request);
    }

    recived[p] = true;
    sended[p] = true;

    int steps = 0;
    bool c = true;
    cout << getN(p) << "Started " << endl;
    while (c) {
        c = false;
        for (int i = 0; i < PROCESSES; i++) {
            if (i == p) continue;

            if (!recived[i]) {
                if (!recivInit[i]) {
                    MPI_Irecv(&known[i], 1, MPI_INT, i, 123, MPI_COMM_WORLD, &rReq[i]);
                    recivInit[i] = true;
                    cout << getN(p) << "Registered reciver to " << i << endl;
                }
                MPI_Status status;
                if (recivFlag[i]) {
                    recived[i] = true;
                    cout << getN(p) << "Recived " << known[i] << " from " << i << endl;
                }
                else {
                    MPI_Test(&rReq[i], &recivFlag[i], &status);
                    cout << getN(p) << "Pinged " << i << endl;
                }
            }

            if (!sended[i]) {
                MPI_Send(&secret, 1, MPI_INT, i, 123, MPI_COMM_WORLD);
                sended[i] = true;
                cout << getN(p) << "Sended " << secret << " to " << i << endl;
            }

            c = c || !recived[i] || !sended[i];
        }

        if (++steps >= MAX_STEPS) {
            break;
        }
        Sleep(STEP_TIME);
    }

    std::cout << getN(p) << "recived numbers: ";
    for (int n = 0; n < PROCESSES; n++) {
        std::cout << known[n] << ", ";
    }
    std::cout << "\n";
}

static void processJob2() {
    int p = 1;
    int secret = 1;
    int known[PROCESSES] = { secret };

    bool recived[PROCESSES] = { false };
    bool recivInit[PROCESSES] = { false };
    int recivFlag[PROCESSES] = { 0 };
    MPI_Request rReq[PROCESSES];

    bool sended[PROCESSES] = { false };
    MPI_Request sReq[PROCESSES];

    for (int n = 0; n < PROCESSES; n++) {
        known[n] = secret;
        recived[n] = false;
        recivInit[n] = false;
        recivFlag[n] = 0;
        sended[n] = false;
        rReq[n] = *(new MPI_Request);
        sReq[n] = *(new MPI_Request);
    }

    recived[p] = true;
    sended[p] = true;

    int steps = 0;
    bool c = true;
    cout << getN(p) << "Started " << endl;
    while (c) {
        c = false;
        for (int i = 0; i < PROCESSES; i++) {
            if (i == p) continue;

            if (!recived[i]) {
                if (!recivInit[i]) {
                    MPI_Irecv(&known[i], 1, MPI_INT, i, 123, MPI_COMM_WORLD, &rReq[i]);
                    recivInit[i] = true;
                    cout << getN(p) << "Registered reciver to " << i << endl;
                }
                MPI_Status status;
                if (recivFlag[i]) {
                    recived[i] = true;
                    cout << getN(p) << "Recived " << known[i] << " from " << i << endl;
                }
                else {
                    MPI_Test(&rReq[i], &recivFlag[i], &status);
                    cout << getN(p) << "Pinged " << i << endl;
                }
            }

            if (!sended[i]) {
                MPI_Send(&secret, 1, MPI_INT, i, 123, MPI_COMM_WORLD);
                sended[i] = true;
                cout << getN(p) << "Sended " << secret << " to " << i << endl;
            }

            c = c || !recived[i] || !sended[i];
        }

        if (++steps >= MAX_STEPS) {
            break;
        }
        Sleep(STEP_TIME);
    }

    std::cout << getN(p) << "recived numbers: ";
    for (int n = 0; n < PROCESSES; n++) {
        std::cout << known[n] << ", ";
    }
    std::cout << "\n";
}

static void processJob3() {
    int p = 2;
    int secret = 2;
    int known[PROCESSES] = { secret };

    bool recived[PROCESSES] = { false };
    bool recivInit[PROCESSES] = { false };
    int recivFlag[PROCESSES] = { 0 };
    MPI_Request rReq[PROCESSES] = { };

    bool sended[PROCESSES] = { false };
    MPI_Request sReq[PROCESSES] = {};

    for (int n = 0; n < PROCESSES; n++) {
        known[n] = secret;
        recived[n] = false;
        recivInit[n] = false;
        recivFlag[n] = 0;
        sended[n] = false;
        rReq[n] = *(new MPI_Request);
        sReq[n] = *(new MPI_Request);
    }

    recived[p] = true;
    sended[p] = true;

    int steps = 0;
    bool c = true;
    cout << getN(p) << "Started " << endl;
    while (c) {
        c = false;
        for (int i = 0; i < PROCESSES; i++) {
            if (i == p) continue;

            if (!recived[i]) {
                if (!recivInit[i]) {
                    MPI_Irecv(&known[i], 1, MPI_INT, i, 123, MPI_COMM_WORLD, &rReq[i]);
                    recivInit[i] = true;
                    cout << getN(p) << "Registered reciver to " << i << endl;
                }
                MPI_Status status;
                if (recivFlag[i]) {
                    recived[i] = true;
                    cout << getN(p) << "Recived " << known[i] << " from " << i << endl;
                }
                else {
                    MPI_Test(&rReq[i], &recivFlag[i], &status);
                    cout << getN(p) << "Pinged " << i << endl;
                }
            }

            if (!sended[i]) {
                MPI_Send(&secret, 1, MPI_INT, i, 123, MPI_COMM_WORLD);
                sended[i] = true;
                cout << getN(p) << "Sended " << secret << " to " << i << endl;
            }

            c = c || !recived[i] || !sended[i];
        }

        if (++steps >= MAX_STEPS) {
            break;
        }
        Sleep(STEP_TIME);
    }

    std::cout << getN(p) << "recived numbers: ";
    for (int n = 0; n < PROCESSES; n++) {
        std::cout << known[n] << ", ";
    }
    std::cout << "\n";
}

static void processJob4() {
    int p = 3;
    int secret = 3;
    int known[PROCESSES] = { secret };

    bool recived[PROCESSES] = { false };
    bool recivInit[PROCESSES] = { false };
    int recivFlag[PROCESSES] = { 0 };
    MPI_Request rReq[PROCESSES];

    bool sended[PROCESSES] = { false };
    MPI_Request sReq[PROCESSES];

    for (int n = 0; n < PROCESSES; n++) {
        known[n] = secret;
        recived[n] = false;
        recivInit[n] = false;
        recivFlag[n] = 0;
        sended[n] = false;
        rReq[n] = *(new MPI_Request);
        sReq[n] = *(new MPI_Request);
    }

    recived[p] = true;
    sended[p] = true;

    int steps = 0;
    bool c = true;
    cout << getN(p) << "Started " << endl;
    while (c) {
        c = false;
        for (int i = 0; i < PROCESSES; i++) {
            if (i == p) continue;

            if (!recived[i]) {
                if (!recivInit[i]) {
                    MPI_Irecv(&known[i], 1, MPI_INT, i, 123, MPI_COMM_WORLD, &rReq[i]);
                    recivInit[i] = true;
                    cout << getN(p) << "Registered reciver to " << i << endl;
                }
                MPI_Status status;
                if (recivFlag[i]) {
                    recived[i] = true;
                    cout << getN(p) << "Recived " << known[i] << " from " << i << endl;
                }
                else {
                    MPI_Test(&rReq[i], &recivFlag[i], &status);
                    cout << getN(p) << "Pinged " << i << endl;
                }
            }

            if (!sended[i]) {
                MPI_Send(&secret, 1, MPI_INT, i, 123, MPI_COMM_WORLD);
                sended[i] = true;
                cout << getN(p) << "Sended " << secret << " to " << i << endl;
            }

            c = c || !recived[i] || !sended[i];
        }

        if (++steps >= MAX_STEPS) {
            break;
        }
        Sleep(STEP_TIME);
    }

    std::cout << getN(p) << "recived numbers: ";
    for (int n = 0; n < PROCESSES; n++) {
        std::cout << known[n] << ", ";
    }
    std::cout << "\n";
}

static int lab_1st_def_1(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);


    if (size != PROCESSES) {
        if (rank == 0) {
            std::cout << "This program requires 4 processes.\n";
        }
        MPI_Finalize();
        return 1;
    }

    switch (rank) {
    case 0: {
        processJob1();
        break;
    }
    case 1: {
        processJob2();
        break;
    }
    case 2: {
        processJob3();
        break;
    }
    case 3: {
        processJob4();
        break;
    }
    }

    MPI_Finalize();
    return 1;
}
#include <mpi.h>
#include <iostream>
#include <vector>

const int PROCESSES = 4;

static void processJob1() {
    int p = 0;
    int secret = 1;
    int known[PROCESSES] = { secret };

    bool recived[PROCESSES] = { false };
    bool recivInit[PROCESSES] = { false };
    int recivFlag[PROCESSES] = { 0 };
    MPI_Request rReq[PROCESSES];

    bool sended[PROCESSES] = { false };
    MPI_Request sReq[PROCESSES];

    recived[p] = true;
    sended[p] = true;

    bool c = false;

    for (int j = 0; j < PROCESSES; j++) {
        MPI_Request rr;
        MPI_Request sr;
        rReq[j] = rr;
        sReq[j] = sr;
    }

    while (c) {
        for (int i = 0; i < PROCESSES; i++) {
            if (i = p) continue;

            if (!sended[i]) {
                MPI_Isend(&secret, 1, MPI_INT, p, i, MPI_COMM_WORLD, &sReq[i]);
                sended[i] = true;
            }

            if (!recived[i]) {
                if (!recivInit[i]) {
                    MPI_Irecv(&known[i], 1, MPI_INT, i, p, MPI_COMM_WORLD, &rReq[i]);
                    recivInit[i] = true;
                }
                MPI_Status status;
                if (&recivFlag[i]) {
                    recived[i] = true;
                }
                else {
                    MPI_Test(&rReq[i], &recivFlag[i], &status);
                }
            }

            c = !recived[i] || !sended[i];
        }
    }
    std::cout << "process number: " << secret << "\n";
    std::cout << "recived numbers: ";
    for (int n = 0; n < PROCESSES; n++) {
        std::cout << known[n] << ", ";
    }
    std::cout << "\n";
}

static void processJob2() {
    int p = 1;
    int secret = 2;
    int known[PROCESSES] = { secret };

    bool recived[PROCESSES] = { false };
    bool recivInit[PROCESSES] = { false };
    int recivFlag[PROCESSES] = { 0 };
    MPI_Request rReq[PROCESSES];

    bool sended[PROCESSES] = { false };
    MPI_Request sReq[PROCESSES];

    recived[p] = true;
    sended[p] = true;

    bool c = false;

    for (int j = 0; j < PROCESSES; j++) {
        MPI_Request rr;
        MPI_Request sr;
        rReq[j] = rr;
        sReq[j] = sr;
    }

    while (c) {
        for (int i = 0; i < PROCESSES; i++) {
            if (i = p) continue;

            if (!sended[i]) {
                MPI_Isend(&secret, 1, MPI_INT, p, i, MPI_COMM_WORLD, &sReq[i]);
                sended[i] = true;
            }

            if (!recived[i]) {
                if (!recivInit[i]) {
                    MPI_Irecv(&known[i], 1, MPI_INT, i, p, MPI_COMM_WORLD, &rReq[i]);
                    recivInit[i] = true;
                }
                MPI_Status status;
                if (&recivFlag[i]) {
                    recived[i] = true;
                }
                else {
                    MPI_Test(&rReq[i], &recivFlag[i], &status);
                }
            }

            c = !recived[i] || !sended[i];
        }
    }
    std::cout << "process number: " << secret << "\n";
    std::cout << "recived numbers: ";
    for (int n = 0; n < PROCESSES; n++) {
        std::cout << known[n] << ", ";
    }
    std::cout << "\n";
}

static void processJob3() {
    int p = 2;
    int secret = 3;
    int known[PROCESSES] = { secret };

    bool recived[PROCESSES] = { false };
    bool recivInit[PROCESSES] = { false };
    int recivFlag[PROCESSES] = { 0 };
    MPI_Request rReq[PROCESSES];

    bool sended[PROCESSES] = { false };
    MPI_Request sReq[PROCESSES];

    recived[p] = true;
    sended[p] = true;

    bool c = false;

    for (int j = 0; j < PROCESSES; j++) {
        MPI_Request rr;
        MPI_Request sr;
        rReq[j] = rr;
        sReq[j] = sr;
    }

    while (c) {
        for (int i = 0; i < PROCESSES; i++) {
            if (i = p) continue;

            if (!sended[i]) {
                MPI_Isend(&secret, 1, MPI_INT, p, i, MPI_COMM_WORLD, &sReq[i]);
                sended[i] = true;
            }

            if (!recived[i]) {
                if (!recivInit[i]) {
                    MPI_Irecv(&known[i], 1, MPI_INT, i, p, MPI_COMM_WORLD, &rReq[i]);
                    recivInit[i] = true;
                }
                MPI_Status status;
                if (&recivFlag[i]) {
                    recived[i] = true;
                }
                else {
                    MPI_Test(&rReq[i], &recivFlag[i], &status);
                }
            }

            c = !recived[i] || !sended[i];
        }
    }
    std::cout << "process number: " << secret << "\n";
    std::cout << "recived numbers: ";
    for (int n = 0; n < PROCESSES; n++) {
        std::cout << known[n] << ", ";
    }
    std::cout << "\n";
}

static void processJob4() {
    int p = 3;
    int secret = 4;
    int known[PROCESSES] = { secret };

    bool recived[PROCESSES] = { false };
    bool recivInit[PROCESSES] = { false };
    int recivFlag[PROCESSES] = { 0 };
    MPI_Request rReq[PROCESSES];

    bool sended[PROCESSES] = { false };
    MPI_Request sReq[PROCESSES];

    recived[p] = true;
    sended[p] = true;

    bool c = false;

    for (int j = 0; j < PROCESSES; j++) {
        MPI_Request rr;
        MPI_Request sr;
        rReq[j] = rr;
        sReq[j] = sr;
    }

    while (c) {
        for (int i = 0; i < PROCESSES; i++) {
            if (i = p) continue;

            if (!sended[i]) {
                MPI_Isend(&secret, 1, MPI_INT, p, i, MPI_COMM_WORLD, &sReq[i]);
                sended[i] = true;
            }

            if (!recived[i]) {
                if (!recivInit[i]) {
                    MPI_Irecv(&known[i], 1, MPI_INT, i, p, MPI_COMM_WORLD, &rReq[i]);
                    recivInit[i] = true;
                }
                MPI_Status status;
                if (&recivFlag[i]) {
                    recived[i] = true;
                }
                else {
                    MPI_Test(&rReq[i], &recivFlag[i], &status);
                }
            }

            c = !recived[i] || !sended[i];
        }
    }
    std::cout << "process number: " << secret << "\n";
    std::cout << "recived numbers: ";
    for (int n = 0; n < PROCESSES; n++) {
        std::cout << known[n] << ", ";
    }
    std::cout << "\n";
}

static int lab_1st_def1(int argc, char** argv) {
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
#include <mpi.h>
#include <iostream>
#include <vector>
#include <cstring>

using namespace std;

const int ZOND = 763905;
const int ECHO = 662891;
const int EMPTY_RESPONSE = -1;

static void print_matrix(const vector<vector<int>>& matrix) {
    for (const auto& row : matrix) {
        for (int val : row) {
            cout << val << " ";
        }
        cout << endl;
    }
}

static void log(int id, string msg) {
    cout << "[" << id << "] " << msg << endl;
 }

static int lab_5th(int argc, char** argv) {
    vector<vector<int>> topology = {
        {1, 3},      // Узел 0 (Root)
        {0, 2, 4},   // Узел 1
        {1, 3},      // Узел 2
        {0, 2, 4},   // Узел 3
        {1, 3},      // Узел 4
    };

    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != topology.size()) {
        if (rank == 0) {
            cerr << "required " << topology.size() << " processes." << endl;
        }
        MPI_Finalize();
        return 1;
    }

    char message[256];
    vector<vector<int>> received_topology(size, vector<int>(size, 0));

    if (rank == 0) {
        cout << "input message: ";
        cin.getline(message, 256);

        cout << "start topology:" << endl;
        print_matrix(topology);
    }

    int parent = -1;
    vector<int> children;
    vector<vector<int>> local_topology(size, vector<int>(size, 0));
    MPI_Status status;

    if (rank != 0) {
        int source;
        MPI_Recv(&source, 1, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &status);
        log(rank, "recived zond");
        parent = source;
    }

    for (int neighbor : topology[rank]) {
        if (neighbor != parent) {
            MPI_Send(&rank, 1, MPI_INT, neighbor, 1, MPI_COMM_WORLD);
        }
    }

    log(rank, "sended zonds");
    MPI_Barrier(MPI_COMM_WORLD);

    do {
        log(rank, "try probe");
        MPI_Probe(MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &status);
        log(rank, "end probe");
        
        if (status.MPI_ERROR == 0) {
            log(rank, "send empty responce");
            MPI_Send(&EMPTY_RESPONSE, 1, MPI_INT, status.MPI_SOURCE, 2, MPI_COMM_WORLD);
        }
    } while (status.MPI_ERROR == 0);

    for (int neighbor : topology[rank]) {
        if (neighbor != parent) {
            vector<vector<int>> child_topology(size, vector<int>(size, 0));
            MPI_Recv(&child_topology[0][0], size * size, MPI_INT, neighbor, 2, MPI_COMM_WORLD, &status);
            log(rank, "recived echo");
            children.push_back(neighbor);
            for (int i = 0; i < size; i++) {
                for (int j = 0; j < size; j++) {
                    local_topology[i][j] |= child_topology[i][j];
                }
            }
        }
    }

    if (parent != -1) {
        local_topology[parent][rank] = 1;
        local_topology[rank][parent] = 1;
    }

    if (parent != -1) {
        MPI_Send(&local_topology[0][0], size * size, MPI_INT, parent, 2, MPI_COMM_WORLD);
    }

    if (rank == 0) {
        print_matrix(local_topology);
    }

    if (rank == 0) {
        for (int child : children) {
            MPI_Send(message, 256, MPI_CHAR, child, 3, MPI_COMM_WORLD);
            MPI_Send(&local_topology[0][0], size * size, MPI_INT, child, 4, MPI_COMM_WORLD);
        }
    }
    else {
        MPI_Recv(message, 256, MPI_CHAR, parent, 3, MPI_COMM_WORLD, &status);
        MPI_Recv(&received_topology[0][0], size * size, MPI_INT, parent, 4, MPI_COMM_WORLD, &status);

        cout << "Процесс " << rank << " получил сообщение: " << message << endl;
        print_matrix(received_topology);

        for (int child_id = 0; child_id < size; child_id++) {
            if (received_topology[rank][child_id]) {
                MPI_Send(message, 256, MPI_CHAR, child_id, 3, MPI_COMM_WORLD);
                MPI_Send(&received_topology[0][0], size * size, MPI_INT, child_id, 4, MPI_COMM_WORLD);
            }
        }
    }

    MPI_Finalize();
    return 0;
}
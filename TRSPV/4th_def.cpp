#include <mpi.h>
#include <iostream>
#include <queue>
#include <stddef.h>

using namespace std;

const int SERVER_RANK = 0;
const int RESOURCES = 3;
MPI_Datatype request_type;

struct request {
    int client;
    int requested_resource;
    int value;
};

struct resource {
    int free = 1;
    queue<request> order = {};
};

static void server_job() {
    MPI_Status status;
    resource res[RESOURCES];

    while (1) {
        request incoming;
        MPI_Recv(&incoming, 1, request_type, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

        if (status.MPI_SOURCE <= RESOURCES) {// Ответ от ресурса
            MPI_Send(&incoming.value, 1, request_type, incoming.client, MPI_ANY_TAG, MPI_COMM_WORLD);

            res[incoming.requested_resource].order.pop();
            res[incoming.requested_resource].free = 1;
        }
        else { // Запрос от клиента
            res[incoming.requested_resource].order.push(incoming);
        }

        if (res[incoming.requested_resource].free && !res[incoming.requested_resource].order.empty()) {
            MPI_Send(&res[incoming.requested_resource].order.front(), 1, request_type, res[incoming.requested_resource].order.front().requested_resource, MPI_ANY_TAG, MPI_COMM_WORLD);
            res[incoming.requested_resource].free = 0;
        }
    }
}

static void client_job() {
    int rank, value;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    request req;
    req.client = rank;

    for(int i = 0; i < 5; i++) {
        req.value = (rand() / rank) % 10;
        req.requested_resource = req.value = (rand() / rank) % RESOURCES;
        cout<<rank<<": sended value: " << req.value
        MPI_Send(&req, 1, request_type, SERVER_RANK, MPI_ANY_TAG, MPI_COMM_WORLD);
        MPI_Send(&value, 1, MPI_INT, SERVER_RANK, MPI_ANY_TAG, MPI_COMM_WORLD);
    }
}

static void resource_job() {
    MPI_Status status;
    request req;
    while (1) {
        MPI_Recv(&req, 1, request_type, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        req.value *= 5;
        MPI_Send(&req, 1, request_type, SERVER_RANK, MPI_ANY_TAG, MPI_COMM_WORLD);
    }
}

static int lab_4th_def(int argc, char** argv) {

    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int nitems = 3;
    int          blocklengths[3] = { 1, 1, 1 };
    MPI_Datatype types[3] = { MPI_INT, MPI_INT, MPI_INT };
    MPI_Aint     offsets[3];

    offsets[0] = offsetof(request, client);
    offsets[1] = offsetof(request, requested_resource);
    offsets[2] = offsetof(request, value);


    MPI_Type_create_struct(nitems, blocklengths, offsets, types, &request_type);
    MPI_Type_commit(&request_type);

    if (size < 5) {
        if (rank == 0) {
            std::cerr << "This program requires more then 5 processes." << std::endl;
        }
        MPI_Finalize();
        return 1;
    }

    switch (rank) {
    case 0: {
        server_job();
        break;
    }
    case 1: {
        resource_job();
        break;
    }
    default: {
        client_job();
        break;
    }
    }
    return 1;
}
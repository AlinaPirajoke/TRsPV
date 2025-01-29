#include <mpi.h>
#include <vector>
#include <iterator>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string>

using namespace std;

class Request {
public:
    int requested_quantity;
    int process_id;

    Request(int requested_quantity, int process_id) {
        this->requested_quantity = requested_quantity;
        this->process_id = process_id;
    }
};

class Resource {
public:
    int id;
    bool free;
    int value;
    vector<Request> queue;

    Resource(int id, int value) {
        this->id = id;
        this->value;
        queue = {};
        free = 1;
    }
};

const int SERVER = 0;  // Ранк сервера
const int REQUEST = 1; // Тип сообщения: запрос ресурса
const int RELEASE = 2; // Тип сообщения: освобождение ресурса
const string S_MSG = " [SERVER] ";
const string C_MSG = "[CLIENT] ";

// Количество ресурсов
const int RESOURCE_COUNT = 3;

// Функция сервера
static void server(int world_size) {

    vector<Resource> storage = {};

    for (int i = 0; i < RESOURCE_COUNT; i++) {
        int value = rand() % 10;
        storage.push_back(Resource(i, value));
        cout << S_MSG << i << " resource start value: " << value << endl;
    }

    while (true) {
        MPI_Status status;
        int message[2];

        // Ожидаем запрос от клиентов
        MPI_Recv(&message, 2, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

        int client_rank = status.MPI_SOURCE;
        int request_type = status.MPI_TAG;
        int resource_id = message[0];
        int value = message[1];

        // Обработка запроса
        if (request_type == REQUEST) {
            Request request = Request(1, client_rank);
            storage[resource_id].queue.push_back(request);
            cout << S_MSG << "A request for resource "<< resource_id <<" is received from process " << client_rank << endl;
        }
        else if (request_type == RELEASE) {
            storage[resource_id].value = value;
            storage[resource_id].free = true;
            cout << S_MSG << "A release for resource " << resource_id << " is received from process " << client_rank << "; New value: " << value << endl;
        }

        // Ответ на запросс
        for (int s = 0; s < RESOURCE_COUNT; s++) {;
            if (storage[s].free && storage[s].queue.size() > 0) {
                Request request = storage[s].queue.back();

                MPI_Send(&storage[s].value, 1, MPI_INT, request.process_id, REQUEST, MPI_COMM_WORLD);
                storage[s].queue.pop_back();
                storage[s].free = false;
                cout << S_MSG << "Resource " << storage[s].id << " is allocated to process " << request.process_id << endl;
            }
        }
    }
}

// Функция клиента
static void client(int rank) {
    for (int i = 0; i < 5; i++) {
        int resource_id = rand() % RESOURCE_COUNT;
        int request[2] = { resource_id, 0 };

        // Запрос ресурса у сервера
        MPI_Send(&request, 2, MPI_INT, SERVER, REQUEST, MPI_COMM_WORLD);
        cout << C_MSG << "Requested resource " << resource_id << " for client " << rank << endl;

        int resource_value;
        MPI_Recv(&resource_value, 1, MPI_INT, SERVER, REQUEST, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        cout << C_MSG << "Recived resource " << resource_id << " for client " << rank << endl;

        // Обновление ресурса
        int change = (rand() % 10) + 1;
        request[1] = change;
        Sleep(100);

        MPI_Send(&request, 2, MPI_INT, SERVER, RELEASE, MPI_COMM_WORLD);
        cout << C_MSG << "Release resource " << resource_id << " from client " << rank << endl;

        Sleep(100);
    }
}

static int lab_4th(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int world_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    if (world_size != 3) {
        if (world_rank == 0) {
            cout << "This programm required 3 processes" << endl;
        }
        MPI_Finalize();
        return 1;
    }

    if (world_rank == SERVER) {
        server(world_size);
    }
    else {
        client(world_rank);
    }

    MPI_Finalize();
    return 0;
}

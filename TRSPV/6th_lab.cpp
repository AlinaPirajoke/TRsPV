#include <mpi.h>
#include <iostream>

const int MASS_SIZE = 12;
const int PROCESSES = 6;
const int IPP = MASS_SIZE / PROCESSES;

using namespace std;

static void sort(int array[]) {
    for (int i = 0; i < IPP*2; i++) {
        for (int j = 0; j < (IPP*2)-1; j++) {
            if (array[j] > array[j + 1]) {
                int b = array[j]; 
                array[j] = array[j + 1]; 
                array[j + 1] = b; 
            }
        }
    }
}

static void merge(int mass1[IPP], int mass2[IPP], int merged [IPP * 2] ) {
    for (int i = 0; i < IPP; i++) {
        merged[i] = mass1[i];
        merged[i + IPP] = mass2[i];
    }
}

static void remember(int merged[], int local[], int is_left) {
    int start = is_left ? 0 : IPP;
    for (int i = 0; i < IPP; i++) {
        local[i] = merged[start + i];
    }
}

static void print_array(int array[], int size) {
    for (int i = 0; i < size; i++) {
        cout << array[i] << " ";
    }
    cout << endl;
}

static int get_process_for_exchange(int rank, int step) {
    int x = 1;

    if (step % 2 == 1) {
        if (rank % (PROCESSES - 1) == 0) return -1;
        x = -1;
    }

    if (rank % 2 == 0) return rank + x;
    return rank - x;
}

static int lab_6th(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Status status;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != PROCESSES) {
        if (rank == 0) {
            cerr << "required " << PROCESSES << " processes." << endl;
        }
        MPI_Finalize();
        return 0;
    }

    int mass[MASS_SIZE], local_mass[IPP], foreign_mass[IPP], merged_mass[IPP * 2];

    if (rank == 0) {
        for (int i = 0; i < MASS_SIZE; i++) {
            mass[i] = rand() % 100;
        }

        cout << "Start array:" << endl;
        print_array(mass, MASS_SIZE);
    }
    
    MPI_Scatter(mass, IPP, MPI_INT, local_mass, IPP, MPI_INT, 0, MPI_COMM_WORLD);

    int pfe;
    bool is_left;

    for (int s = 0; s < PROCESSES; s++) {
        pfe = get_process_for_exchange(rank, s);
        
        if (pfe == -1) continue;
        is_left = rank < pfe;

        if (is_left) {
            MPI_Send(&local_mass, IPP, MPI_INT, pfe, 2, MPI_COMM_WORLD);
            MPI_Recv(&foreign_mass, size * size, MPI_INT, pfe, 2, MPI_COMM_WORLD, &status);   
        }
        else {
            MPI_Recv(&foreign_mass, size * size, MPI_INT, pfe, 2, MPI_COMM_WORLD, &status);
            MPI_Send(&local_mass, IPP, MPI_INT, pfe, 2, MPI_COMM_WORLD);
        }

        merge(local_mass, foreign_mass, merged_mass);
        sort(merged_mass);

        cout << rank << " : Merged array:";
        print_array(merged_mass, IPP * 2);
        
        remember(merged_mass, local_mass, is_left);
        
        cout << rank << " : Updated array:";
        print_array(local_mass, IPP);
    }

    MPI_Gather(local_mass, IPP, MPI_INT, mass, IPP, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        cout << "\nSorted array:" << endl;
        print_array(mass, MASS_SIZE);
    }

    MPI_Finalize();
    return 1;
}
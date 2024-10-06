#include <mpi.h>
#include <iostream>
#include <vector>
#include "1st_lab.cpp"

using namespace std;
const int CURRENT_LAB = 1;

int main(int argc, char** argv) {
	switch (CURRENT_LAB)
	{
	case 1: {
		lab_1st(argc, argv);
		break;
	}
	default:
		break;
	}
    return 1;
}
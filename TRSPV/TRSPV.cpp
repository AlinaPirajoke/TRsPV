#include "1st_lab.cpp"
#include "2nd_lab.cpp"
#include "3rd_lab.cpp"
#include "1st_lab_def_1.cpp"

using namespace std;
const int CURRENT_LAB = 3;

int main(int argc, char** argv) {
	switch (CURRENT_LAB)
	{
	case 1: {
		lab_1st(argc, argv);
		break;
	}
	case 101: {
		lab_1st_def_1(argc, argv);
		break;
	}
	case 2: {
		lab_2nd(argc, argv);
		break;
	}
	case 3: {
		lab_3rd(argc, argv);
		break;
	}
	default:
		break;
	}
    return 1;
}
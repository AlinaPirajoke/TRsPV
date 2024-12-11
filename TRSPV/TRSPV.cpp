#include "1st_lab.cpp"
//#include "1st_lab_def_1.cpp"
#include "1st_lab_def_2.cpp"

using namespace std;
const int CURRENT_LAB = 102;

int main(int argc, char** argv) {
	switch (CURRENT_LAB)
	{
	case 1: {
		lab_1st(argc, argv);
		break;
	}
	case 101: {
		//lab_1st_def_1(argc, argv);
		break;
	}
	case 102: {
		lab_1st_def_2(argc, argv);
		break;
	}
	default:
		break;
	}
    return 1;
}
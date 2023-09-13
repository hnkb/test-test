
//#include <Crib/Platform/Windows.h>
#include <iostream>

using namespace std;


int main(int argc, char** argv)
{
	cout << "Crib benchmarks\n";

	cout << argc << "\n";
	for (int i = 0; i < argc; i++)
		cout << "  -> " << argv[i] << "\n";

	string filename = __FILE__;
	cout << filename << "\n";

	//cout << "\n" << GetCommandLineA() << "\n";

 	return 0;
}

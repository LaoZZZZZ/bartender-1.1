#include <iostream>

#include "../oneproportiontest.hpp"

using namespace barcodeSpace;
using namespace std;

int main(int argc, char* argv[]) {
	double z_value = 3.18; // 0.001
	
	OneProportionTest tester(z_value, atof(argv[1]));
	cout << tester.isSignificant(atof(argv[2]), 0, atof(argv[3]), 0.0) << endl;

	return 0;
}

#include <iostream>
#include "../proportiontest.h"

using namespace barcodeSpace;
using namespace std;

int main() {
	double z_value = 3.18; // 0.001
	
	barcodeSpace::ProportionTest tester(z_value);
	cout << tester.isSignificant(0, 5, 2000, 2100) << endl;

	return 0;
}

#include <iostream>

#include "../distanceselector.h"

using namespace std;
using namespace barcodeSpace;
int main() {
	double error = 0.01;
	double cutoff = 0.001;
	size_t klen = 26;
	DistanceSelector select(error, cutoff, klen);
	cout << select.calculateDistance(603600) << endl;
	return 0;
}

#include <iostream>

#include "../distanceselector.h"

using namespace std;
using namespace barcodeSpace;
int main() {
	double error = 0.02;
	int meansize = 3;
	size_t klen = 20;
	DistanceSelector select(error, meansize, klen);
	cout << select.calculateDistance(603600) + 1 << endl;
	return 0;
}

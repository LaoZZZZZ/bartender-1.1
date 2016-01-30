#include <iostream>

#include "../meansequentialestimator.h"

using namespace std;
using barcodeSpace::MeanSequentialEstimator;
int main() {
	MeanSequentialEstimator est;
	for (int i = 0; i < 10; ++i) {
		est.Add(i);
	}
	cout << est.GetMean() << endl;	
	return 0;
}

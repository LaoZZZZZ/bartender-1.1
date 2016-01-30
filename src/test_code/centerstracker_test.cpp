#include <iostream>
#include <cassert>
#include <memory>
#include "../centerstracker.h"
using std::endl;
using std::cout;

using barcodeSpace::CentersTracker;
int main() {
	CentersTracker tracker(10);
	for(size_t i = 1; i < 20; ++i) {
		tracker.AddCount(i);
	}
	for( int i = 1; i < 10; i++) {
		assert(tracker.Counts()[i] == 1);
	}
	assert(tracker.Counts()[10] == 10);
	cout << "Passed all test cases!" << endl;
	return 0;
}

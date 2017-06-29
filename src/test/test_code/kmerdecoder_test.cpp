#include "../kmerdecoder.hpp"
#include "../typdefine.h"

#include <iostream>

using namespace std;
using namespace barcodeSpace;

int main() {
	kmer k = 1;
	KmerDecoder d(10);
	string result = d.DNASequence(k);
	cout << result << endl;
	vector<size_t> bps;
	d.bpSequence(k, bps);
	for(const auto& b : bps) {
		cout << b << '\t';
	}
	cout << endl;
	return 0;
}

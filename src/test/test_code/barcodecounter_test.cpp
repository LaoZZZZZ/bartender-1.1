#include <iostream>
#include <cassert>
#include <memory>
#include <boost/regex.hpp>
#include "../barcodecounter.h"
#include "../sequence.h"
#include "../typdefine.h"

using std::string;
using std::vector;
using std::endl;
using std::cout;
using std::unique_ptr;
using barcodeSpace::BarcodeCounter;
using barcodeSpace::Sequence; 
int main(void) {
	BarcodeCounter counter(5);
    counter.insert(0);
    counter.insert("AAAAA");
    assert(counter[0] == 2);
	counter.insert(32);
    counter.insert("AAGAA");
	assert(counter[32] == 2);
    assert(counter.size() == 2);
	assert(counter.BarcodeLength() == 5);
	const barcodeFreqTable table = counter.BarcodeTable();
	for(const auto& pair : table) {
		cout << pair.first << ":\t" << pair.second << endl;
	}
	return 0;
}	

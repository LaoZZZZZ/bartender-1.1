#include "../seedselector.hpp"
#include "../typdefine.h"


#include <iostream>
#include <vector>
using namespace std;
using namespace barcodeSpace;

int main() {
	vector<pair<string, freq>> barcodes = {
		{"GCAAAAACGT", 1},
		{"CCCCCAAGCA", 1},
		{"GGGGGAATAC", 1},
		{"GTTTTAAATG", 1},
		{"GCTAGAACCT", 1}};
	SeedSelector selector(6,10);
	for (const auto& seq : barcodes) {
		selector.addBarcode(seq);
 	}	
	vector<std::pair<int, int>> seeds = selector.getSeedsPositions();
	for (const auto& s : seeds) {
		cout << s.first << "\t" << s.second << endl;
	}
	return 0;
}

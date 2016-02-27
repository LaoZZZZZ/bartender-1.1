#include "../barcodepool.hpp"
#include "../seedselector.hpp"
#include "../typdefine.h"


#include <iostream>
#include <memory>
#include <vector>
#include <unordered_map>
#include <string>

using namespace std;
using namespace barcodeSpace;

int main() {
	vector<pair<string, freq>> barcodes = {
		{"GCAAAAACGT", 1},
		{"CCCCCAAGCA", 1},
		{"GGGGGAATAC", 1},
		{"GTTTTAAATG", 1},
		{"GCTAGAACCT", 1}};
   unordered_map<string, vector<string>> raw_barcode({{"AAAAA", {"AAA","AAC", "AAC","AGG", "ATC", "ATC", "ATT"}},{"AACAA", {"AAA"}},{"AACAT", {"TCA", "TCA", "TCA", "TTC"}},{"AGAAA",{"ATA"}}});
   BarcodePool::createInstance(raw_barcode);
   std::shared_ptr<BarcodePool> pool = BarcodePool::getAutoInstance();
 
	SeedSelector selector(5);
	selector.addBarcode(pool);
	vector<int> seeds = selector.getSeedsPositions();
	for (const auto& s : seeds) {
		cout << s << '\t';
	}
	cout << endl;
	return 0;
}

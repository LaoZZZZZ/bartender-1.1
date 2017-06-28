#include "../barcodepool.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;
using namespace barcodeSpace;

int main() {
	unordered_map<string, vector<string>> raw_barcode({{"AAAAA", {"AAA","AAC", "AAC", "ATC", "ATC"}},{"AAAAT", {"AAA", "AAC"}},{"AACAT", {"TCA", "TCA", "TCA", "TTC"}}});
        BarcodePool::createInstance(raw_barcode);
	std::shared_ptr<BarcodePool> pool = BarcodePool::getAutoInstance();
	for (size_t i = 0; i < pool->size(); ++i) {
		cout << pool->barcode(i) << '\t' << pool->barcodeFrequency(i) << endl;
		for (const auto& p : pool->primers(i)) {
			cout << p << '\t';
		}
		cout << endl;
	}
	cout << pool->NumOfRawBarcode() << '\t' << pool->NumOfReplicates() << endl;
	return 0;
}

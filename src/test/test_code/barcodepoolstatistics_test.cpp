#include "../barcodepool.hpp"
#include "../barcodepoolstatistics.hpp"

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
	cout << pool->NumOfRawBarcode() << '\t' << pool->NumOfReplicates() << endl;
	BarcodeStatistics stat(pool);
	
	for (const auto& p : stat.FullFrequencyTable(5)) {
		for (auto f : p)
			cout << f << ',';
		cout << endl;
	}	
	cout << endl;

	for (const auto&p : stat.UniqueFrequencyTable(5)) {
		for (auto f : p)
			cout << f << ',';
		cout << endl;
	}
	cout << endl;
	for (const auto& p : stat.FullEntropy(5)) {
		cout << p << ',';
	}	
	cout << endl;

	for (const auto&p : stat.UniqueEntropy(5)) {
		cout << p << ',';
	}
	cout << endl;

	return 0;

}

#include "../barcodepool.hpp"
#include "../barcodepoolstatistics.hpp"
#include "../errorestimator.hpp"

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
	
	ErrorEstimator estimator(28, 0.87);
	cout <<	estimator.estimate(stat.FullFrequencyTable(5), stat.FullEntropy(5)) << endl;
	
	return 0;

}

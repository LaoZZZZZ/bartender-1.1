#include "../clustersbucketer.hpp"
#include "../barcodepool.hpp"

#include <iostream>
#include <list>
#include <memory>
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

	BarcodeCluster::setBarcodePool(pool);	
	std::list<shared_ptr<BarcodeCluster>> clusters;
	size_t barcode_length = 5;
        size_t max_size = 0;
        
        for(size_t index = 0; index < pool->size(); ++ index){
            
            if (pool->barcode(index).length() != barcode_length)
                continue;
            std::shared_ptr<BarcodeCluster> ptemp(new BarcodeCluster(index));
            clusters.push_back(ptemp);
        }
	cout << clusters.size() << endl;
	
	vector<int> seeds{0,1,2,3,4};
	
	ClusterBucketer bucketer(seeds,3);
	cout << "round: " << bucketer.round() << endl;
	while (!bucketer.done()) {
	cout << "********************" <<endl;
	bucketer.shatter(clusters);
	const vector<list<shared_ptr<BarcodeCluster>>>& bins = bucketer.Bins();
	size_t i = 0;
	for (auto& b : bins) {
		++i;
		if(b.empty()) continue;
		cout << i << endl;
		for (const auto& c : b) {
			cout << c->center() << '\t';
		}
		cout << endl;
	}
	}
	return 0;
}

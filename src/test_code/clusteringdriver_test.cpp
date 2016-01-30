#include <iostream>
#include <cassert>
#include <memory>
#include <unordered_map>

#include "../clusteringdriver.hpp"

using namespace std;
using namespace barcodeSpace;


void cluster_drive(size_t barcode_length,
		   size_t seed_length,
		   const std::shared_ptr<BarcodePool>& pool) {
	ClusteringDriver driver(barcode_length, seed_length);
	driver.clusterDrive(pool);
	list<shared_ptr<BarcodeCluster>> clusters = driver.clusters();
	cout << clusters.size() << endl;
	for (const auto& c : clusters) {
		cout << c->center() << c->size() << endl;
	}
}
int main(void) {

   size_t barcode_length = 5;
   size_t seed_length = 2;
   unordered_map<string, vector<string>> raw_barcode({{"AAAAA", {"AAA","AAC", "AAC","AGG", "ATC", "ATC", "ATT"}},{"CAAAA", {"AAA"}},{"AACAT", {"TCA", "TCA", "TCA", "TTC"}}});
   BarcodePool::createInstance(raw_barcode);
   std::shared_ptr<BarcodePool> pool = BarcodePool::getAutoInstance();
   cout << pool->NumOfRawBarcode() << '\t' << pool->NumOfReplicates() << endl;

   BarcodeCluster::setBarcodePool(pool);	
   cluster_drive(barcode_length, seed_length, pool);
   return 0;
}



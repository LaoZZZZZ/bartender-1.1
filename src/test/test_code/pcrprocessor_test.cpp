#include <algorithm>
#include <numeric>
#include <iostream>
#include <cassert>
#include <memory>
#include <string>
#include <vector>
#include <array>
#include "../pcrprocessor.hpp"
#include "../barcodecluster.hpp"

using namespace std;
using namespace barcodeSpace;

double Entropy(const std::array<int, 4>& base_freq, double* majority, double* total) {
     *majority = *std::max_element(base_freq.begin(), base_freq.end());

     *total = std::accumulate(base_freq.begin(), base_freq.end(), 0);
     if(*total == *majority) {
         return 0.0;
     } else {
         double ratio = *majority / *total;
         return -1 * (ratio * log2(ratio) + (1-ratio)*log2(1-ratio));
     }
}

 
void printFrequency(const vector<array<int,4>>& frequencies) {
    cout << "*****************************" << endl;
    for(const auto& bp : frequencies) {
	for (const auto& f : bp) {
		cout << f << '\t';
        }
	cout << endl;
    } 
    cout << "*****************************" << endl;
 
}

int main() {
   //unordered_map<string, vector<string>> raw_barcode({{"AAAAA", {"AAA","AAC", "AAC","AGG", "ATC", "ATC", "ATT"}},{"CAAAA", {"AAA"}},{"AACAT", {"AGG", "ATC","TCA", "TCA", "TCA", "TTC"}}});
   unordered_map<string, vector<string>> raw_barcode1({{"AAAAA", {"AAA", "ATT", "TCA"}},{"CAAAA", {"AAA", "ACG", "TTT"}},{"AACAA", {"AAA","AGG", "ATC","TCA", "TTC"}},{"GAAAA",{"TCT","TGT", "TTT","TTT"}}});
   BarcodePool::createInstance(raw_barcode1);
   std::shared_ptr<BarcodePool> pool = BarcodePool::getAutoInstance();
   cout << pool->NumOfRawBarcode() << '\t' << pool->NumOfReplicates() << endl;
   BarcodeCluster::setBarcodePool(pool);
    for(size_t i = 0; i < pool->size(); ++i) {
	cout << pool->barcode(i) << " : ";
	for (const auto& p : pool->primers(i)) {
		cout << p << ',';
	}
	cout << endl;
    }
 
    PCRProcessor pcr_dealer;
    list<std::shared_ptr<BarcodeCluster>> clusters;
    ////////////////////////////////////////////////////////
    /////////////////////One center////////////////////////
    ///////////////////////////////////////////////////////

    std::shared_ptr<BarcodeCluster> first_cluster(new BarcodeCluster(0));
    std::shared_ptr<BarcodeCluster> second_cluster(new BarcodeCluster(1));
    std::shared_ptr<BarcodeCluster> third_cluster(new BarcodeCluster(2));
    std::shared_ptr<BarcodeCluster> four_cluster(new BarcodeCluster(3));
    cout << "cluster size: " << first_cluster->size() << endl;
    first_cluster->merge(second_cluster);
    first_cluster->merge(third_cluster);
    first_cluster->merge(four_cluster);
    clusters.push_back(first_cluster); 
    pcr_dealer.process(clusters,pool);
    cout << clusters.front()->size() <<endl;
    printFrequency(clusters.front()->bpFrequency());
    for(size_t i = 0; i < pool->size(); ++i) {
	cout << pool->barcode(i) << " : " ;
	for (const auto& p : pool->primers(i)) {
		cout << p << ',';
	}
	cout << endl;
    }
    return 0;
}

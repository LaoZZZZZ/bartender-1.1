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
   unordered_map<string, vector<string>> raw_barcode1({{"AAAAA", {"AAA", "TCA"}},{"CAAAA", {"AAA"}},{"AACAT", {"AAA","AGG", "ATC","TCA", "TTC"}}});
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
    cout << "cluster size: " << first_cluster->size() << endl;
    first_cluster->merge(second_cluster);
    first_cluster->merge(third_cluster);
    clusters.push_back(first_cluster); 
    pcr_dealer.process(clusters,pool);
    //assert(clusters.front()->size() == 5);
    cout << clusters.front()->size() <<endl;
    printFrequency(clusters.front()->bpFrequency());
    for(size_t i = 0; i < pool->size(); ++i) {
	cout << pool->barcode(i) << " : " ;
	for (const auto& p : pool->primers(i)) {
		cout << p << ',';
	}
	cout << endl;
    }
    /*
    ///////////////////////////////////////////////////////
    //////////////////////////Two barcode freq /////////////
    ////////////////////////////////////////////////////////    
    pcr_dealer.reset();
    clusters.clear();
    seq = "TGACT";
    f = 6;
    k = 0;
    kmersBitwiseTransform::getInstance()->seq_2_bitwise(seq, k, 5);    

    const std::shared_ptr<DictatorCenter> second_center(new DictatorCenter(k,5));
    std::shared_ptr<cluster> second_cluster(new dictatorCluster(second_center,{f}));
    barcode_primers[5].insert({k, {"AAA", "AAA","AAA", "AAA", "ACC", "ACT"}}); 

    first_cluster->merge(*second_cluster);
    clusters.push_back(first_cluster);
    pcr_dealer.process(clusters, barcode_primers);
    printFrequency(clusters.front()->bpFrequency());
    cout << "cluster size after pcr " << clusters.front()->size() << endl;
    cout << pcr_dealer.numberOfReplicates() << endl;
    cout << pcr_dealer.PCREffect() << endl;
    */
    return 0;
}

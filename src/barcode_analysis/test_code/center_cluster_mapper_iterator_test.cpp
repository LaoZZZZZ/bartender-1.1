#include <algorithm>
#include <iostream>
#include <cassert>
#include <list>
#include <memory>
#include <string>
#include <vector>
#include <array>
#include "../dictatorcenter.h"
#include "../kmers_bitwisetransform.h"
#include "../typdefine.h"
#include "../cluster.h"
#include "../dictatorcluster.h"
#include "../centerclustermapper.h"
#include "../centerrecalibrator.h"
#include "../centerclustermapperiterator.h"
using std::string;
using std::endl;
using std::cout;
using std::unique_ptr;
using std::vector;
using std::array;
using std::list;
using barcodeSpace::DictatorCenter;
using barcodeSpace::kmersBitwiseTransform;
using barcodeSpace::cluster;
using barcodeSpace::dictatorCluster;
using barcodeSpace::CenterClusterMapper;
using barcodeSpace::CenterClusterMapperIterator;
using barcodeSpace::CenterRecalibrator;
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
void printCenter(const vector<kmer>& centers) {
    for(const auto& c : centers) {
	string temp;
        kmersBitwiseTransform::getInstance()->bitwise_2_seq(c, temp, 5);
        cout << temp << endl;
    }   
}
int main() {
    std::shared_ptr<CenterClusterMapper> linker(new CenterClusterMapper(10));
    unordered_map<int, vector<kmer>> all_centers;  
    // At most give three centers.
    // entropy threshold is 0.33(0.1vs0.9)
    // base pair proportion larger than 0.1 could be considered as a center
    CenterRecalibrator center_detector(0.33, // entropy threshold
				       3, // maximum center 
				       0.05, // p-value
				       0.1); // sequence error

    ////////////////////////////////////////////////////////
    /////////////////////One center////////////////////////
    ///////////////////////////////////////////////////////
    string seq("AAAAA"); 
    kmer k;
    freq f = 5;
    kmersBitwiseTransform::getInstance()->seq_2_bitwise(seq, k, 5);    
    const std::shared_ptr<DictatorCenter> center(new DictatorCenter(k,5));
    std::shared_ptr<cluster> first_cluster(new dictatorCluster(center,{f}));
    const vector<array<int,4>>& frequencies = first_cluster->bpFrequency();
    
    std::vector<kmer> centers;
    vector<double> entropies{0,0,0,0,0};
    
    assert(!center_detector.IdentifyCenters(frequencies, entropies, &centers));
    assert(centers.size() == 1);
    linker->addPair(first_cluster, centers); 
    all_centers.insert({first_cluster->ClusterID(), centers});
//    printCenter(centers);
    ////////////////////////////////////////////////////////
    /////////////////////// Two centers ///////////////////
    ///////////////////////////////////////////////////////
    seq = "AAAAT";
    k = 0;
    kmersBitwiseTransform::getInstance()->seq_2_bitwise(seq, k, 5);    
    const std::shared_ptr<DictatorCenter> second_center(new DictatorCenter(k,5));
 
    const vector<array<int,4>> frequency_table_2(
	{{17, 3, 0, 0},
	{20, 0, 0, 0},
	{20, 0, 0, 0},
	{20, 0, 0, 0},
	{0, 0, 0, 20}});
    std::shared_ptr<cluster> second_cluster(new dictatorCluster(second_center, {20}, frequency_table_2, 2));
    double majority = 0;
    double total = 0;
    vector<double> entropy_second;
    for (const auto bp : second_cluster->bpFrequency()) {
	entropy_second.push_back(Entropy(bp, &majority, &total));	
    }
    centers.clear();
    assert(!center_detector.IdentifyCenters(second_cluster->bpFrequency(), entropy_second, &centers));
    assert(centers.size() == 2);
    linker->addPair(second_cluster, centers); 
    all_centers.insert({second_cluster->ClusterID(), centers});
 //   printCenter(centers);
     ////////////////////////////////////////////////////////
    /////////////////////// Three centers ///////////////////
    ///////////////////////////////////////////////////////
    seq = "AAAAC";
    k = 0;
    kmersBitwiseTransform::getInstance()->seq_2_bitwise(seq, k, 5);    
    const std::shared_ptr<DictatorCenter> third_center(new DictatorCenter(k,5));
    const vector<array<int,4>> frequency_table_3(
	{{14, 3, 3, 0},
	{20, 0, 0, 0},
	{20, 0, 0, 0},
	{20, 0, 0, 0},
	{0, 20, 0,0 }});

    std::shared_ptr<cluster> third_cluster(new dictatorCluster(third_center, {20}, frequency_table_3, 3));
    majority = 0;
    total = 0;
    vector<double> entropy_third;
    for (const auto bp : third_cluster->bpFrequency()) {
	entropy_third.push_back(Entropy(bp, &majority, &total));	
    }
    centers.clear();
    assert(!center_detector.IdentifyCenters(third_cluster->bpFrequency(), entropy_third, &centers));
    assert(centers.size() == 3);
    centers.clear();
    assert(!center_detector.IdentifyCenters(third_cluster->bpFrequency(), entropy_third, &centers));
    linker->addPair(third_cluster, centers);
    all_centers.insert({third_cluster->ClusterID(), centers});
  //  printCenter(centers);
    ////////////////////////////////////////////////////////
    /////////////////////// Four centers ///////////////////
    ///////////////////////////////////////////////////////
    seq = "ATAAG";
    k = 0;
    kmersBitwiseTransform::getInstance()->seq_2_bitwise(seq, k, 5);    
    const std::shared_ptr<DictatorCenter> four_center(new DictatorCenter(k,5));
    const vector<array<int,4>> frequency_table_4(
	{{17, 3, 0, 0},
	{20, 0, 0, 0},
	{20, 0, 0, 0},
	{0, 0, 0, 20},
	{0, 3, 17,0 }});


    std::shared_ptr<cluster> four_cluster(new dictatorCluster(four_center, {20}, frequency_table_4, 4));
    majority = 0;
    total = 0;
    vector<double> entropy_four;
    for (const auto bp : four_cluster->bpFrequency()) {
	entropy_four.push_back(Entropy(bp, &majority, &total));	
    }
    centers.clear();
    assert(center_detector.IdentifyCenters(four_cluster->bpFrequency(), entropy_four, &centers));
    assert(centers.size() == 3);
    linker->addPair(four_cluster, centers);
    all_centers.insert({four_cluster->ClusterID(), centers});

   // printCenter(centers);
    CenterClusterMapperIterator iter(linker);
    while (iter.has_next()) {
	int id = iter.next()->ClusterID();
	list<kmer> temp( all_centers[id].begin(),all_centers[id].end());
        assert(linker->myCenter(id) == temp);
    }
    return 0;
}

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
#include "../clustercenterlinkgenerator.h"
#include "../errorrateestimator.h"
using std::string;
using std::endl;
using std::cout;
using std::shared_ptr;
using std::vector;
using std::array;
using std::list;
using barcodeSpace::DictatorCenter;
using barcodeSpace::kmersBitwiseTransform;
using barcodeSpace::cluster;
using barcodeSpace::dictatorCluster;
using barcodeSpace::CenterClusterMapper;
using barcodeSpace::ClusterCenterLinkGenerator;
using barcodeSpace::ErrorRateEstimator;

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

void printCenter(const list<kmer>& centers) {
    for(const auto& c : centers) {
	string temp;
        kmersBitwiseTransform::getInstance()->bitwise_2_seq(c, temp, 5);
        cout << temp << endl;
    }   
}
int main() {
    list<shared_ptr<cluster>> clusters;
    unordered_map<int, vector<kmer>> all_centers;  
    ClusterCenterLinkGenerator link_generator(0.33, 4, 0.05, 0.1);
    ErrorRateEstimator error_estimator(0.9, 5, 65);  

    ////////////////////////////////////////////////////////
    /////////////////////One center////////////////////////
    ///////////////////////////////////////////////////////
    string seq("AAAAA"); 
    kmer k;
    freq f = 5;
    kmersBitwiseTransform::getInstance()->seq_2_bitwise(seq, k, 5);    
    const std::shared_ptr<DictatorCenter> center(new DictatorCenter(k,5));
    std::shared_ptr<cluster> first_cluster(new dictatorCluster(center,{f}));
    clusters.push_back(first_cluster);    
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
    clusters.push_back(second_cluster);
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
    clusters.push_back(third_cluster);
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
    clusters.push_back(four_cluster);
    error_estimator.Estimate(clusters);
    cout << error_estimator.ErrorRate() << endl;
    const list<vector<double>>& entropies = error_estimator.Entropies();    
    link_generator.Generate(clusters, entropies);
    
    const shared_ptr<CenterClusterMapper> linker = link_generator.CenterClusterLink();  
    for (int i = 0; i <= 4; ++i) {
	if (linker->hasCluster(i)) {
		const list<kmer>& c = linker->myCenter(i);
		printCenter(c);	
	}
    } 
    return 0;
}

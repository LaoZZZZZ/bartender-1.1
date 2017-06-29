#include <algorithm>
#include <numeric>
#include <iostream>
#include <cassert>
#include <memory>
#include <string>
#include <vector>
#include <array>
#include "../dictatorcenter.h"
#include "../kmers_bitwisetransform.h"
#include "../typdefine.h"
#include "../cluster.h"
#include "../dictatorcluster.h"
#include "../centerrecalibrator.h"
using namespace std;
using barcodeSpace::DictatorCenter;
using barcodeSpace::kmersBitwiseTransform;
using barcodeSpace::cluster;
using barcodeSpace::dictatorCluster;
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
void printCenter(const vector<string>& centers) {
    for(const auto& c : centers) {
        cout << c << endl;
    }   
}
int main() {
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
    string seq("AGACT"); 
    kmer k;
    freq f = 5;
    kmersBitwiseTransform::getInstance()->seq_2_bitwise(seq, k, 5);    
    const std::shared_ptr<DictatorCenter> center(new DictatorCenter(k,5));
    std::unique_ptr<cluster> first_cluster(new dictatorCluster(center,{f}));
    const vector<array<int,4>>& frequencies = first_cluster->bpFrequency();
    printFrequency(frequencies);
    
    std::vector<std::string> centers;
    vector<double> entropies{0,0,0,0,0};
    
    assert(!center_detector.IdentifyCenters(frequencies, entropies, &centers));
    printCenter(centers);   
    assert(centers.size() == 1);
    ////////////////////////////////////////////////////////
    /////////////////////// Two centers ///////////////////
    ///////////////////////////////////////////////////////
    const vector<array<int,4>> frequency_table_2(
	{{17, 3, 0, 0},
	{0, 0, 20, 0},
	{20, 0, 0, 0},
	{0, 20, 0, 0},
	{0, 0, 0, 20}});
    std::unique_ptr<cluster> second_cluster(new dictatorCluster(center, {20}, frequency_table_2, 2));
    printFrequency(second_cluster->bpFrequency()); 
    double majority = 0;
    double total = 0;
    vector<double> entropy_second;
    for (const auto bp : second_cluster->bpFrequency()) {
	entropy_second.push_back(Entropy(bp, &majority, &total));	
    }
    centers.clear();
    assert(!center_detector.IdentifyCenters(second_cluster->bpFrequency(), entropy_second, &centers));
    printCenter(centers); 
    assert(centers.size() == 2);
  
     ////////////////////////////////////////////////////////
    /////////////////////// Three centers ///////////////////
    ///////////////////////////////////////////////////////
    const vector<array<int,4>> frequency_table_3(
	{{14, 3, 3, 0},
	{0, 0, 20, 0},
	{20, 0, 0, 0},
	{0, 20, 0, 0},
	{0, 0, 0, 20}});
    std::unique_ptr<cluster> third_cluster(new dictatorCluster(center, {20}, frequency_table_3, 3));
    printFrequency(third_cluster->bpFrequency()); 
    majority = 0;
    total = 0;
    vector<double> entropy_third;
    for (const auto bp : third_cluster->bpFrequency()) {
	entropy_third.push_back(Entropy(bp, &majority, &total));	
    }
    centers.clear();
    assert(!center_detector.IdentifyCenters(third_cluster->bpFrequency(), entropy_third, &centers));
    printCenter(centers); 
    assert(centers.size() == 3);
    centers.clear();
    assert(!center_detector.IdentifyCenters(third_cluster->bpFrequency(), entropy_third, &centers));
    ////////////////////////////////////////////////////////
    /////////////////////// Four centers ///////////////////
    ///////////////////////////////////////////////////////
    const vector<array<int,4>> frequency_table_4(
	{{16, 4, 0, 0},
	{3, 0, 17, 0},
	{20, 0, 0, 0},
	{0, 20, 0, 0},
	{0, 0, 0, 20}});
    std::unique_ptr<cluster> four_cluster(new dictatorCluster(center, {20}, frequency_table_4, 4));
    printFrequency(four_cluster->bpFrequency()); 
    majority = 0;
    total = 0;
    vector<double> entropy_four;
    for (const auto bp : four_cluster->bpFrequency()) {
	entropy_four.push_back(Entropy(bp, &majority, &total));	
    }
    centers.clear();
    assert(center_detector.IdentifyCenters(four_cluster->bpFrequency(), entropy_four, &centers));
    printCenter(centers); 
    assert(centers.size() == 3);
    
    const vector<array<int,4>> frequency_table_20 (
{		
{0,0,0,6},
{0,0,6,0},
{6,0,0,0},
{6,0,0,0},
{0,0,6,0},
{1,5,0,0},
{0,1,0,5},
{0,1,5,0},
{1,0,1,4},
{0,0,0,6},
{6,0,0,0},
{0,1,3,2},
{1,0,5,0},
{5,0,1,0},
{6,0,0,0},
{6,0,0,0},
{6,0,0,0},
{0,6,0,0},
{0,0,0,6},
{0,6,0,0}
});

    vector<double> entropy_20;
    for (const auto& bp : frequency_table_20) {
	entropy_20.push_back(Entropy(bp, &majority, &total));	
	cout << entropy_20.back() << endl;
    }
    centers.clear();
    center_detector.IdentifyCenters(frequency_table_20, entropy_20, &centers);
    for (const auto& c : centers) {	
	cout << c << endl;	
    }	
    return 0;
}

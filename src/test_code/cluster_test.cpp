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
using std::string;
using std::endl;
using std::cout;
using std::unique_ptr;
using std::vector;
using std::array;
using barcodeSpace::DictatorCenter;
using barcodeSpace::kmersBitwiseTransform;
using barcodeSpace::cluster;
using barcodeSpace::dictatorCluster;
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
    string seq("AGACT"); 
    kmer k;
    freq f = 5;
    kmersBitwiseTransform::getInstance()->seq_2_bitwise(seq, k, 5);    
    const std::shared_ptr<DictatorCenter> center(new DictatorCenter(k,5));
    std::unique_ptr<cluster> first_cluster(new dictatorCluster(center,{f}));
    const vector<array<int,4>>& frequencies = first_cluster->bpFrequency();
    printFrequency(frequencies);
    
    // Create another cluster and merge with the first one.
    string another_seq("AGCCG");
    kmer another_k;
    freq another_f = 3;
    kmersBitwiseTransform::getInstance()->seq_2_bitwise(another_seq, another_k, 5);    
    const std::shared_ptr<DictatorCenter> another_center(new DictatorCenter(another_k, 5));
    std::unique_ptr<cluster> second_cluster(new dictatorCluster(another_center, {another_f}));
    
    first_cluster->merge(*second_cluster);
    printFrequency(first_cluster->bpFrequency());
    seq.clear();
    kmersBitwiseTransform::getInstance()->bitwise_2_seq(first_cluster->center()->center(), seq, 5);
    cout << seq  << endl;
    
    first_cluster->merge(*second_cluster);
    printFrequency(first_cluster->bpFrequency());
    seq.clear();
    kmersBitwiseTransform::getInstance()->bitwise_2_seq(first_cluster->center()->center(), seq, 5);
    cout << seq  << endl;
    cout << cluster::MaximumID() << endl;
    /*
    // Similar center( distance is within 2 bp)
    string another("AGCCA");
    kmer anotherk;
    kmersBitwiseTransform::getInstance()->seq_2_bitwise(another, anotherk, 5);
    DictatorCenter another_center(anotherk, 5);
    assert(another_center.isSimilar(center)); 
    DictatorCenter::SetThreshold(1);
    assert(!another_center.isSimilar(center));    
    // Non-similar center( distance is within 2 bp)
    string different("AGCCC");
    kmer differentk;
    kmersBitwiseTransform::getInstance()->seq_2_bitwise(different, differentk, 5);
    DictatorCenter different_center(differentk, 5);
    assert(!different_center.isSimilar(center)); 
    */
    return 0;
}

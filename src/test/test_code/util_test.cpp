#include <iostream>
#include <cassert>
#include "../util.h"
#include "../dictatorcluster.h"
#include "../dictatorcenter.h"
using std::string;
using std::vector;
using std::endl;
using std::cout;
using barcodeSpace::kmersDictionary;
using barcodeSpace::reverseComplementInplace;
using barcodeSpace::reverseComplement;
using barcodeSpace::outputPositionWeightMatrix;
using barcodeSpace::cluster;
using barcodeSpace::DictatorCenter;
using barcodeSpace::dictatorCluster;
using namespace barcodeSpace;
int main() {
    string seq;
    reverseComplementInplace(seq);
    seq = "ATCG";
    reverseComplementInplace(seq);
    assert(seq == "CGAT");
    seq = "AAAAA";
    reverseComplementInplace(seq);
    assert(seq == "TTTTT");
    assert(reverseComplement(seq) == "AAAAA");
    seq = "CGTA";
    assert(reverseComplement(seq) == "TACG");
    seq = "CGTAC";
    assert(reverseComplement(seq) == "GTACG");
    
    seq = ("AGACT"); 
    kmer k;
    freq f = 5;
    kmersBitwiseTransform::getInstance()->seq_2_bitwise(seq, k, 5);    
    const std::shared_ptr<DictatorCenter> center(new DictatorCenter(k,5));
    std::shared_ptr<cluster> first_cluster(new dictatorCluster(center,{f}));
   
    string filename("test_frequency.txt");
    std::list<shared_ptr<cluster>> clusters({first_cluster});
    outputPositionWeightMatrix(filename, clusters); 
    return 0;
}

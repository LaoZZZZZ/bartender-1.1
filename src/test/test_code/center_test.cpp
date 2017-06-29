#include <iostream>
#include <cassert>
#include <memory>
#include <string>
#include "../dictatorcenter.h"
#include "../kmers_bitwisetransform.h"
#include "../typdefine.h"

using std::string;
using std::endl;
using std::cout;
using std::unique_ptr;
using barcodeSpace::DictatorCenter;
using barcodeSpace::kmersBitwiseTransform;
int main() {
    string seq("AGAAA"); 
    kmer k;
    kmersBitwiseTransform::getInstance()->seq_2_bitwise(seq, k, 5);    
    DictatorCenter center(k,5);
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
    return 0;
}

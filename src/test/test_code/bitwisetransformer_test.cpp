#include <iostream>
#include <cassert>
#include "../kmers_bitwisetransform.h"
#include "../typdefine.h"
using std::string;
using std::vector;
using std::endl;
using std::cout;
using barcodeSpace::kmersDictionary;
using barcodeSpace::kmersBitwiseTransform;
 
int main(void) {
    kmersBitwiseTransform* trans = kmersBitwiseTransform::getInstance();
    kmer hashcode(0);
    string seq("AAAA");
    trans->seq_2_bitwise(seq, hashcode, 4); 
    assert(0 == hashcode); 
    seq = "AAAC";
    trans->seq_2_bitwise(seq, hashcode, 4); 
    assert(1 == hashcode); 
    seq = "AAAG";
    trans->seq_2_bitwise(seq, hashcode, 4); 
    assert(2 == hashcode); 
    seq = "AAAT";
    trans->seq_2_bitwise(seq, hashcode, 4); 
    assert(3 == hashcode); 

    seq = "TTTT";
    trans->seq_2_bitwise(seq, hashcode, 4); 
    assert(255 == hashcode); 

    seq = "TAAA";
    trans->seq_2_bitwise(seq, hashcode, 4); 
    assert(192 == hashcode); 

    seq = "GAAA";
    trans->seq_2_bitwise(seq, hashcode, 4); 
    assert(128 == hashcode); 
    seq = "CAAA";
    trans->seq_2_bitwise(seq, hashcode, 4); 
    assert(64 == hashcode); 
    hashcode = 64;
    seq.clear();
    trans->bitwise_2_seq(hashcode, seq, 4);
    assert("CAAA" == seq);
    hashcode = 128;
    seq.clear();
    trans->bitwise_2_seq(hashcode, seq, 4);
    assert("GAAA" == seq);
    hashcode = 192;
    seq.clear();
    trans->bitwise_2_seq(hashcode, seq, 4);
    assert("TAAA" == seq);
    return 0;
}

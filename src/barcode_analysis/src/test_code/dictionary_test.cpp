#include <iostream>
#include <cassert>
#include "../kmers_dictionary.h"
using std::string;
using std::vector;
using std::endl;
using std::cout;
using barcodeSpace::kmersDictionary;

int main(void) {
    kmersDictionary* dict = kmersDictionary::getInstance();

    assert(0 == int(dict->asc2dna('A')));
    assert(1 == int(dict->asc2dna('C')));
    assert(2 == int(dict->asc2dna('G')));
    assert(3 == int(dict->asc2dna('T')));
    assert('T' == dict->complement('A'));
    assert('G' == dict->complement('C'));
    assert('C' == dict->complement('G'));
    assert('A' == dict->complement('T'));

    assert('A' == dict->dna2asc(0));	
    assert('C' == dict->dna2asc(1));	
    assert('G' == dict->dna2asc(2));	
    assert('T' == dict->dna2asc(3));	
    assert('N' == dict->dna2asc(4));	
    return 0;
}

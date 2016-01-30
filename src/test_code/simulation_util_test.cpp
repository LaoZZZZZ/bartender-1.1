#include <iostream>
#include <cassert>
#include "../testSimulation.cpp"
using std::string;
using std::vector;
using std::endl;
using std::cout;
using barcodeSpace::kmersDictionary;
using barcodeSpace::reverseComplementInplace;
using barcodeSpace::reverseComplement;
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
    return 0;
}

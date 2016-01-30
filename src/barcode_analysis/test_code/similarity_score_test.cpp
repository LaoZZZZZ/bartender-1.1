#include <iostream>
#include <cassert>
#include <memory>
#include <string>
#include <array>
#include <vector>

#include "../util.h"
#include "../kmers_bitwisetransform.h"
#include "../typdefine.h"
using std::string;
using std::endl;
using std::cout;
using std::unique_ptr;
using std::array;
using std::vector;
using barcodeSpace::kmersBitwiseTransform;
using barcodeSpace::similarityScore;
int main() {
    string seq("AGAAA"); 
    kmer k;
    kmersBitwiseTransform::getInstance()->seq_2_bitwise(seq, k, 5);    
    cout << k <<endl;
    vector<array<int, 4>> frequency_table;
    frequency_table.push_back({5,0,0,0});
    frequency_table.push_back({0,0,5,0});
    frequency_table.push_back({5,0,0,0});
    frequency_table.push_back({5,0,0,0});
    frequency_table.push_back({5,0,0,0});
    assert(0 == similarityScore(k, frequency_table));
    
    // Only one  base pair different
    frequency_table.clear();
    frequency_table.push_back({5,0,0,0});
    frequency_table.push_back({1,0,4,0});
    frequency_table.push_back({5,0,0,0});
    frequency_table.push_back({5,0,0,0});
    frequency_table.push_back({5,0,0,0});
    assert( 1 == similarityScore(k, frequency_table)); 

    // Only six base pair different
    frequency_table.clear();
    frequency_table.push_back({5,0,0,0});
    frequency_table.push_back({1,0,4,0});
    frequency_table.push_back({3,0,2,0});
    frequency_table.push_back({4,1,0,0});
    frequency_table.push_back({3,0,1,1});
    assert( 6 == similarityScore(k, frequency_table)); 
    cout << (2^3^5^3^5) << endl; 
    return 0;
}

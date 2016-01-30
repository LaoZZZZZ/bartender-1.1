
#ifndef UTIL_H_
#define UTIL_H_

#include <array>
#include <limits>
#include <memory>
#include <stdlib.h>
#include <string>
#include <stdexcept>
#include <vector>

#include "kmer_freq.h"
#include "kmers_dictionary.h"
#include "typdefine.h"

namespace barcodeSpace {


// Check if the average quality is larger than the given threshold.
bool qualityCheck(const std::string& qualities, double threshold);
//TODO(lu): Implement a in-place version.
void reverseComplementInplace(std::string& seq);
std::string reverseComplement(const std::string& seq);
template<typename T>
int     hammingDist(const T& seq1,const T& seq2){
    int dist = 0;
    if(seq1.length() != seq2.length()){
        throw std::runtime_error(" Can not calculate hamming distance between unequal length strings!\n");
    }else{
        size_t l = seq1.length();
        for(size_t i = 0; i < l ; i++){
            if(seq1[i] != seq2[i])
                ++dist;
        }
    }
    return dist;
}

size_t kmerDistance(const kmer&,const kmer&,size_t);

/**
  * Calculates the the similarity score by doing pair comparison.
*/
double similarityScore(const kmer&, const std::list<kmers_freq>&,size_t);

/**
  * Calculates the similarity score using the frequency table,
  * which should be faster than the pairwise compairson method.
*/
double similarityScore(const kmer& center,
                       const std::vector<std::array<int, 4>>& frequency_table);

std::list<std::string> strSplit(const std::string&);

void strSplit(const std::string&, std::list<std::string>&);


}

#endif /*ndef UTIL_H_*/

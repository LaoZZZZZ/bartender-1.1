#include "barcodecluster.hpp"
#include "util.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>
#include <memory>
#include <boost/algorithm/string.hpp>

using std::string;
using std::list;
using std::fstream;
using std::endl;
namespace barcodeSpace{
std::list<std::string> strSplit(const std::string& str) {
    string tmp;
    std::list<std::string> result;
    int len(static_cast<int>(str.length()));
    for(int i = 0; i < len; i++){
        if (str[i] == ',') {
            result.push_back(tmp);
            tmp.clear();
        } else {
            tmp += str[i];
        }
    }
    if (tmp.length())
        result.push_back(tmp);
    return result;
}

void strSplit(const std::string& str, std::list<std::string>& result) {
    string tmp;
    result.clear();
    int len(static_cast<int>(str.length()));
    for (int i = 0; i < len; i++) {
        if(str[i] == ','){
            result.push_back(tmp);
            tmp.clear();
        } else {
            tmp += str[i];
        }
    }
    if (tmp.length())
        result.push_back(tmp);
}

void reverseComplementInplace(std::string& seq) {
    if (seq.empty()) return;
    kmersDictionary* dict = kmersDictionary::getInstance();
    int start = 0;
    int end = static_cast<int>(seq.length()) - 1;
    char temp;
    while (start <= end) {
        temp = dict->complement(seq[start]);
        seq[start] = dict->complement(seq[end]);
        seq[end] = temp;
        ++start;
        --end;
    }
}

std::string reverseComplement(const std::string& seq) {

    kmersDictionary* dict = kmersDictionary::getInstance();
    std::string result;
    int pos = static_cast<int>(seq.length()) - 1;
    while (pos >= 0) {
        result.push_back(dict->complement(seq[pos]));
        --pos;
    }
    return result;
}

size_t kmerDistance(const kmer& k1, const kmer& k2, size_t klen) {
    size_t total = 0;
    kmer sim = k1^k2;
    for(size_t i = 0; i < klen; i++){
        total += ((3 &sim >> (2*i)) > 0);
    }
    return total;
}

/*
double similarityScore(const kmer& k, const list<kmers_freq>& barcodes, size_t klen){

    double result(0);
    for(auto it = barcodes.begin(); it != barcodes.end(); it++){
        result += kmerDistance(it->_key,k,klen)*it->_freq;
    }
    return result;
}*/
double similarityScore(const kmer& center,
                       const std::vector<std::array<int, 4>>& frequency_table) {
    size_t total = 0;
    const auto& first = frequency_table.front();
    int total_freq = 0;
    for(const auto& c : first) {
        total_freq += c;
    }
    size_t sz = frequency_table.size() - 1;
    for(size_t i = 0; i < sz; ++i) {
        total += total_freq - frequency_table[sz - i][(center>>(2*i)) & 0x3];
    }
    return total;
}



bool qualityCheck(const std::string& qual, double threshold){
    double average(0);
    size_t len = qual.length();
    for(size_t i = 0; i < len; i++){
        average += static_cast<int>(qual[i]);
    }
    return average/len > threshold;
}

void outputPositionWeightMatrix(const std::string& filename,
                                const std::vector<std::shared_ptr<BarcodeCluster>>& clusters) {

    fstream outhandler(filename.c_str(), fstream::out);
    for(const auto& c : clusters) {
        const std::vector<std::array<int,4>>& freq = c->bpFrequency();
        for(int bp = 0; bp < 4; ++bp) {
            outhandler << c->center();
            for(size_t pos = 0; pos < freq.size(); ++pos) {
                outhandler << "," << freq[pos][bp];
            }
            outhandler << endl;
        }
    }
}

bool isDnaSequence(const std::string& sequence) {
    kmersDictionary* dict = kmersDictionary::getInstance();
    for (const auto& c : sequence) {
        if (!dict->isNucleotide(c)) {
            return false;
        }
    }
    return true;
}

std::string toUpper(const string& str) {
    return boost::to_upper_copy<std::string>(str);
}
}   // namespace barcodeSpace.

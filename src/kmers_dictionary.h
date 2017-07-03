#ifndef KMERS_DICTIONARY_H
#define KMERS_DICTIONARY_H

#include "typdefine.h"

#include <memory>
#include <unordered_map>
#include <unordered_set>

using std::unordered_map;
using std::unordered_set;
namespace barcodeSpace{
/**
 * Holds all possible characters.
 * Currently it support basically contains ATCGN,
 * which coded separatedly as 0 1 2 3 4.
*/
class kmersDictionary
{
public:
    static kmersDictionary* getInstance();
    static std::shared_ptr<kmersDictionary> getAutoInstance();
    // Converts 0 , 1, 2, 3, 4 to 'A', 'C', 'G', 'T', 'N'.
    char dna2asc(int c)const { return _dna2asc[c];}
    // Converts 'A', 'C', 'G', 'T', 'N'  to 0,1,2,3,4.
    // current N will be viewed as A
    UINT8 asc2dna(int c)const {return _asc2dna[c];}
    // Given the coded dna bp, return then coded complementary bp
    // i.e 0(A) -> 3(T), 1(C) -> 2(G)
    char complement(int c)const {return _complement[c];}
    bool isNucleotide(int c) {
        return _asc2dna[c] != 20U;
    }
protected:
    kmersDictionary();
    bool isValid(char c);
private:
    static std::shared_ptr<kmersDictionary>           _pIns;
    /// For converting from ASCII to the Dna5 code where A=0, C=1, G=2,
    /// T=3, N=4
    UINT8 _asc2dna[256];
    char _dna2asc[5];
    char _complement[256];
};
} //namespace kmers_ppseq
#endif // KMERS_DICTIONARY_H

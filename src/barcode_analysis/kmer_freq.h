#ifndef KMER_FREQ_H
#define KMER_FREQ_H
#include <ostream>
#include <functional>
#include <cassert>
#include "typdefine.h"
#include "assert_helpers.h"


namespace barcodeSpace{


/**
  * Keeps record the frequency of single kmer.
  *
*/
class kmers_freq{
public:
    kmers_freq(kmer k,freq f);
    kmers_freq(const kmers_freq& e);

    inline bool operator==(const kmers_freq& cp){
        return cp._key == this->_key;
    }

    // Adds frequence for this kmer.
    void addFreq(freq f){this->_freq += f;}

    kmers_freq& operator=(const kmers_freq& cp){
        this->_key = cp._key;
        this->_freq = cp._freq;
        return *this;
    }

    kmers_freq& operator+=(const kmers_freq& sin){
        assert_eq(this->_key,sin._key);
        this->_freq += sin._freq;
        return *this;
    }

    kmers_freq operator+(const kmers_freq& cp){
        assert(this->_key==cp._key);
        return kmers_freq(this->_key,_freq+cp._freq);
    }

    friend std::ostream& operator<<(std::ostream& os,
                                    const kmers_freq& o){
        os<<o._key << '\t' << (int)(o._freq) << std::endl;
        return os;
    }

public:
    kmer                    _key;
    freq                    _freq;
};

}   // namespace barcodeSpace

#endif // KMER_FREQ_H

#ifndef KMER_FREQ_H
#define KMER_FREQ_H
#include "suffix_commons.h"
#include <ostream>
#include <functional>
using std::ostream;
namespace prefixMatching{


/**
  * keep record the frequency of single kmer
*/
class kmers_freq{
public:
    kmers_freq(kmer k,freq f);
    kmers_freq(const kmers_freq& e);

    inline bool operator==(const kmers_freq& cp){
        return cp._key == this->_key;
    }
    // add frequence for this kmer.
    void addFreq(freq f){this->_freq += f;}
    /*
    inline bool operator< (const kmers_freq& cp){
        return this->_key < cp._key;
    }
    inline bool operator<= (const kmers_freq& cp){
        return this->_key <= cp._key;
    }
    inline bool operator> (const kmers_freq& cp){
        return this->_key > cp._key;
    }
    inline bool operator>= (const kmers_freq& cp){
        return this->_key > cp._key;
    }
    inline bool operator!=(const kmers_freq& cp){
        return this->_key != cp._key;
    }
    */
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
    friend ostream& operator<<(ostream& os,const kmers_freq& o){
        os<<o._key<<'\t'<<(int)(o._freq)<<endl;
        return os;
    }
public:
    kmer                    _key;
    freq                    _freq;
};

}

#endif // KMER_FREQ_H

#include "kmer_freq.h"
#include "typdefine.h"

namespace barcodeSpace{

kmers_freq::kmers_freq(kmer k, freq f):_key(k),_freq(f){}
kmers_freq::kmers_freq(const kmers_freq& e){
    this->_key = e._key;
    this->_freq = e._freq;
}
}   //  namespace barcodeSpace

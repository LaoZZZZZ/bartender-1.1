#ifndef CENTER_H
#define CENTER_H
#include "suffix_commons.h"
class Center
{
public:
    Center(size_t);
    virtual bool isSimilar(const Center&) = 0;
    virtual const vector<kmer>& centers()const = 0;
    virtual const kmer& center()const=0;
    size_t klen()const{return this->_klen;}
private:
    size_t                                  _klen;
};

#endif // CENTER_H

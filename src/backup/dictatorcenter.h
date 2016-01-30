#ifndef DICTATORCENTER_H
#define DICTATORCENTER_H
#include "center.h"
#include "suffix_commons.h"
#include "util.h"
namespace prefixMatching{
class dictatorCenter : public Center
{
public:
    dictatorCenter(kmer c,size_t len);
    bool isSimilar(const Center&);
    const vector<kmer>& centers()const{throw 1;}
    const kmer& center()const{return this->_center;}
private:
    kmer                            _center;
    static const size_t             _thres = 2;

};
}
#endif // DICTATORCENTER_H

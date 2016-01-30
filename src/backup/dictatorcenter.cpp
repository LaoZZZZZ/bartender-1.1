#include "dictatorcenter.h"

namespace prefixMatching{
dictatorCenter::dictatorCenter(kmer c,size_t klen):Center(klen),_center(c)
{
}
bool dictatorCenter::isSimilar(const Center& c){
    const kmer& k = c.center();
    size_t total = kmerDistance(this->_center,k,this->klen());
    return total <= dictatorCenter::_thres;
}
}

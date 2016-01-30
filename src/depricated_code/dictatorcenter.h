#ifndef DICTATORCENTER_H
#define DICTATORCENTER_H

#include "typdefine.h"

namespace barcodeSpace{
class DictatorCenter
{
public:
    DictatorCenter(kmer c,size_t len);

    const kmer& center()const{return this->_center;}

    size_t klen()const{return this->_klen;}
    size_t length() const {return this->_klen;}
    int operator[](size_t index) {
        return (_center >> 2 *(_klen - index))& 0x3;
    }
    DictatorCenter(const DictatorCenter& cp) {
        this->_klen = cp.klen();
        this->_center = cp.center();
    }
    DictatorCenter& operator=(const DictatorCenter& cp) {
        _klen = cp.klen();
        _center = cp.center();
        return *this;
    }
private:
    size_t                          _klen;
    kmer                            _center;
};

}   // namespace barcodeSpace
#endif // DICTATORCENTER_H

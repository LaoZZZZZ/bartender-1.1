#include "barcodecounting.h"

namespace prefixMatching{
barcodeCounting::barcodeCounting(size_t klen):_transformer(NULL),_klen(klen)
{
    _transformer = kmers_BitwiseTransform::getInstance();
    assert(_transformer);
}
void barcodeCounting::insert(const BTDnaString& seq){
    kmer k = this->bitwise(seq);
    if(this->_barcode_db.count(k)){
        this->_barcode_db[k] += 1;
    }else
        this->_barcode_db[k] = 1;
}
}

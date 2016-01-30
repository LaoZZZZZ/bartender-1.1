#include "cluster.h"

namespace prefixMatching {

cluster::cluster(const std::shared_ptr<Center>& c,const std::list<freq>& sz):
                 _center(c),_score(0),_size(0),_pointSize(sz)
{

    for(const int& i : _pointSize){
        _size += i;
    }

    kmers_freq centerbarcode(c->center(),this->_size);
    this->_barcodes.push_back(centerbarcode);

}
/**
  * \brief only support naive addition, which should not be incurred at most time
*/
void cluster::addBarcode(const kmers_freq& f){
    this->_barcodes.push_back(f);
    this->_size += f._freq;
}

}

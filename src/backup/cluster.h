#ifndef CLUSTER_H
#define CLUSTER_H
#include <list>
#include <memory>
#include "suffix_commons.h"
#include "center.h"
#include "kmer_freq.h"
#include "kmers_bitwisetransform.h"
using std::shared_ptr;
using std::list;

namespace prefixMatching{


class cluster
{
public:
    cluster(const std::shared_ptr<Center>&,const std::list<freq>&);
    virtual void addBarcode(const kmers_freq&);
    /**
      * combine two clusters into one cluster with single columns
    */
    virtual void merge(const cluster&) = 0;
    /**
      * Representative
    */
    std::shared_ptr<Center>      center()const{return this->_center;}
    /**
      * All barcodes contained in this cluster
      * Including their frequencies
    */
    const list<kmers_freq>&      barcodes()const{return this->_barcodes;}
    /**
      * The score of the center
    */
    double                       centerScore()const{return this->_score;}
    // get total number of barcode in this cluster
    size_t                       size()const{return this->_size;}
    size_t                       columnSize()const{return this->_pointSize.size();}
    const list<freq>&             colums()const{return this->_pointSize;}
    virtual const vector<vector<int>>&    bpFrequency()const{throw 1;}
    friend ostream& operator<<(ostream& os, const cluster& c){
        BTDnaString seq;
        kmers_BitwiseTransform::getInstance()->bitwise_2_seq(c.center()->center(),
                                                             seq,c.center()->klen());
        os<<seq;
        for(const int& i: c.colums()){
              os<<','<<i;
        }


        return os;
    }
protected:
    std::shared_ptr<Center>                   _center;
    list<kmers_freq>                          _barcodes;
    double                                    _score;
    size_t                                    _size;
    list<freq>                                _pointSize;
};
}
#endif // CLUSTER_H

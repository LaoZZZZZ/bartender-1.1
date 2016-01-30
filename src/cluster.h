#ifndef CLUSTER_H
#define CLUSTER_H

#include "dictatorcenter.h"
#include "kmer_freq.h"
#include "kmers_bitwisetransform.h"
#include "typdefine.h"

#include <algorithm>
#include <array>
#include <list>
#include <memory>
#include <string>
#include <vector>

namespace barcodeSpace{

/**
  * A representation of the cluster.
  *
  * TODO(lu): Once we have the frequency table, we do not need the each barcode information anymore.
  * Cause we can caluculate the score for each center and estimate the error directly
  * from the frequency table more efficiently.
  * So Remove the barcode list part and adjust the behavior correspondingly.
  * 1.Probably consensus center is better. since it is more efficient to update when merging clusters.
  * 2.Change the score calculation part.
  * 3.Probably we don't need the inheritance here. Work out the detail.
*/
class cluster
{
public:
    cluster(const std::shared_ptr<DictatorCenter>&,const std::vector<freq>&);
    cluster(const std::shared_ptr<DictatorCenter>& center, const std::vector<freq>& time_points,
            const std::vector<std::array<int, 4>>& frequency_table, int cluster_id);
    static int MaximumID() {return ClusterCount;}
    size_t barcodeLength() const {return _center_len;}
    /**
      * Combines two clusters into one cluster for single time points
    */
    virtual void merge(const cluster&) = 0;
    /**
      * Combines two clusters that come from different time points
      */
    virtual void combine(const cluster&) = 0;
    /**
      * Cluster representative.
    */
    std::shared_ptr<DictatorCenter>      center()const{return this->_center;}
    /**
      * Cluster name
    */
    int ClusterID() const {return this->_cid;}

    // Used when loading from the file.
    void SetClusterID(int id) {this->_cid = id;}
    /**
      * All barcodes contained in this cluster
      * Including their frequencies.
    */
    const std::list<kmers_freq>&      barcodes()const{return this->_barcodes;}
    
    /**
     Replace the barcode list with the new one
     updates the center and bp frequency matrix.
     */
    void updateBarcodeList(const std::list<kmers_freq>&);

    // Gets total number of barcode in this cluster
    size_t                       size()const{return this->_size;}

    size_t                       columnSize()const{return this->_pointSize.size();}

    const std::vector<freq>&             columns()const{return this->_pointSize;}

    std::vector<freq> columns() {return this->_pointSize;}

    // used for the time when loading from the file
    void SetTimePointFrequency(const std::vector<freq>& time_point){
        this->_pointSize.assign(time_point.begin(), time_point.end());
    }

    const std::vector<std::array<int,4>>&    bpFrequency()const{
        return this->_bpFrequency;
    }

    void SetFrequencyTable(const std::vector<std::array<int,4>>&  table) {
        this->_bpFrequency.assign(table.begin(), table.end());
    }

    friend std::ostream& operator<<(std::ostream& os, const cluster& c) {
        std::string seq;
        kmersBitwiseTransform::getInstance()->bitwise_2_seq(c.center()->center(),
                                                            seq,c.center()->klen());
        os<<seq;
        for(const size_t& i : c.columns()){
              os<<','<<i;
        }
        return os;
    }

    virtual ~cluster() {}
protected:
    // Updates the base pair frequency after merging with another cluster.
    void updateFrequency(const cluster&);
    
    //void updateFrequency(const kmers_freq&);
    void updateFrequency(const kmers_freq&);

    // Updates the center after merging with other cluster.
    void updateCenter() {
        kmer c = 0;
        for (const auto& pos : _bpFrequency) {
            size_t bp = std::max_element(pos.begin(), pos.end()) - pos.begin();
            c |= bp;
            c = c << 2;
        }
        c = c >> 2;
        _center.reset(new DictatorCenter(c, _bpFrequency.size()));
    }

    std::shared_ptr<DictatorCenter>                   _center;
    std::list<kmers_freq>                             _barcodes;
    size_t                                            _size;
    std::vector<freq>                                 _pointSize;
    // The base pair frequency at each position.
    // There are four elements in this vector
    // 0 : A, 1: C, 2: G, 3: T
    std::vector<std::array<int,4>>                    _bpFrequency;
    int                                               _cid;
    size_t 					      _center_len;
    static int                                        ClusterCount;
private:
    // Does not allow copy and assignment.
    cluster(const cluster&);
    cluster& operator=(const cluster&);
};
}
#endif // CLUSTER_H

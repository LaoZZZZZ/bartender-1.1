#ifndef CLUSTERALGORITHM_H
#define CLUSTERALGORITHM_H


#include "cluster.h"
#include "formats.h"
#include "kmer_freq.h"
#include "util.h"

#include <list>
#include <memory>
#include <vector>

namespace barcodeSpace{
class clusterAlgorithm
{
public:
    clusterAlgorithm(size_t cutoff,size_t klen,size_t dist_thres);
    virtual void clusterImp(const std::list<std::shared_ptr<cluster> >& cs) = 0;
    const std::list<std::shared_ptr<cluster>>& clusters()const{return _clusters;}
    virtual ~clusterAlgorithm() {}
protected:
    virtual void reset() = 0;
    bool isSimilar(const std::shared_ptr<cluster>& c1,
                   const std::shared_ptr<cluster>& c2) {
        return kmerDistance(c1->center()->center(),c2->center()->center(),_klen) <= this->_dist_thres;
    }
    size_t clusterDistance(const std::shared_ptr<cluster>& c1,
                           const std::shared_ptr<cluster>& c2) {
        return kmerDistance(c1->center()->center(),c2->center()->center(),_klen);
    }
protected:
    // Splits the clusters into two groups such that most of those large clusters
    // are not similar to each other.
    virtual void cleanSplit(const std::list<std::shared_ptr<cluster> >& cs) = 0;

    size_t                                                        _cutoff;   // cutoff of freq that the barcode could be considered as cluster.
    size_t                                                        _klen;     // length of barcode
    size_t                                                        _dist_thres; // the distance used to tell if two clusters or barcodes are similar.
    std::list<std::shared_ptr<cluster> >                          _clusters; // formed clusters
    std::list<std::shared_ptr<cluster> >                          _antenna;  // those barcodes that frequencey is lower than cutoff.
};

}
#endif // CLUSTERALGORITHM_H

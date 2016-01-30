#ifndef CLUSTERALGORITHM_H
#define CLUSTERALGORITHM_H


#include "barcodecluster.hpp"
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
    typedef BarcodeCluster Cluster;
    clusterAlgorithm(size_t cutoff,size_t klen,size_t dist_thres);
    virtual void clusterImp(const std::list<std::shared_ptr<Cluster> >& cs) = 0;
    const std::list<std::shared_ptr<Cluster>>& clusters()const{return _clusters;}
    virtual ~clusterAlgorithm() {}
protected:
    virtual void reset() = 0;
    bool isSimilar(const std::shared_ptr<Cluster>& c1,
                   const std::shared_ptr<Cluster>& c2) {
        return hammingDist(c1->center(), c2->center()) <= _dist_thres;
        //return kmerDistance(c1->center()->center(),c2->center()->center(),_klen) <= this->_dist_thres;
    }
    size_t clusterDistance(const std::shared_ptr<Cluster>& c1,
                           const std::shared_ptr<Cluster>& c2) {
        //return kmerDistance(c1->center()->center(),c2->center()->center(),_klen);
        return hammingDist(c1->center(), c2->center());
    }
protected:
    // Splits the clusters into two groups such that most of those large clusters
    // are not similar to each other.
    virtual void cleanSplit(const std::list<std::shared_ptr<Cluster> >& cs) = 0;

    size_t                                                        _cutoff;   // cutoff of freq that the barcode could be considered as cluster.
    size_t                                                        _klen;     // length of barcode
    size_t                                                        _dist_thres; // the distance used to tell if two clusters or barcodes are similar.
    std::list<std::shared_ptr<Cluster> >                          _clusters; // formed clusters
    std::list<std::shared_ptr<Cluster> >                          _antenna;  // those barcodes that frequencey is lower than cutoff.
};

}
#endif // CLUSTERALGORITHM_H

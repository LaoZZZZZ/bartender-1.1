#ifndef CLUSTERALGORITHM_H
#define CLUSTERALGORITHM_H

#include <unordered_set>
using std::unordered_set;
#include "suffix_commons.h"
#include "dictatorcluster.h"
#include "dictatorcenter.h"
#include "consensuscluster.h"
#include "kmer_freq.h"
namespace prefixMatching{
class clusterAlgorithm
{
public:
    clusterAlgorithm(size_t cutoff,size_t klen,const list<std::shared_ptr<cluster> >&,CLUSTERTYPE ctype=DICTATOR);
    clusterAlgorithm(size_t cutoff,size_t klen,const list<kmers_freq>&,CLUSTERTYPE ctype=DICTATOR);
    void clusterImp(bool pairwise=true);
    const list<std::shared_ptr<cluster>>& clusters()const{return _clusters;}
private:
    void split(const list<kmers_freq>&);
    void split(const list<std::shared_ptr<cluster>>&);
private:
    size_t                                                        _cutoff; // cut off of freq that the barcode could be considered as cluster
    std::list<std::shared_ptr<cluster> >                          _clusters; // formed clusters
    std::list<std::shared_ptr<cluster> >                          _antenna;  // those barcodes that frequencey is lower than cutoff
    size_t                                                        _klen; // length of barcode
    CLUSTERTYPE                                                   _ctype;
};

}
#endif // CLUSTERALGORITHM_H

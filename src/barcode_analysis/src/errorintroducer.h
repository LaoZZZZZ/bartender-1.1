#ifndef ERRORINTRODUCER_H
#define ERRORINTRODUCER_H

#include "barcodemutationgenerator.h"
#include "cluster.h"
#include "clusteralgorithm.h"

#include <list>
#include <memory>
#include <unordered_map>
namespace barcodeSpace {
/**
  * This class is designed to first assign low frequency cluster to
  * its nearest biggest cluster so that the big cluster could have sequence error.
  * This step is to make the later test more robust.
  * This class also serve
*/
class ErrorIntroducer : public clusterAlgorithm
{
public:
    ErrorIntroducer(size_t cutoff,size_t klen,size_t dist_thres);
    void clusterImp(const std::list<std::shared_ptr<cluster> >&);
private:
    void reset() {_large_cluster_table.clear(); _clusters.clear(); _antenna.clear();}
    void cleanSplit(const std::list<std::shared_ptr<cluster> >&);
    std::unordered_map<kmer, std::shared_ptr<cluster>>           _large_cluster_table;
    BarcodeMutationGenerator _mutator;
};
}   // namespace barcodeSpace
#endif // ERRORINTRODUCER_H

#ifndef CLUSTERINGWITHTEST_H
#define CLUSTERINGWITHTEST_H

#include "barcodecluster.hpp"
#include "clustermergertester.h"

#include "clusteralgorithm.h"
#include "formats.h"

namespace barcodeSpace {
class ClusteringWithTest : public clusterAlgorithm
{
public:
    ClusteringWithTest(size_t cutoff,size_t klen,
                       size_t dist_thres,
                       std::shared_ptr<ClusterMergerTester> tester);
    void clusterImp(const std::list<std::shared_ptr<Cluster>>&);
private:
   void reset() { _clusters.clear(); _antenna.clear();}
   void split(const std::list<std::shared_ptr<Cluster>>&);
   void cleanSplit(const std::list<std::shared_ptr<Cluster>>&);
   
    std::shared_ptr<ClusterMergerTester>                          _tester;   // the tester that tests whether it should merge.

};
}   // namespace barcodeSpace
#endif // CLUSTERINGWITHTEST_H

#ifndef CLUSTERINGWITHTEST_H
#define CLUSTERINGWITHTEST_H

#include "clustermergertester.h"

#include "clusteralgorithm.h"

namespace barcodeSpace {
class ClusteringWithTest : public clusterAlgorithm
{
public:
    ClusteringWithTest(size_t cutoff,size_t klen,
                       size_t dist_thres = 2,
                       double zvalue = 3.08,
                       bool pool = true);
    void clusterImp(const std::list<std::shared_ptr<cluster>>&);
private:
   void reset() { _clusters.clear(); _antenna.clear();}
   void split(const std::list<std::shared_ptr<cluster>>&);
   void cleanSplit(const std::list<std::shared_ptr<cluster>>&);
   std::unique_ptr<ClusterMergerTester>                          _tester;   // the tester that tests whether it should merge.

};
}   // namespace barcodeSpace
#endif // CLUSTERINGWITHTEST_H

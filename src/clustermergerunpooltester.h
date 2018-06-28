#ifndef CLUSTERMERGERUNPOOLTESTER_H
#define CLUSTERMERGERUNPOOLTESTER_H

#include "clustermergertester.h"
#include "proportiontest.h"

#include <array>
#include <memory>
#include <vector>


// This class is used to test whether to merge two clusters
// based on the proportion test.
// The proportion is calculated according the following formula.
// p1 = # of base pairs that is different majority,
// t1 = total number of base pairs in the first cluster.

// p2 = # of base pairs that are different the majority of cluster 1
// t2 = total number of base pairs in the cluster2
namespace barcodeSpace {
class ClusterMergerUnPoolTester : public ClusterMergerTester
{
public:
    ClusterMergerUnPoolTester(double zvalue);
    bool shouldMerge(const std::vector<std::array<uint64_t,4>>& freq_c1,
                     const std::vector<std::array<uint64_t,4>>& freq_c2);
private:
    std::unique_ptr<HypothesisTester>  _tester;
};
}   // namespace barcodeSpace
#endif // CLUSTERMERGERUNPOOLTESTER_H

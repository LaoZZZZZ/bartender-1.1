#ifndef CLUSTERMERGERPOOLTESTER_H
#define CLUSTERMERGERPOOLTESTER_H

#include "clustermergertester.h"
#include "proportionpooltest.h"

#include <array>
#include <memory>
#include <vector>

namespace barcodeSpace {
class ClusterMergerPoolTester : public ClusterMergerTester
{
public:
    ClusterMergerPoolTester(double zvalue);
    bool shouldMerge(const std::vector<std::array<int,4>>& freq_c1,
                     const std::vector<std::array<int,4>>& freq_c2);
protected:
    std::unique_ptr<HypothesisTester> _tester;
};
}   // namespace barcodeSpace
#endif // CLUSTERMERGERPOOLTESTER_H

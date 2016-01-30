#ifndef CLUSTERMERGERTESTER_H
#define CLUSTERMERGERTESTER_H

#include <array>
#include <memory>
#include <vector>
namespace barcodeSpace {

class ClusterMergerTester
{
public:
    ClusterMergerTester();
    virtual bool shouldMerge(const std::vector<std::array<int,4>>& freq_c1,
                             const std::vector<std::array<int,4>>& freq_c2) = 0;
    virtual ~ClusterMergerTester() {}
protected:
    std::vector<std::array<int, 4>> PoolFrequencyTable(const std::vector<std::array<int,4>>& freq_c1,
                                                       const std::vector<std::array<int,4>>& freq_c2);
};
}

#endif // CLUSTERMERGERTESTER_H

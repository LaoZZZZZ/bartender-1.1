#include "proportionpooltest.h"
#include "clustermergerpooltester.h"

#include <algorithm>
#include <array>
#include <numeric>
#include <vector>
#include <utility>
using namespace std;
namespace barcodeSpace {
ClusterMergerPoolTester::ClusterMergerPoolTester(double zvalue) : ClusterMergerTester(), _tester(new ProportionPoolTest(zvalue))
{
}
bool ClusterMergerPoolTester::shouldMerge(const std::vector<std::array<uint64_t,4>>& freq_c1,
                                          const std::vector<std::array<uint64_t,4>>& freq_c2) {

    std::pair<uint64_t, uint64_t> c1_stat = getErrorAndBase(freq_c1);

    vector<array<uint64_t, 4>> updated = this->PoolFrequencyTable(freq_c1, freq_c2);
    std::pair<int, int> overall_stat = getErrorAndBase(updated);

    if (c1_stat.first < overall_stat.first/2) {
        c1_stat = getErrorAndBase(freq_c2);
    }
    return !_tester->isSignificant(c1_stat.second, overall_stat.second,
                                   c1_stat.first, overall_stat.first);
}
}   // namespace barcodeSpace


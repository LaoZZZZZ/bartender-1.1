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
bool ClusterMergerPoolTester::shouldMerge(const std::vector<std::array<int,4>>& freq_c1,
                                          const std::vector<std::array<int,4>>& freq_c2) {

    std::pair<int, int> c1_stat = getErrorAndBase(freq_c1);

    vector<array<int, 4>> updated = this->PoolFrequencyTable(freq_c1, freq_c2);
    std::pair<int, int> overall_stat = getErrorAndBase(updated);

    if (c1_stat.first < overall_stat.first/2) {
        c1_stat = getErrorAndBase(freq_c2);
    }
    return !_tester->isSignificant(c1_stat.second, overall_stat.second, c1_stat.first, overall_stat.first);
}
std::pair<int, int> ClusterMergerPoolTester::getErrorAndBase(const std::vector<std::array<int,4>>& freq) {
    int total = 0;
    int error_bases = 0;
    for(const auto& pos : freq) {
        int temp_total = std::accumulate(pos.begin(), pos.end(), 0);
        error_bases += temp_total - *std::max_element(pos.begin(), pos.end());
        total += temp_total;
    }
    return {total, error_bases};
}
}   // namespace barcodeSpace


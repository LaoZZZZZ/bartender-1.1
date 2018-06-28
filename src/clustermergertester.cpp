#include "clustermergertester.h"

#include <array>
#include <algorithm>
#include <numeric>
#include <cassert>
#include <vector>

namespace barcodeSpace {

ClusterMergerTester::ClusterMergerTester(){}
std::vector<std::array<uint64_t, 4>> ClusterMergerTester::PoolFrequencyTable(
                                    const std::vector<std::array<uint64_t,4>>& freq_c1,
                                    const std::vector<std::array<uint64_t,4>>& freq_c2) {
    assert(freq_c1.size() == freq_c2.size());
    std::vector<std::array<uint64_t, 4>> pooled_table;
    for (size_t i = 0; i < freq_c1.size(); ++i) {
        pooled_table.push_back(std::array<uint64_t, 4>());
        for (int j = 0; j < 4; ++j) {
            pooled_table.back()[j] = freq_c1[i][j];
            pooled_table.back()[j] += freq_c2[i][j];
        }
    }
    return pooled_table;
}
std::pair<uint64_t, uint64_t> ClusterMergerTester::getErrorAndBase(
      const std::vector<std::array<uint64_t,4>>& freq) {
    uint64_t total = 0;
    uint64_t error_bases = 0;
    for(const auto& pos : freq) {
        uint64_t temp_total = std::accumulate(pos.begin(), pos.end(), 0);
        error_bases += temp_total - *std::max_element(pos.begin(), pos.end());
        total += temp_total;
    }
    return {total, error_bases};
}
}

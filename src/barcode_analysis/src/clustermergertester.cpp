#include "clustermergertester.h"

#include <array>
#include <cassert>
#include <vector>
namespace barcodeSpace {

ClusterMergerTester::ClusterMergerTester(){}
std::vector<std::array<int, 4>> ClusterMergerTester::PoolFrequencyTable(
                                    const std::vector<std::array<int,4>>& freq_c1,
                                    const std::vector<std::array<int,4>>& freq_c2) {
    assert(freq_c1.size() == freq_c2.size());
    std::vector<std::array<int, 4>> pooled_table;
    for (size_t i = 0; i < freq_c1.size(); ++i) {
        pooled_table.push_back(std::array<int, 4>());
        for (int j = 0; j < 4; ++j) {
            pooled_table.back()[j] = freq_c1[i][j];
            pooled_table.back()[j] += freq_c2[i][j];
        }
    }
    return pooled_table;
}

}

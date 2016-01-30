#include "clustermergerunpooltester.h"


#include <algorithm>
#include <array>
#include <numeric>
#include <vector>
#include <utility>

using std::accumulate;
using std::max_element;
using std::pair;
using std::vector;
using std::array;

namespace barcodeSpace {
ClusterMergerUnPoolTester::ClusterMergerUnPoolTester(double zvalue) : ClusterMergerTester(),_tester(new ProportionTest(zvalue))
{
}

bool ClusterMergerUnPoolTester::shouldMerge(const std::vector<std::array<int,4>>& freq_c1,
                                            const std::vector<std::array<int,4>>& freq_c2) {


    std::vector<std::array<int, 4>> pooled = this->PoolFrequencyTable(freq_c1, freq_c2);

    int error_bases_1 = 0;
    int error_bases_2 = 0;
    int majority_index = 0;
    int num_barcode_1 = accumulate(freq_c1.front().begin(), freq_c1.front().end(), 0);
    int num_barcode_2 = accumulate(freq_c2.front().begin(), freq_c2.front().end(), 0);

    for (size_t i = 0; i < pooled.size(); ++i) {
        majority_index = std::max_element(pooled[i].begin(), pooled[i].end()) - pooled[i].begin();


        error_bases_1 += num_barcode_1 - freq_c1[i][majority_index];
        error_bases_2 += num_barcode_2 - freq_c2[i][majority_index];
    }
    int total_1 = freq_c1.size() * num_barcode_1;
    int total_2 = freq_c2.size() * num_barcode_2;
    if (total_1 < total_2) {
        std::swap(total_1, total_2);
        std::swap(error_bases_1, error_bases_2);
    }
    return !_tester->isSignificant(error_bases_1, error_bases_2, total_1,total_2);
}

}   // namespace barcodeSpace

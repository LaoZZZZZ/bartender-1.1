//
//  clustermergeronesampletester.cpp
//  barcode_project
//
//  Created by luzhao on 1/13/16.
//  Copyright © 2016 luzhao. All rights reserved.
//

#include "clustermergeronesampletester.hpp"

#include "oneproportiontest.hpp"

namespace barcodeSpace {
    ClusterMergerOneSampleTester::ClusterMergerOneSampleTester(double zvalue, double error_rate) :
    ClusterMergerTester(), _tester(new OneProportionTest(zvalue, error_rate)) {
    }
    bool ClusterMergerOneSampleTester::shouldMerge(const std::vector<std::array<uint64_t,4>>& freq_c1,
                                                   const std::vector<std::array<uint64_t,4>>& freq_c2) {
        
        std::vector<std::array<uint64_t, 4>> pool_pmw = this->PoolFrequencyTable(freq_c1, freq_c2);
        std::pair<uint64_t, uint64_t> overall_stat = this->getErrorAndBase(pool_pmw);
        return !_tester->isSignificant(overall_stat.second, 0, overall_stat.first, 0);
    }
}

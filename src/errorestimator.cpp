//
//  errorestimator.cpp
//  barcode_project
//
//  Created by luzhao on 2/27/16.
//  Copyright © 2016 luzhao. All rights reserved.
//

#include "errorestimator.hpp"
#include "split_util.h"

#include <algorithm>
#include <array>
#include <iostream>
#include <numeric>
#include <cassert>
#include <vector>
using namespace std;
namespace barcodeSpace {
    ErrorEstimator::ErrorEstimator(int numb_base_pairs,
                                   double entropy_threshold)
    : _num_base_pairs(numb_base_pairs), _entropy_threshold(entropy_threshold) {
        assert(numb_base_pairs > 0);
    }
    
    double ErrorEstimator::estimate(
                    const vector<std::array<int,4>>& frequency_table,
                    const vector<double>& entropies) {
        assert(entropies.size() == frequency_table.size());
        double error_bases = 0;
        double num_bases = 0;
        vector<std::pair<double, int>> positions;
        for (size_t cur = 0; cur < entropies.size(); ++cur) {
                positions.push_back({entropies[cur], cur});
        }
        sort(positions.begin(), positions.end());
        for (const auto& p : positions) {
            if (p.first > _entropy_threshold) {
                break;
            }
            int total = accumulate(frequency_table[p.second].begin(),
                                  frequency_table[p.second].end(),
                                   0);
            int max_base = *max_element(frequency_table[p.second].begin(),
                                        frequency_table[p.second].end());
            num_bases += total;
            error_bases += (total - max_base);
            if (num_bases > _num_base_pairs) {
                break;
            }
        }
        if (num_bases > 0){
            return error_bases / num_bases;
        }
        return 0;
    }
}   // barcodeSpace

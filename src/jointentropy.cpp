//
//  joinentropy.cpp
//  barcode_project
//
//  Created by luzhao on 4/27/16.
//  Copyright © 2016 luzhao. All rights reserved.
//

#include "jointentropy.hpp"

#include <cassert>
#include <math.h>

namespace barcodeSpace {
    double JointEntropy::pairedEntropy(
        const ConditionFrequencyTable& cond_table){
        
        double result = 0.0;
        for (size_t i = 0; i < 4; ++i) {
            for (size_t j = 0; j < 4; ++j) {
                if (cond_table[i][j] > 0) {
                    result += cond_table[i][j] * (log2(cond_table[i][j]) - _log_total);
                }
            }
        }
        return -1 * result / _total;
    }
    JointEntropy::JointEntropy(int total_num_barcodes){
        reset(total_num_barcodes);
    }
    void JointEntropy::reset(int total) {
        assert(total > 0);
        _total = total;
        _log_total = log2(_total);
    }
}
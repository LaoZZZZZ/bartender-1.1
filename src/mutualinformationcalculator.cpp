//
//  mutualinformationcalculator.cpp
//  barcode_project
//
//  Created by luzhao on 4/21/16.
//  Copyright © 2016 luzhao. All rights reserved.
//

#include "mutualinformationcalculator.hpp"

#include <iostream>
#include <array>
#include <cassert>
#include <math.h>
#include <vector>

using namespace std;

namespace barcodeSpace {
    double MutualInformationCalculator::calculate(
                     const MarginalFrequencyTable& margin_table_1,
                     const MarginalFrequencyTable& margin_table_2,
                     const ConditionFrequencyTable& conditionTable) {
        double mi = 0.0;
        assert(margin_table_1.size() == 4);
        assert(margin_table_2.size() == 4);
        for (size_t bp_1 = 0; bp_1 < 4; ++bp_1) {
            for (size_t bp_2 = 0; bp_2 < 4; ++bp_2) {

                if (margin_table_1[bp_1] != 0 &&
                    margin_table_2[bp_2] != 0 &&
                    conditionTable[bp_1][bp_2] != 0) {
                    mi += conditionTable[bp_1][bp_2] /_total_frequency * (log2(conditionTable[bp_1][bp_2]) + _log_total - log2(margin_table_1[bp_1]) - log2(margin_table_2[bp_2]));
                }

            }
        }
        return mi;
    }
    
}   // namespace barcodeSpace

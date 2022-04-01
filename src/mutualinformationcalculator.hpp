//
//  mutualinformationcalculator.hpp
//  barcode_project
//
//  Created by luzhao on 4/21/16.
//  Copyright © 2016 luzhao. All rights reserved.
//

#ifndef mutualinformationcalculator_hpp
#define mutualinformationcalculator_hpp

#include <array>
#include <math.h>
#include <stdio.h>
#include <vector>
#include <stdint.h>

namespace barcodeSpace {
    // This class is designed to calculate the mutual information
    // between two positions.
    class MutualInformationCalculator {
    public:
        typedef std::array<std::array<uint64_t,4>,4>  ConditionFrequencyTable;
        typedef std::array<uint64_t, 4>   MarginalFrequencyTable;
        
        MutualInformationCalculator(size_t total) : _total_frequency(total) {
            _log_total = log2(_total_frequency);
        }
        double calculate(const MarginalFrequencyTable& margin_table_1,
                         const MarginalFrequencyTable& margin_table_2,
                         const ConditionFrequencyTable& conditionTable);
    private:
        double      _total_frequency;
        double      _log_total;
    };
}   // barcodeSpace

#endif /* mutualinformationcalculator_hpp */

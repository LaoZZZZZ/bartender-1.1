//
//  pairwisemi.cpp
//  barcode_project
//
//  Created by luzhao on 4/23/16.
//  Copyright © 2016 luzhao. All rights reserved.
//

#include "pairwisemi.hpp"

#include "mutualinformationcalculator.hpp"
#include "pairwisemeasurement.hpp"


#include <vector>
namespace barcodeSpace {

    std::vector<PairwiseMeasurement> PairwiseMI::calculate(
        const std::vector<int>& selected_positions,
        const BPFrequencyTracker& bp_tracker) {
        
        
        std::vector<PairwiseMeasurement> result;
        MutualInformationCalculator calculator(bp_tracker.totalFrequency());
        
        for (int i = 0; i < selected_positions.size() - 1; ++i) {
            for (int j = i + 1; j < selected_positions.size(); ++ j) {
                
                result.push_back(PairwiseMeasurement(selected_positions[i],
                                                     selected_positions[j],
                calculator.calculate(bp_tracker.getMarginalFrequency(i),
                    bp_tracker.getMarginalFrequency(selected_positions[j])
                    ,bp_tracker.getConditionFrequency(i, j))
                                 ));
            }
        }
        return result;
    }
}   // namespace barcodeSpace
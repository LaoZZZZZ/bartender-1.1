//
//  pairwisemi.hpp
//  barcode_project
//
//  Created by luzhao on 4/23/16.
//  Copyright © 2016 luzhao. All rights reserved.
//

#ifndef pairwisemi_hpp
#define pairwisemi_hpp

#include "pairwisemeasurement.hpp"
#include "bpfrequencytracker.hpp"

#include <stdio.h>
#include <vector>



namespace barcodeSpace {
    
    class PairwiseMI {
    public:
        PairwiseMI() {}
        std::vector<PairwiseMeasurement> calculate(const std::vector<int>& selected_positions, const BPFrequencyTracker& bp_tracker);
    };
    
}   // namespace barcodeSpace

#endif /* pairwisemi_hpp */

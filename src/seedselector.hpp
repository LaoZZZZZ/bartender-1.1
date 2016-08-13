//
//  seedselector.hpp
//  barcode_project
//
//  Created by luzhao on 12/13/15.
//  Copyright © 2015 luzhao. All rights reserved.
//

#ifndef seedselector_hpp
#define seedselector_hpp

#include "barcodepool.hpp"
#include "bpfrequencytracker.hpp"
#include "typdefine.h"

#include <array>
#include <list>
#include <string>
#include <vector>

namespace barcodeSpace {
    
    // An abstract interface for the seed selection strategy
    class SeedSelector {
    public:
        SeedSelector(int barcode_length);
        virtual std::vector<int> getSeedsPositions(const BPFrequencyTracker& frequency_tracker) = 0;
        int barcodeLength()const {return _barcode_length;}
        virtual ~SeedSelector() {}
    private:
        int _barcode_length;
    };
}
#endif /* seedselector_hpp */

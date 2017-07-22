//
//  entropyseedselector.hpp
//  barcode_project
//
//  Created by luzhao on 4/27/16.
//  Copyright © 2016 luzhao. All rights reserved.
//

#ifndef entropyseedselector_hpp
#define entropyseedselector_hpp

#include "bpfrequencytracker.hpp"
#include "seedselector.hpp"
#include <stdio.h>

#include <array>
#include <vector>
namespace barcodeSpace {
    
    class EntropySeedSelector : public SeedSelector {
    public:
        typedef std::vector<std::array<int, 4>> PositionWeightMatrix;
        // Given the total number of bp for seed
        // this class will pick up the first seed_length bps as the seed pattern
        // If the total number of qualified positions is less than the seed_length,
        // then all qualified position will be included.
        EntropySeedSelector(int barcode_length, double entropy_threshold);
        // In the result value. The first element is the start position. The second element
        // is the span of this part of consecutive seed part.
        virtual std::vector<int> getSeedsPositions(const BPFrequencyTracker& frequency_tracker) {
            CalculateEntropy(frequency_tracker.getMarginalFrequencyTable());
            SelectSeeds();
            return _seeds;
        }
        const std::vector<double>& entropies() const {return _entropy;}
        virtual ~EntropySeedSelector() {}
    private:
        
        void CalculateEntropy(const PositionWeightMatrix& marginal_frequency_table);
        /** Select the seed position by the entropy values.
         *  The seed position will be ordered by the entropy value in descending order.
         */
        void SelectSeeds();
        
        
        std::vector<double> _entropy;
        std::vector<int>    _seeds;
        double              _entropy_threshold;
    };
    
}   // namespace barcodeSpace

#endif /* entropyseedselector_hpp */

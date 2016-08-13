//
//  jointentropyseedselector.hpp
//  barcode_project
//
//  Created by luzhao on 4/27/16.
//  Copyright © 2016 luzhao. All rights reserved.
//

#ifndef jointentropyseedselector_hpp
#define jointentropyseedselector_hpp

#include "bpfrequencytracker.hpp"

#include "jointentropy.hpp"
#include "seedpositionpool.hpp"
#include "seedselector.hpp"

#include <stdio.h>
#include <vector>

namespace barcodeSpace {
    
    /**
     * Give an order of positions that will be used as seed
     * in the clustering process.
     * The method here is to use the pair-wise joint entropy.
     * Contuning stack positions together which have highest joint entropy
     * in the remaining unselected positions.
     */
    class JointEntropySeedSelector :public SeedSelector{
    public:
        typedef PairwiseMeasurement PairwiseEntropy;
        /**
         * barcode_length represents the barcode length 
         * entropy_thres is used to select high diversity positions.
         */
        JointEntropySeedSelector(int barcode_length,
                                 double entropy_thres);
        /**
         * This function is a virtual function derived from SeedSelector
         * This implementation will use the join entropy to order selected nucleotide positions.
         */
        std::vector<int> getSeedsPositions(const BPFrequencyTracker& frequency_tracker);
        
        virtual ~JointEntropySeedSelector() {}
    private:
        // This function implements the simplest strategy using the joint entropy.
        // 1. Pick up the largest joint entropy position pair
        // 2. Remove this two positions from the pool.
        // 3. If there are still unpicked pairs go to step 1.
        void buildPositionFromPool(SeedPositionPool&);
        double  _entropy_thres;
        std::vector<int>    _seed_position;
    };
}   // namespace barcodeSpace

#endif /* jointentropyseedselector_hpp */

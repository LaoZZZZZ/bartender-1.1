//
//  miseedselector.hpp
//  barcode_project
//
//  Created by luzhao on 4/23/16.
//  Copyright © 2016 luzhao. All rights reserved.
//

#ifndef miseedselector_hpp
#define miseedselector_hpp

#include "bpfrequencytracker.hpp"
#include "seedpositionpool.hpp"


#include "seedselector.hpp"
#include <unordered_map>
#include <vector>

namespace barcodeSpace {
    class MISeedSelector : public SeedSelector {
    public:
        MISeedSelector(int barcode_length, double entropy_threshold);
        
        std::vector<int> getSeedsPositions(const BPFrequencyTracker& frequency_tracker);
    private:
        
        std::vector<int> buildSeedSequenceFromPool(SeedPositionPool&);
        
        /*
        std::vector<int> generateSeedSequence(
            const std::vector<int>& positions,
            const std::unordered_map<int, std::vector<std::pair<double,int>>>& mi_matrix);*/
        //std::vector<std::vector<double>>    _pairwise_mi_matrix;
        
        //int _seed_length;
        //int _step;
        double _entropy_threshold;
    };
    
}   // namespace barcodeSpace
#endif /* miseedselector_hpp */

//
//  jointentropyseedselector.cpp
//  barcode_project
//
//  Created by luzhao on 4/27/16.
//  Copyright © 2016 luzhao. All rights reserved.
//

#include "jointentropyseedselector.hpp"

#include "entropyseedselector.hpp"
#include "seedpositionpool.hpp"


#include <vector>

using std::vector;
namespace barcodeSpace {
    
    JointEntropySeedSelector::JointEntropySeedSelector(int barcode_length,
                                                       double entropy_thres) :
        SeedSelector(barcode_length), _entropy_thres(entropy_thres)
    {
    
    }
    
    std::vector<int> JointEntropySeedSelector::getSeedsPositions(const BPFrequencyTracker& frequency_tracker) {
        
        vector<int> candidate_positions = EntropySeedSelector(this->barcodeLength(), _entropy_thres).getSeedsPositions(frequency_tracker);
        
        if (candidate_positions.size() == 1) {
            return candidate_positions;
        }
        // Get the pair-wise entropy between these candidate positions.
        vector<PairwiseMeasurement> joint_entropies;
        JointEntropy    joint_entropy_calculator(static_cast<int>(frequency_tracker.totalFrequency()));
        for (int i = 0; i < candidate_positions.size() - 1; ++i) {
            for (int j = i + 1; j < candidate_positions.size(); ++j) {
                joint_entropies.push_back(
                        PairwiseMeasurement(candidate_positions[i],
                                            candidate_positions[j],
                                            joint_entropy_calculator.pairedEntropy(frequency_tracker.getConditionFrequency(candidate_positions[i], candidate_positions[j]))));
            }
        }
        
        SeedPositionPool    pool(JointEntropyComparison,
                                 joint_entropies);
        
        buildPositionFromPool(pool);
        
        return _seed_position;
    }
    
    void JointEntropySeedSelector::buildPositionFromPool(SeedPositionPool& pool) {
        
        _seed_position.clear();
        while (pool.hasNext()) {
            auto p = pool.next();
            std::cout << p.position_1 << '\t' << p.position_2 << '\t' << p.measurement << std::endl;
            _seed_position.push_back(p.position_1);
            pool.removePosition(p.position_1);
            _seed_position.push_back(p.position_2);
            pool.removePosition(p.position_2);
        }
    }

    
}   //  namespace barcodeSpace
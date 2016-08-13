//
//  miseedselector.cpp
//  barcode_project
//
//  Created by luzhao on 4/23/16.
//  Copyright © 2016 luzhao. All rights reserved.
//

#include "miseedselector.hpp"
#include "mutualinformationcalculator.hpp"
#include "pairwisemeasurement.hpp"
#include "pairwisemi.hpp"
#include "split_util.h"
#include "seedselector.hpp"
#include "entropyseedselector.hpp"

#include <algorithm>
#include <cassert>
#include <unordered_map>
#include <vector>

using std::vector;
using std::unordered_map;
namespace barcodeSpace {
    
    MISeedSelector::MISeedSelector(int barcode_length, double entropy_thres):
        SeedSelector(barcode_length), _entropy_threshold(entropy_thres) {
            assert(barcode_length > 0);
    }
    std::vector<int> MISeedSelector::getSeedsPositions(const BPFrequencyTracker& frequency_tracker) {
        
        MutualInformationCalculator mi_cal(frequency_tracker.totalFrequency());
        vector<int> positions = EntropySeedSelector(barcodeLength(), _entropy_threshold).getSeedsPositions(frequency_tracker);
        
        vector<PairwiseMeasurement> pair_mi = PairwiseMI().calculate(positions,frequency_tracker);
        
        
        /*
        unordered_map<int, vector<std::pair<double,int>>> candidate_positions;

        for (const auto& p : positions) {
            candidate_positions.insert({p,vector<std::pair<double,int>>()});

        }
        
        for (int i = 0; i < positions.size(); ++i) {
            for (int j = i + 1; j < positions.size(); ++j) {
                double mi = mi_cal.calculate(
                    frequency_tracker.getMarginalFrequency(positions[i]),
                    frequency_tracker.getMarginalFrequency(positions[j]),
                    frequency_tracker.getConditionFrequency(positions[i], positions[j]));
                candidate_positions[positions[i]].push_back({mi,positions[j]});
                candidate_positions[positions[j]].push_back({mi,positions[i]});
                }
        }
        for (auto& record : candidate_positions) {
            sort(record.second.begin(), record.second.end());
        }
        
        //return generateSeedSequence(positions, candidate_positions);
        */
        
        SeedPositionPool pool(MutualInformationComparison,pair_mi);
        return buildSeedSequenceFromPool(pool);
    }
    
    
    vector<int> MISeedSelector::buildSeedSequenceFromPool(barcodeSpace::SeedPositionPool & pool) {
        
        vector<int> seed_position;
        while (pool.hasNext()) {
            auto p = pool.next();
            std::cout << p.position_1 << '\t' << p.position_2 << '\t' << p.measurement << std::endl;
            seed_position.push_back(p.position_1);
            pool.removePosition(p.position_1);
            seed_position.push_back(p.position_2);
            pool.removePosition(p.position_2);
        }

        return seed_position;
    }
    
    
    /*
    vector<int> MISeedSelector::generateSeedSequence(const vector<int>& positions, const unordered_map<int, vector<std::pair<double,int>>>& mi_matrix) {
        
        return {};
    }*/
}   //  namespace barcodeSpace

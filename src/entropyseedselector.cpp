//
//  entropyseedselector.cpp
//  barcode_project
//
//  Created by luzhao on 4/27/16.
//  Copyright © 2016 luzhao. All rights reserved.
//

#include "entropyseedselector.hpp"

#include "split_util.h"
#include <array>
#include <vector>

using namespace std;
namespace barcodeSpace {
    EntropySeedSelector::EntropySeedSelector(int barcode_length, double entropy_thres) : SeedSelector(barcode_length), _entropy_threshold(entropy_thres){
        _entropy.assign(barcode_length, 0);
    }
    
    // Calculate the binary entropy.
    void EntropySeedSelector::CalculateEntropy(const PositionWeightMatrix& marginal_frequency_table) {
        for (int i = 0; i < this->barcodeLength(); ++i) {
            _entropy[i] = Entropy(marginal_frequency_table[i]);
        }
    }
    
    void EntropySeedSelector::SelectSeeds() {
        _seeds.clear();
        int cur = 0;
        vector<std::pair<double, int>> qualified_positions;
        while (cur < this->barcodeLength()) {
            if (_entropy[cur] >= _entropy_threshold) {
                qualified_positions.push_back({-1 * _entropy[cur], cur});
            }
            ++cur;
        }
        sort(qualified_positions.begin(), qualified_positions.end());
        for (const auto& p : qualified_positions) {
            _seeds.push_back(p.second);
        } 
    }

}   //  namespace barcodeSpace

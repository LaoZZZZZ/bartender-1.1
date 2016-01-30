//
//  seedselector.cpp
//  barcode_project
//
//  Created by luzhao on 12/13/15.
//  Copyright © 2015 luzhao. All rights reserved.
//

#include "seedselector.hpp"
#include "kmers_dictionary.h"
#include "split_util.h"

#include <array>
#include <cassert>
#include <list>
#include <vector>

using namespace std;
namespace barcodeSpace {

SeedSelector::SeedSelector(size_t seed_length, size_t barcode_length) : _seed_length(seed_length), _barcode_length(barcode_length) {
    _entropy_threshold = Entropy({95,5,0,0});
    _dict = kmersDictionary::getAutoInstance();
    assert(_dict.get());
    _position_weight_matrix.assign(barcode_length, {0,0,0,0});
    _entropy.assign(barcode_length, 0);
    assert(_seed_length > 0);
}


void SeedSelector::CalculatePositionWeightMatrix(const std::pair<std::string, freq>& barcodes) {
    
    assert(barcodes.first.length() == _barcode_length);
    // Go through each barcode and update the position weight matrix.
    for (size_t i = 0; i < _barcode_length; ++i) {
        _position_weight_matrix[i][_dict->asc2dna(barcodes.first[i])] += barcodes.second;
    }
}
    
// Calculate the binary entropy.
void SeedSelector::CalculateEntropy() {
    for (size_t i = 0; i < _barcode_length; ++i) {
        _entropy[i] = Entropy(_position_weight_matrix[i]);
    }
}

// Select consecutive postion based on the entropy value on each position.
// Currently, each seed ONLY contains consecutive position.
// Latter, each seed might contain discrete positions if possible.

void SeedSelector::SelectSeeds() {
    _seeds.clear();
    
    int start = -1;
    int end = -1, cur = 0;
    int total_length = 0;
    while (cur < _barcode_length) {
        if (total_length == _seed_length) {
            break;
        }
        else {
            // break point.
            if (_entropy[cur] < _entropy_threshold) {
                if (start > 0) {
                    size_t diff = end - start + 1;
                    total_length += diff;
                    int extra = total_length - static_cast<int>(_seed_length);
                    extra = extra > 0 ? extra : 0;
                    _seeds.push_back({start, diff - extra});
                }
                while (cur < _barcode_length && _entropy[cur] < _entropy_threshold) {
                    ++cur;
                }
                start = cur;
                end = cur;
                ++cur;
            } else {
                if (start < 0) {
                    start = cur;
                }
                end = cur;
                ++cur;
            }
        }
    }
    if (total_length < _seed_length && start < _barcode_length) {
        int diff = end - start + 1;
        int span = static_cast<int>(_seed_length) - total_length;
        span = span >= diff ? diff : span;
        _seeds.push_back({start, span});
    }
}
}   // namespace barcodeSpace
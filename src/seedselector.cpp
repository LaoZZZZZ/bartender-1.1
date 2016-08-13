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

    SeedSelector::SeedSelector(int barcode_length) :_barcode_length(barcode_length) {
    }

// Select consecutive postion based on the entropy value on each position.
// Currently, each seed ONLY contains consecutive position.
// Latter, each seed might contain discrete positions if possible.
/*
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
}*/
}   // namespace barcodeSpace

//
//  barcodepoolstatistics.cpp
//  barcode_project
//
//  Created by luzhao on 2/27/16.
//  Copyright © 2016 luzhao. All rights reserved.
//

#include "barcodepoolstatistics.hpp"

#include "barcodepool.hpp"
#include "split_util.h"
#include "errorestimator.hpp"

#include <algorithm>
#include <array>
#include <memory>
#include <unordered_map>
#include <vector>

using namespace std;
namespace barcodeSpace {
    BarcodeStatistics::BarcodeStatistics(const std::shared_ptr<BarcodePool>& pool) : _barcode_pool(pool){
        _dict = kmersDictionary::getAutoInstance();
        calculatePWM();
        calculateEntropy();
    }
    void BarcodeStatistics::calculatePWM() {
        if (_barcode_pool->size() > 0) {
            for (size_t i = 0; i < _barcode_pool->size(); ++i) {
                int b_len = _barcode_pool->barcode(i).length();
                if (!_full_pwms.count(b_len)) {
                    _full_pwms.insert({b_len, PositionWeightMatrix(b_len,array<int,4>({0,0,0,0}))});
                    _unique_pwms.insert({b_len, PositionWeightMatrix(b_len,array<int,4>({0,0,0,0}))});
                    _full_entropies.insert({b_len, vector<double>(b_len,0)});
                    _unique_entropies.insert({b_len, vector<double>(b_len,0)});
                }
                for (size_t pos = 0; pos < b_len; ++ pos) {
                    _unique_pwms[b_len][pos][_dict->asc2dna(_barcode_pool->barcode(i)[pos])] += 1;
                    _full_pwms[b_len][pos][_dict->asc2dna(_barcode_pool->barcode(i)[pos])] += _barcode_pool->barcodeFrequency(i)
                    ;
                }
            }
        }    // Go through each barcode and update the position weight matrix.

    }
    
    void BarcodeStatistics::calculateEntropy() {
        for (const auto& pwm : _full_pwms) {
            for (size_t i = 0; i < pwm.first; ++i) {
                _full_entropies[pwm.first][i] = Entropy(pwm.second[i]);
               _unique_entropies[pwm.first][i] = Entropy(_unique_pwms[pwm.first][i]);
            }
        }
    }
}
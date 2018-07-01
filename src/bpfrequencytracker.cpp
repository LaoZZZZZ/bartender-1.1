//
//  bpfrequencytracker.cpp
//  barcode_project
//
//  Created by luzhao on 4/21/16.
//  Copyright © 2016 luzhao. All rights reserved.
//

#include "bpfrequencytracker.hpp"

#include <array>
#include <cassert>
#include <vector>

using std::array;
using std::vector;
namespace barcodeSpace {
    BPFrequencyTracker::BPFrequencyTracker(size_t num_positions) {
        _total_position = num_positions;
        assert(_total_position > 0);
        _totalFrequency = 0;
        _condition_frequency_tracker.assign(num_positions, vector<ConditionFrequencyTable>(num_positions,ConditionFrequencyTable()));
        _self_marginal_frequency.assign(_total_position,array<uint64_t,4>());
       
        for (size_t pos = 0; pos < _total_position; ++pos) {
            for (auto& t : _condition_frequency_tracker[pos]) {
                for (auto& c : t) {
                    c.fill(0);
                }
            }
        }
        for (auto& marg : _self_marginal_frequency) {
            marg.fill(0);
        }
        _dict = kmersDictionary::getAutoInstance();
        _bps_buffer.assign(_total_position, 0);
    }
    void BPFrequencyTracker::addFrequency(const std::string& seq,
                                          size_t freq) {
        assert(seq.length() == _total_position);
        for (size_t pos = 0; pos < _total_position; ++pos) {
            _bps_buffer[pos] = _dict->asc2dna(seq[pos]);
            _self_marginal_frequency[pos][_bps_buffer[pos]] += 1;
        }
        _totalFrequency += freq;
        
        // update the conditional frequency table
        for (size_t pos = 0; pos < _total_position - 1; ++pos) {
            for (size_t n_pos = pos + 1; n_pos < _total_position; ++n_pos) {
                _condition_frequency_tracker[pos][n_pos][_bps_buffer[pos]][_bps_buffer[n_pos]] += freq;
            }
        }
    }
}   // namespace barcodeSpace

//
//  bpfrequencytracker.hpp
//  barcode_project
//
//  Created by luzhao on 4/21/16.
//  Copyright © 2016 luzhao. All rights reserved.
//

#ifndef bpfrequencytracker_hpp
#define bpfrequencytracker_hpp

#include "kmers_dictionary.h"

#include <algorithm>
#include <array>
#include <memory>
#include <numeric>
#include <string>
#include <vector>

namespace barcodeSpace {
    // Keep track marginal and conditional frequency tables of all positions.
    class BPFrequencyTracker {
    public:
        typedef std::array<std::array<int,4>,4>  ConditionFrequencyTable;
        BPFrequencyTracker(size_t num_positions);
        
        // Given the conditional position and target position,
        // this function returns the nucleotide frequency table of the
        // target position given the conditinoal position.
        const ConditionFrequencyTable& getConditionFrequency(int cond_pos,
                                                             int target_pos) const {
            return _condition_frequency_tracker[cond_pos][target_pos];
        }
        
        void addFrequency(const std::string& seq,
                          size_t freq = 1);
        // Get the
        const std::array<int,4>& getMarginalFrequency(int position) const {
            return _self_marginal_frequency[position];
        }
        
        size_t numberOfPositions()const {return _total_position;}
        size_t totalFrequency() const {

            return _self_marginal_frequency.empty()? 0:std::accumulate(_self_marginal_frequency.front().begin(),
                                   _self_marginal_frequency.front().end(),
                                   0U);
        }
        const std::vector<std::array<int, 4>>& getMarginalFrequencyTable() const {return _self_marginal_frequency;}
    private:
        
        // Diction used to transfer nucleotide into integer.
        std::shared_ptr<kmersDictionary>    _dict;
        // Length of barcode
        size_t  _total_position;
        
        // hold the conditional bp frequency for each nucleotide position.
        std::vector<std::vector<ConditionFrequencyTable>>_condition_frequency_tracker;
        // keep track the marginal nucleotide frequency for each position.
        std::vector<std::array<int, 4>>    _self_marginal_frequency;
        
        
        std::vector<int> _bps_buffer;
    };
}   // namespace barcodeSpace

#endif /* bpfrequencytracker_hpp */

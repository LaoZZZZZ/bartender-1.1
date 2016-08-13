//
//  seedpositionpool.cpp
//  barcode_project
//
//  Created by luzhao on 4/28/16.
//  Copyright © 2016 luzhao. All rights reserved.
//

#include "seedpositionpool.hpp"

#include <iostream>
#include <stdexcept>

using namespace std;
namespace barcodeSpace {
    
        SeedPositionPool::SeedPositionPool(
        std::function<bool(const PairwiseMeasurement& ,const PairwiseMeasurement&)> fun_obj,
                                       const std::vector<PairwiseMeasurement>& positions)
    : _has_next(true), _next(-1,-1,-1), _p_queue(positions.begin(), positions.end(),fun_obj) {
        _removed_pairs.insert({-1,std::list<PairwiseMeasurement>()});
    }
    PairwiseMeasurement SeedPositionPool::getNextPosition() {
        while (!_p_queue.empty()) {
            PairwiseMeasurement temp = _p_queue.top();
            _p_queue.pop();
            if (_removed_pairs.count(temp.position_1)){
                _removed_pairs[temp.position_1].push_back(temp);
            } else if (_removed_pairs.count(temp.position_2)){
                _removed_pairs[temp.position_2].push_back(temp);
            } else {
                _selected_pairs.push_back(temp);
                return temp;
            }
        }
        throw std::out_of_range("Running out of measurement pairs in the paired pool!");
    }
    /* This function will add all pair measurements that contains this position back to the priority queue.
     * Exception: the pair measurement contains this position but also contains the removed positions will be not added back!
     */
    void SeedPositionPool::addPositionBack(int pos) {
        if (_removed_pairs.count(pos)) {
            for(const auto& pm :_removed_pairs[pos]) {
                // Get the other position in this pair measurement.
                int key = pm.position_1 == pos? pm.position_2 : pm.position_1;
                // if the other position is in the removed list, do nothing
                if (_removed_pairs.count(key)) {
                    _removed_pairs[key].push_back(pm);
                } else {
                   _p_queue.push(pm);
                }
            }
            _removed_pairs.erase(pos);
            if (isValid()){
                _p_queue.push(_next);
            }
        }
    }
}   // namespace barcodeSpace
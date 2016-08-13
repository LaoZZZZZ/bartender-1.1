//
//  seedpositionpool.hpp
//  barcode_project
//
//  Created by luzhao on 4/28/16.
//  Copyright © 2016 luzhao. All rights reserved.
//

#ifndef seedpositionpool_hpp
#define seedpositionpool_hpp

#include <algorithm>
#include <functional>
#include <list>
#include <queue>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>
#include <vector>


#include "pairwisemeasurement.hpp"

namespace barcodeSpace {
    
    // Contains the pairwise measurement(mi or joint entropy) between two positions
    
    /**
     * This class is a container that holds a pool of positions ordered by
     * the given measurement(i.e. entropy or mutual information).
     * A valid sequence of calls
     * SeedPositionPool pool; while(pool.hasNext()) { pool.next();}
     * Once removePosition is called, hasNext() has to be called before calling any other function.
     */
    class SeedPositionPool {
    public:
        SeedPositionPool(std::function<bool(const PairwiseMeasurement&, const PairwiseMeasurement&)>fun_obj,
                         const std::vector<PairwiseMeasurement>& );
        bool hasNext() {
            
            if (isValid()) {
                return true;
            }
            try {
                _next = getNextPosition();
                _has_next = true;
            } catch (const std::exception& e) {
                _has_next = false;
            }
            return _has_next;
            
        }
        PairwiseMeasurement next() {
            PairwiseMeasurement temp = _next;
            _next.position_1 = -1;
            return temp;
        }
        /**
         * Put this position in the blacklist.
         */
        void removePosition(int pos) {

            if (!_removed_pairs.count(pos)) {
                _removed_pairs.insert({pos, std::list<PairwiseMeasurement>()});
            }
        }
        
        /* This function will add all pair measurements that contains this position back to the priority queue.
         * Exception: the pair measurement contains this position but also contains the removed positions will be not added back!
        */
        void addPositionBack(int pos);
        /**
         * Reset the internal state to the initial state
         */
        void reset() {
            for (const auto& p : _removed_pairs) {
                for (const auto& pm : p.second) {
                    _p_queue.push(pm);
                }
            }
            _removed_pairs.clear();
            for (const auto& pm : _selected_pairs) {
                _p_queue.push(pm);
            }
            _selected_pairs.clear();
        }
        const std::vector<PairwiseMeasurement>& selectedPairs() const {
            return _selected_pairs;
        }
    private:
        bool isValid() {
            return !_removed_pairs.count(_next.position_1) &&
                  !_removed_pairs.count(_next.position_2);
        }
        /**
         * This function will return the top pairs that both positions are not removed.
         */
        PairwiseMeasurement getNextPosition();
        
        bool                _has_next;
        PairwiseMeasurement _next;
        
        std::vector<PairwiseMeasurement>    _selected_pairs;
        std::unordered_map<int, std::list<PairwiseMeasurement>> _removed_pairs;
        std::priority_queue<PairwiseMeasurement,
                            std::vector<PairwiseMeasurement>,
                            std::function<bool(const PairwiseMeasurement&,                           const PairwiseMeasurement&)>>  _p_queue;
    };
}   // namespace barcodeSpace

#endif /* seedpositionpool_hpp */

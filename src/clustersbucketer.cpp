//
//  clustersbucketer.cpp
//  barcode_project
//
//  Created by luzhao on 1/27/16.
//  Copyright © 2016 luzhao. All rights reserved.
//

#include "clustersbucketer.hpp"
#include "barcodeindexer.hpp"

#include <cassert>
#include <math.h>
#include <vector>
using namespace std;
namespace barcodeSpace {
    
    ClusterBucketer::ClusterBucketer(const std::vector<int>& seed_pos,
                                     size_t seed_len,
                                     size_t step) : _seed_positions(seed_pos), _seed_len(seed_len), _step(step){
	assert(step <= seed_len);
        splitSeedPositions();
        size_t total = static_cast<size_t>(pow(4,_seed_len));
        _bins.assign(total,CBin());
    }

    void ClusterBucketer::shatter(const std::list<std::shared_ptr<BarcodeCluster>>& clusters) {
        for (auto& bin : _bins) {
            bin.clear();
        }
        if (_cur == _indexers.size()) {
            return;
        }
        for (const auto& c : clusters) {
            _bins[_indexers[_cur]->GetIndex(c->center())].push_back(c);
        }
        _cur += 1;
    }

    void ClusterBucketer::splitSeedPositions() {
        size_t cur = 0;
        while (cur < _seed_positions.size()) {
            size_t end = min(_seed_positions.size(), cur + _seed_len);
            // reach the last truncated seed. extend its left bound such that the seed length is equal to seed length.
            vector<int> seed_range;
            if (end == _seed_positions.size()) {
                seed_range.assign(_seed_positions.end() - _seed_len, _seed_positions.end());
                cur = _seed_positions.size(); // exit the loop in next check
            } else {
                seed_range.assign(_seed_positions.begin() + cur, _seed_positions.begin() + end);
            }
            std::unique_ptr<BarcodeIndexer> temp(new BarcodeIndexer(seed_range));
            _indexers.push_back(std::move(temp));
            cur += _step;
        }
        _cur = 0;
    }
}   // namespace barcodeSpace

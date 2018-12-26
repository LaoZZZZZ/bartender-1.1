//
//  clustertabledumper.cpp
//  barcode_project
//
//  Created by luzhao on 12/21/15.
//  Copyright © 2015 luzhao. All rights reserved.
//

#include "clustertabledumper.hpp"
#include "minicsv.h"
#include "split_util.h"

#include <algorithm>
#include <string>

using namespace std;
using namespace csv;
namespace barcodeSpace {
    void ClusterTableDumper::generateHeader(size_t num_time_points) {
        _cash.push_back("Cluster.ID");
        _cash.push_back("Center");
        _cash.push_back("Cluster.Score");
        
        std::stringstream buffer;
        for(size_t time_point = 1; time_point <= num_time_points; ++time_point) {
            buffer << "time_point_" << time_point;
            _cash.push_back(buffer.str());
            buffer.str("");
        }
        
    }

    double ClusterTableDumper::maxEntropy(const std::vector<std::array<uint64_t, 4>>& frequency_table) {
        double score(0);
        for (const auto& bp_freq : frequency_table) {
            score = fmax(score, Entropy(bp_freq));
        }
        return score;
    }
    
    void ClusterTableDumper::run() {
        int id = 0;
        for (const auto& c : _clusters) {
            ++id;
            _cash[0] = std::to_string(id);
            //_cash[1] = decodeKmer(c->center()->center(), c->center()->klen());
            _cash[1] = c->center();
            //_cash[1] = to_string(c->center()->center());
            _cash[2] = std::to_string(maxEntropy(c->bpFrequency()));
            size_t pos = 3;
            assert(_cash.size() - pos == c->columns().size());
            for (const auto& f : c->columns()) {
                _cash[pos] = std::to_string(f);
                ++pos;
            }
            _out.Write(_cash);
        }
    }
}   // namespace barcodeSpace

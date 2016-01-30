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

    double ClusterTableDumper::maxEntropy(const std::vector<std::array<int, 4>>& frequency_table) {
        double score(0);
        for (const auto& bp_freq : frequency_table) {
            score = fmax(score, Entropy(bp_freq));
        }
        return score;
    }
}   // namespace barcodeSpace

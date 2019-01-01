//
//  qualitytabledumper.cpp
//  barcode_project
//
//  Created by luzhao on 12/21/15.
//  Copyright © 2015 luzhao. All rights reserved.
//

#include "qualitytabledumper.hpp"

#include <cassert>
#include <sstream>
#include <string>

using namespace std;
namespace barcodeSpace {
    void QualityTableDumper::generateHeader() {
        _cash.push_back("Cluster.ID");
        _cash.push_back("Base");
        std::stringstream buffer;
        for(size_t pos = 1; pos <= _max_barcode_length; ++pos) {
            buffer << "position_" << pos;
            _cash.push_back(buffer.str());
            buffer.str("");
        }
    }
    
    void QualityTableDumper::WritePWM(int cluster_id, const std::vector<std::array<uint64_t, 4>>& pwm) {
        
        assert(_max_barcode_length >= pwm.size());
        _cash[0] = to_string(cluster_id);
        
        for (int bp = 0; bp < 4; ++bp) {
            _cash[1] = _dict->dna2asc(bp);
            size_t pos = 0;
            for (; pos < pwm.size(); ++pos) {
                _cash[pos + 2] = to_string(pwm[pos][bp]);
            }
            for(;pos < _max_barcode_length; ++pos) {
                _cash[pos + 2] = "0";
            }
            _out.Write(_cash);
        }
    }
 
    void QualityTableDumper::run() {
        int id = 0;
        for (const auto& c : _clusters) {
            ++id;
            WritePWM(id, c->bpFrequency());
        }
    }
}

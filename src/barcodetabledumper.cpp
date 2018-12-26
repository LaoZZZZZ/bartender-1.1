//
//  barcodetabledumper.cpp
//  barcode_project
//
//  Created by luzhao on 12/21/15.
//  Copyright © 2015 luzhao. All rights reserved.
//


#include "barcodetabledumper.hpp"
#include "barcodepool.hpp"

#include <string>
#include <vector>

using namespace std;
namespace barcodeSpace {
    void BarcodeTableDumper::run() {
        int id = 0;
        for (const auto& c : _clusters) {
            ++id;
            _cash[2] = to_string(id);
            for (const auto& b : c->barcodes()) {
                _cash[0] = _pool->barcode(b);
                _cash[1] = to_string(_pool->barcodeFrequency(b));
                _out.Write(_cash);
            }
        }
    }
}


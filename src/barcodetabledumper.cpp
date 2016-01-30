//
//  barcodetabledumper.cpp
//  barcode_project
//
//  Created by luzhao on 12/21/15.
//  Copyright © 2015 luzhao. All rights reserved.
//


#include "barcodetabledumper.hpp"
#include "barcodepool.hpp"
#include "rawdatainfo.hpp"

#include <string>
#include <vector>

using namespace std;
namespace barcodeSpace {
    void BarcodeTableDumper::writeBarcodeLine(int cluster_id, const std::vector<size_t>& barcodes,
                                              const std::shared_ptr<BarcodePool>& pool) {
        _cash[2] = to_string(cluster_id);
        for (const auto& b : barcodes) {
            _cash[0] = pool->barcode(b);
            _cash[1] = to_string(pool->barcodeFrequency(b));
            _out.Write(_cash);
        }
    }
 
}

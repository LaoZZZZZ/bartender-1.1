//
//  barcodetabledumper.cpp
//  barcode_project
//
//  Created by luzhao on 12/21/15.
//  Copyright © 2015 luzhao. All rights reserved.
//

#include "barcodetabledumper.hpp"
#include "rawdatainfo.hpp"

#include <string>
using namespace std;
namespace barcodeSpace {
    void BarcodeTableDumper::writeBarcodeLine(int cluster_id, const std::string& barcode, const list<string>& line_info) {
        _cash[1] = barcode;
        _cash[2] = to_string(cluster_id);
        for (const auto& l : line_info) {
            _cash[0] = l;
            _out.Write(_cash);
        }
    }
 
}

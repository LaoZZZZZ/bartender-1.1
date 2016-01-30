//
//  countbarcodeprocessor.cpp
//  barcode_project
//
//  Created by luzhao on 12/16/15.
//  Copyright © 2015 luzhao. All rights reserved.
//

#include "countbarcodeprocessor.hpp"

using namespace std;
namespace barcodeSpace {
    CountBarcodeProcessor::CountBarcodeProcessor(const std::string& filename, bool header) :
        RawBarcodeProcessor(filename, header) {
    }

    void CountBarcodeProcessor :: parseFile() {
        vector<string> row;
        while (_csv_reader->nextRow(&row)) {
            assert(row.size() == 2);
            _original_sequence.push_back({row.front(), stoi(row[1])});
            row.clear();
        }
    }
}   // namespace barcodeSpace

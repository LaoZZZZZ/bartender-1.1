//
//  rawbarcodeloader.cpp
//  barcode_project
//
//  Created by luzhao on 1/26/16.
//  Copyright © 2016 luzhao. All rights reserved.
//

#include "rawbarcodeloader.hpp"
#include "util.h"
#include <algorithm>
#include <string>
#include <vector>

using namespace std;
namespace barcodeSpace {
    RawBarcodeLoader::RawBarcodeLoader(const string& file) {
        CSVReader<std::string>* csv_reader_ptr = new CSVReader<std::string>(file, false);
        assert(csv_reader_ptr);
        _csv_reader.reset(csv_reader_ptr);
        _barcode_length_range.first = INT_MAX;
        _barcode_length_range.second = 0;
    }
    void RawBarcodeLoader::process() {
        vector<string> row;
        while (_csv_reader->nextRow(&row)) {
            assert(row.size() == 2);
            const std::string rawBarcode = toUpper(row.front());
            if (_table.find(rawBarcode) == _table.end()) {
                const string revComplement = reverseComplement(rawBarcode);
                if (_table.find(revComplement) == _table.end()) {
                    _table.insert({row.front(),{row[1]}});
                } else {
                    if (isDnaSequence(row.back())) {
                        _table[revComplement].push_back(reverseComplement(row.back()));
                    } else {
                        _table[revComplement].push_back(row[1]);
                    }
                }
            } else {
                _table[rawBarcode].push_back(row[1]);
            }
            _barcode_length_range.first = std::min(row[0].length(), _barcode_length_range.first);
            _barcode_length_range.second = std::max(row[0].length(), _barcode_length_range.second);
            row.clear();
        }
    }
    
}
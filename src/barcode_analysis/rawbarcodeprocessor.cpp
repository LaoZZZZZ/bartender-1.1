//
//  rawbarcodeprocessor.cpp
//  barcode_project
//
//  Created by luzhao on 12/14/15.
//  Copyright © 2015 luzhao. All rights reserved.
//

#include "rawbarcodeprocessor.hpp"
#include "csvreader.h"
#include "sequenceencoder.hpp"

#include <cassert>
#include <list>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;
namespace barcodeSpace {
    RawBarcodeProcessor::RawBarcodeProcessor(const std::string& filenames, bool header) :  InputProcessor(){
        CSVReader<std::string>* csv_reader_ptr = new CSVReader<std::string>(filenames, header);
        assert(csv_reader_ptr);
        _csv_reader.reset(csv_reader_ptr);
    }
    
    void RawBarcodeProcessor::parseFile() {
        vector<string> row;
        SequenceEncoder encoder;
        while (_csv_reader->nextRow(&row)) {
            assert(row.size() == 2);
            size_t blen = row.front().length();
            while (_barcode_tables.size() < blen + 1) {
                _barcode_tables.push_back({});
                _barcode_2_line.push_back({});
            }
            kmer k = encoder.encode(row.front());
            if (_barcode_tables[blen].find(k) == _barcode_tables[blen].end()) {
                _barcode_tables[blen].insert({k,1U});
                _barcode_2_line[blen].insert({k,{row[1]}});
            } else {
                _barcode_tables[blen][k] += 1;
                _barcode_2_line[blen][k].push_back(row[1]);
            }
            row.clear();
        }
    }

    
}
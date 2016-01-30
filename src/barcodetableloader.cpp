//
//  barcodetableloader.cpp
//  barcode_project
//
//  Created by luzhao on 1/21/16.
//  Copyright © 2016 luzhao. All rights reserved.
//

#include "barcodetableloader.hpp"
#include "kmer_freq.h"

#include <string>
#include <list>
#include <unordered_map>

using namespace std;
namespace barcodeSpace {
    BarcodeTableLoader::BarcodeTableLoader(const string& file_name)
    : _reader(file_name, true), _total_barcode(0) {
    }
    
    bool BarcodeTableLoader::nextBarcodeList(std::list<kmers_freq> &barcode_list, int id) {
        barcode_list.clear();
        string s_id = std::to_string(id);
        vector<string> row;
        unordered_map<kmer, kmers_freq> barcode_table;
        while(_reader.peek(row)) {
            if (row[2] == s_id) {
                kmer k = seq_encoder.encode(row[1]);
                if (barcode_table.find(k) == barcode_table.end()) {
                    barcode_table.insert({k, kmers_freq(k, 1)});
                } else {
                    barcode_table[k]._freq += 1;
                }
            }
            _reader.skip();
            row.clear();
        }
        for (const auto& p : barcode_table) {
            barcode_list.push_back(p.second);
        }
        return true;
    }
    
}

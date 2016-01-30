//
//  sequencecounter.cpp
//  barcode_project
//
//  Created by luzhao on 12/31/15.
//  Copyright © 2015 luzhao. All rights reserved.
//

#include "sequencecounter.hpp"

namespace barcodeSpace {
    SequenceCounter::SequenceCounter() : _total(0) {
    }
    void SequenceCounter::insert(const std::string &seq, size_t line) {
        
        kmer k = _coder.encode(seq);
        while (_barcode_table.size() < seq.length() + 1) {
            _barcode_table.push_back(std::unordered_map<kmer, std::list<size_t> >());
        }
        if (_barcode_table[seq.length()].find(k) == _barcode_table[seq.length()].end()) {
            _barcode_table[seq.length()].insert({k, {line}});
        } else {
            _barcode_table[seq.length()][k].push_back(line);
        }
        ++_total;
    }
    
}   // namespace barcodeSpace

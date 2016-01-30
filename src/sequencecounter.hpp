//
//  sequencecounter.hpp
//  barcode_project
//
//  Created by luzhao on 12/31/15.
//  Copyright © 2015 luzhao. All rights reserved.
//

#ifndef sequencecounter_hpp
#define sequencecounter_hpp

#include "sequenceencoder.hpp"
#include "typdefine.h"

#include <list>
#include <string>
#include <vector>
#include <unordered_map>

namespace barcodeSpace {
    struct stringHasher {
        kmer operator()(const std::string& seq) {
            return coder.encode(seq);
        }
        const static SequenceEncoder coder;
    };

class SequenceCounter {
public:
    SequenceCounter();
    void insert(const std::string& seq, size_t line);
    const std::vector<std::unordered_map<kmer, std::list<size_t>>>& barcodeTable() const {
        return _barcode_table;
    }
    // Total number of sequences
    size_t size() const {return _total;}
private:
    std::vector<std::unordered_map<kmer, std::list<size_t> >> _barcode_table;
    size_t  _total;
    SequenceEncoder _coder;
};
}   // namespace barcodeSpace
#endif /* sequencecounter_hpp */

//
//  barcodeindexer.hpp
//  barcode_project
//
//  Created by luzhao on 1/26/16.
//  Copyright © 2016 luzhao. All rights reserved.
//

#ifndef barcodeindexer_hpp
#define barcodeindexer_hpp

#include "kmers_dictionary.h"

#include <memory>
#include <stdio.h>
#include <string>
#include <vector>

namespace barcodeSpace {
    // Give the index of a barcode by using the selected seed positions.
    class BarcodeIndexer {
    public:
        BarcodeIndexer(const std::vector<int> seed_pos);
        size_t GetIndex(const std::string& barcode) {
            size_t index = 0;
            for (const auto& p : _seed_positions) {
                if (p >= barcode.length()) {
                    throw std::runtime_error("The seed position is larger than the barcode length!");
                }
                index |= _dict->asc2dna(barcode[p]);
                index <<= 2;
            }
            index >>=2;
            return index;
        }
	const std::vector<int>& seeds()const {return _seed_positions;}
    private:
        std::vector<int> _seed_positions;
        std::shared_ptr<kmersDictionary> _dict;
    };
}

#endif /* barcodeindexer_hpp */

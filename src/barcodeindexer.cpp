
//
//  barcodeindexer.cpp
//  barcode_project
//
//  Created by luzhao on 1/26/16.
//  Copyright © 2016 luzhao. All rights reserved.
//

#include "barcodeindexer.hpp"
#include "kmers_dictionary.h"

#include <string>
#include <stdexcept>
using namespace std;
namespace barcodeSpace {
    BarcodeIndexer::BarcodeIndexer(const std::vector<int> seed_pos) : _seed_positions(seed_pos) {
        _dict = kmersDictionary::getAutoInstance();
    }

}
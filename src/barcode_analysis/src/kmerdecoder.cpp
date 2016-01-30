//
//  kmerdecoder.cpp
//  barcode_project
//
//  Created by luzhao on 12/13/15.
//  Copyright © 2015 luzhao. All rights reserved.
//

#include "kmerdecoder.hpp"
#include "kmers_bitwisetransform.h"

#include <cassert>

namespace barcodeSpace {

    KmerDecoder::KmerDecoder(size_t k_len) : _k_len(k_len) {
        _trans = kmersBitwiseTransform::getInstance();
        assert(_trans);
    }
}
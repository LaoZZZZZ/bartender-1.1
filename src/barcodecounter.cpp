//
//  barcodecounter.cpp
//  barcode_project
//
//  Created by luzhao on 12/25/15.
//  Copyright © 2015 luzhao. All rights reserved.
//
#include "barcodecounter.h"
#include <string>
#include "kmers_bitwisetransform.h"

using std::string;

namespace barcodeSpace{

BarcodeCounter::BarcodeCounter(size_t klen):_klen(klen)
{
    this->_trans = kmersBitwiseTransform::getInstance();
}
}

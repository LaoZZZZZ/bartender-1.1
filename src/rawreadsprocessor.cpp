//
//  rawreadsprocessor.cpp
//  barcode_project
//
//  Created by luzhao on 12/16/15.
//  Copyright © 2015 luzhao. All rights reserved.
//

#include "rawreadsprocessor.hpp"
#include "formats.h"

#include <boost/regex.hpp>
#include <string>
using namespace std;
namespace barcodeSpace {
    
    RawReadsProcessor::RawReadsProcessor(const std::string& seq_file,
                                         const boost::regex& pattern,
                                         file_format format,
                                         double quality_threshold) : InputProcessor(), _format(format) {
        
    }
    void RawReadsProcessor::parseFile() {
    }
    
}   // namespace barcodeSpace

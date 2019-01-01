//
//  rawreadsprocessor.cpp
//  barcode_project
//
//  Created by luzhao on 12/16/15.
//  Copyright © 2015 luzhao. All rights reserved.
//

#include "rawreadsprocessor.hpp"
#include "formats.h"

#include <regex>
#include <string>
namespace barcodeSpace {
    
    RawReadsProcessor::RawReadsProcessor(const std::string& seq_file,
                                         const std::regex& pattern,
                                         file_format format,
                                         double quality_threshold) : InputProcessor(), _format(format) {
        
    }
    void RawReadsProcessor::parseFile() {
    }
    
}   // namespace barcodeSpace

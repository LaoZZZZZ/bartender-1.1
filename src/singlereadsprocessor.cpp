//
//  singlereadsprocessor.cpp
//  barcode_project
//
//  Created by luzhao on 1/1/16.
//  Copyright © 2016 luzhao. All rights reserved.
//

#include "singlereadsprocessor.hpp"

#include "pattern.h"
#include "sequence.h"
#include "util.h"

#include <string>
#include <sstream>
namespace barcodeSpace {
    SingleReadsProcessor::SingleReadsProcessor(const std::string& reads_file_name,
                                               std::shared_ptr<BarcodeExtractor> extractor,
                                               file_format format,
                                               const std::string& output,
                                               double qual_thres) :
    _extractor(extractor), _formats(format), _outprefix(output),
    _barcode_dumper(output + "_barcode.txt", false),
    _total_reads(0), _total_barcodes(0),
    _total_valid_barcodes(0), _quality_threshold(qual_thres){
        _pattern_handler.reset(CreatePatternParser(reads_file_name, format));
    }
    void SingleReadsProcessor::extract() {
        bool done = false;
        bool success = false;
        Sequence read;
        std::stringstream ss;
        while(!done) {
            read.clear();
            size_t line = _pattern_handler->CurrentLine();
            _pattern_handler->parse(read, success, done);
            // If get a read successfully, then extract the barcode from the read.
            if (success) {
                success = _extractor->ExtractBarcode(read);
                
                // If extracted a barcode from the read successfully,
                if (success) {
                    // The average quality is above the threshold.
                    if (qualityCheck(read.quality(), _quality_threshold)) {
                        ss << read.fowardSeq() << ',' << line << std::endl;
                        _barcode_dumper.writeString(ss.str());
                        ++_total_valid_barcodes;
                        ss.str("");
                    }
                    // Keep tract all barcodes.
                    ++_total_barcodes;
                }
                // keep tract all parsed reads.
                ++_total_reads;
            }
        }
        
    }
}   // namespace barcodeSpace

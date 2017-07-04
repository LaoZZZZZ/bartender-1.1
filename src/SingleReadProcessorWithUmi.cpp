//
//  SingleReadProcessorWithUmi.cpp
//  bartender
//
//  Created by luzhao on 7/4/17.
//  Copyright © 2017 luzhao. All rights reserved.
//

#include "SingleReadProcessorWithUmi.hpp"
#include "util.h"
#include <limits.h>

namespace barcodeSpace {
    SingleReadProcessorWithUmi::SingleReadProcessorWithUmi(
        const std::string& reads_file_name,
        std::shared_ptr<BarcodeExtractor> barcodeExtractor,
        std::shared_ptr<UmiExtractor> umiExtractor,
        file_format format,
        const std::string& output,
        double qual_threshold) :
    _barcodeExtractor(barcodeExtractor), _umiExtractor(umiExtractor),
    _formats(format), _outprefix(output),
    _barcode_dumper(output + "_barcode.txt", false),
    _total_reads(0), _total_barcodes(0),
    _total_valid_barcodes(0), _quality_threshold(qual_threshold){
        
    }
    
    void SingleReadProcessorWithUmi::extractAndLogCount(const Sequence& read, const string& umi) {
        std::stringstream ss;
        // The average quality is above the threshold.
        if (qualityCheck(read.quality(), _quality_threshold)) {
            ss << read.fowardSeq() << ',' << umi << std::endl;
            _barcode_dumper.writeString(ss.str());
            // Keep tract all valid barcodes that pass the quality check.
            ++_total_valid_barcodes;
        }
        // keep track all read that has valid barcode in it
        ++_total_barcodes;
    }
    void SingleReadProcessorWithUmi::extract() {
        bool done = false;
        bool success = false;
        Sequence read;
        while(!done) {
            read.clear();
            size_t lineNumber = _pattern_handler->CurrentLine();
            _pattern_handler->parse(read, success, done);
            if (success) {
                // extract umi first.
                string umi;
                // if needs to extrac umi, we need to split the reads into two separate part
                // and try to extract barcode from each of them.
                // And we assume that each read could only have one barcode.
                // That means, we will skip this read if we can find two or more barcodes in it.
                umi = _umiExtractor->extractUmi(read);
                Sequence previous = read.subRead(subReadsBoundaries.front().first, subReadsBoundaries.front().second);
                    Sequence after = read.subRead(
                        subReadsBoundaries.back().first,
                        static_cast<int>(read.length()) - subReadsBoundaries.back().first);
                ExtractionResultType returnTypePrevious = _barcodeExtractor->ExtractBarcode(previous);
                ExtractionResultType returnTypeAfter = _barcodeExtractor->ExtractBarcode(after);
                    
                // This read has at lease two barcode.
                // log this read and skil it
                if (returnTypePrevious != FAIL && returnTypeAfter != FAIL) {
                    std::cerr << "read: " << read.fowardSeq() << " at line " << lineNumber << " has at lease two barcodes" << std::endl;
                        std::cerr << "This read will be skipped!" << std::endl;
                } else {
                    if (returnTypePrevious == FAIL) {
                        if (returnTypeAfter != FAIL) {
                            if (returnTypeAfter == REVERSE_COMPLEMENT) {
                                reverseComplementInplace(umi);
                            }
                            this->extractAndLogCount(after, umi);
                        }
                    } else {
                        if (returnTypePrevious == REVERSE_COMPLEMENT) {
                            reverseComplementInplace(umi);
                        }
                        this->extractAndLogCount(previous,umi);
                    }
                }
                // keep tract all parsed reads.
                ++_total_reads;
            }
        }
        
    }
    void SingleReadProcessorWithUmi::initializeReadsBoundaries() {
        if (_umiExtractor.get() != nullptr) {
            if (_umiExtractor->getUmiConfigs().size() != 1) {
                throw new runtime_error("Bartender now only support only one umi!");
            }
        }

        int firstPartStart = 0;
        int firstPartEnd = _umiExtractor->getUmiConfigs()[0].getUmiPosition();
        subReadsBoundaries.push_back({firstPartStart, firstPartEnd});
        
        int secondPartStart =_umiExtractor->getUmiConfigs()[0].getUmiPosition() + _umiExtractor->getUmiConfigs()[0].getUmiLength();
        subReadsBoundaries.push_back({secondPartStart, UINT_MAX});
    }
}
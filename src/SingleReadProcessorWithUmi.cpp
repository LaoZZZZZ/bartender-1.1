//
//  SingleReadProcessorWithUmi.cpp
//  bartender
//
//  Created by luzhao on 7/4/17.
//  Copyright © 2017 luzhao. All rights reserved.
//

#include "SingleReadProcessorWithUmi.hpp"
#include "UmiExtractor.hpp"
#include "util.h"
#include <limits.h>

namespace barcodeSpace {
    SingleReadProcessorWithUmi::SingleReadProcessorWithUmi(
        const std::string& reads_file_name,
        std::shared_ptr<BarcodeExtractor> barcodeExtractor,
        file_format format,
        const std::string& output,
        double qual_threshold,
        std::shared_ptr<UmiExtractor> umiExtractor) : SingleReadsProcessor(reads_file_name, barcodeExtractor, format, output, qual_threshold), _umiExtractor(umiExtractor){
        initializeReadsBoundaries();
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
    
    void SingleReadProcessorWithUmi::processSingleRead(Sequence& read) {
        // We need to split the reads into two separate parts
        // and try to extract barcode from each of them.
        // And we assume that each read could only have one barcode.
        // That means, we will skip this read if we can find two or more barcodes in it.
        size_t lineNumber = _pattern_handler->CurrentLine();
        string umi = _umiExtractor->extractUmi(read);
        Sequence previous = read.subRead(subReadsBoundaries.front().first, subReadsBoundaries.front().second);
        Sequence after = read.subRead(
            subReadsBoundaries.back().first,
            static_cast<int>(read.length()) - subReadsBoundaries.back().first);
        ExtractionResultType returnTypePrevious = this->_barcodeExtractor->ExtractBarcode(previous);
        ExtractionResultType returnTypeAfter = this->_barcodeExtractor->ExtractBarcode(after);
        // This read has at least two barcode.
        // log this read and skip it since we assume there is only one barcode per read.
        if (returnTypePrevious != FAIL && returnTypeAfter != FAIL) {
            std::cerr << "read: " << read.fowardSeq() << " at line " << lineNumber << " has at lease two barcodes!" << std::endl;
            std::cerr << "This read will be skipped since bartender assumes there is at most one barcode in single read!" << std::endl;
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
    }
    
    void SingleReadProcessorWithUmi::initializeReadsBoundaries() {
        if (_umiExtractor.get() != nullptr) {
            if (_umiExtractor->getUmiConfigs().size() != 1) {
                throw new runtime_error("Bartender now only support only one umi!");
            }
        }
        const UmiConfig umiConfig = this->_umiExtractor->getUmiConfigs().front();
        int firstPartStart = 0;
        int firstPartEnd = umiConfig.getUmiLength();
        subReadsBoundaries.push_back({firstPartStart, firstPartEnd});
        int secondPartStart = umiConfig.getUmiPosition() + umiConfig.getUmiLength();
        subReadsBoundaries.push_back({secondPartStart, UINT_MAX});
    }
}
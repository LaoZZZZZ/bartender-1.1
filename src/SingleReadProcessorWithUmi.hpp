//
//  SingleReadProcessorWithUmi.hpp
//  bartender
//
//  Created by luzhao on 7/4/17.
//  Copyright © 2017 luzhao. All rights reserved.
//

#ifndef SingleReadProcessorWithUmi_hpp
#define SingleReadProcessorWithUmi_hpp

#include "barcodeextractor.h"
#include "filebuf.h"
#include "formats.h"
#include "patternparser.h"
#include "typdefine.h"
#include "UmiExtractor.hpp"

#include <memory>
#include <string>
#include <vector>
#include <stdio.h>
#include <vector>

namespace barcodeSpace {
    // Base class of processing raw reads file.
    class SingleReadProcessorWithUmi {
    public:
        SingleReadProcessorWithUmi(
            const std::string& reads_file_name,
            std::shared_ptr<BarcodeExtractor> barcodeExtractor,
            std::shared_ptr<UmiExtractor> umiExtractor,
            file_format format,
            const std::string& output,
            double qual_threshold);
        void extract();
    private:
        void extractAndLogCount(const Sequence& read, const string& umi);
        void initializeReadsBoundaries();
    private:
        std::unique_ptr<patternParser>  _pattern_handler;
        std::shared_ptr<BarcodeExtractor>   _barcodeExtractor;
        std::shared_ptr<UmiExtractor> _umiExtractor;
        OutFileBuf _barcode_dumper;
        file_format _formats;
        std::string _outprefix;
        size_t  _total_reads;
        size_t  _total_barcodes;
        size_t  _total_valid_barcodes;
        double  _quality_threshold;
        // For each pair in this vector.
        // the first element is the start position of the sub region.
        // the second element is the lenght of this sub region.
        std::vector<std::pair<int, int>> subReadsBoundaries;
    };
}
#endif /* SingleReadProcessorWithUmi_hpp */

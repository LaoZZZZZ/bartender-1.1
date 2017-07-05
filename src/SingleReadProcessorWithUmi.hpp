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
#include "singlereadsprocessor.hpp"

#include <memory>
#include <string>
#include <vector>
#include <stdio.h>
#include <vector>

namespace barcodeSpace {
    // Base class of processing raw reads file.
    class SingleReadProcessorWithUmi :public SingleReadsProcessor {
    public:
        SingleReadProcessorWithUmi(
            const std::string& reads_file_name,
            std::shared_ptr<BarcodeExtractor> barcodeExtractor,
            file_format format,
            const std::string& output,
            double qual_threshold,
            std::shared_ptr<UmiExtractor> umiExtractor);
    private:
        void extractAndLogCount(const Sequence& read, const string& umi);
        void initializeReadsBoundaries();
        // extracts barcode from single read and dump it to out put buffer.
        void processSingleRead(Sequence& read);
    private:
        std::shared_ptr<UmiExtractor> _umiExtractor;
        // For each pair in this vector.
        // the first element is the start position of the sub region.
        // the second element is the lenght of this sub region.
        std::vector<std::pair<int, int>> subReadsBoundaries;
    };
}
#endif /* SingleReadProcessorWithUmi_hpp */

//
//  rawbarcodeprocessor.hpp
//  barcode_project
//
//  Created by luzhao on 12/14/15.
//  Copyright © 2015 luzhao. All rights reserved.
//

#ifndef rawbarcodeprocessor_hpp
#define rawbarcodeprocessor_hpp

#include "csvreader.h"
#include "inputprocessor.hpp"
#include "typdefine.h"

#include <list>
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace barcodeSpace {
    class RawBarcodeProcessor : public InputProcessor {
    public:
        RawBarcodeProcessor(const std::string& filenames, bool header);
        void process();
    protected:
        std::unique_ptr<CSVReader>  _csv_reader;
    private:
        virtual void parseFile();
    };
}   // namespace barcodeSpace
#endif /* rawbarcodeprocessor_hpp */

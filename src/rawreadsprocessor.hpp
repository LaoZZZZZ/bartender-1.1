//
//  rawreadsprocessor.hpp
//  barcode_project
//
//  Created by luzhao on 12/16/15.
//  Copyright © 2015 luzhao. All rights reserved.
//

#ifndef rawreadsprocessor_hpp
#define rawreadsprocessor_hpp

#include "formats.h"
#include "inputprocessor.hpp"

#include <regex>
#include <string>

namespace barcodeSpace {
class RawReadsProcessor : public InputProcessor {
public:
    RawReadsProcessor(const std::string& seq_file,
                      const std::regex& pattern,
                      file_format format,
                      double quality_threshold);
private:
    void parseFile();
    file_format _format;
};
}   // namespace barcodeSpace
#endif /* rawreadsprocessor_hpp */

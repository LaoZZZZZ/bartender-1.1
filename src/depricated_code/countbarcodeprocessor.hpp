//
//  countbarcodeprocessor.hpp
//  barcode_project
//
//  Created by luzhao on 12/16/15.
//  Copyright © 2015 luzhao. All rights reserved.
//

#ifndef countbarcodeprocessor_hpp
#define countbarcodeprocessor_hpp

#include "rawbarcodeprocessor.hpp"

#include <string>
namespace barcodeSpace {
class CountBarcodeProcessor : public RawBarcodeProcessor {
public:
    CountBarcodeProcessor(const std::string& filename, bool header);
    
private:
    void parseFile();
};
}
#endif /* countbarcodeprocessor_hpp */

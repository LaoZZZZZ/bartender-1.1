//
//  countbarcodeprocessor.hpp
//  barcode_project
//
//  Created by luzhao on 12/16/15.
//  Copyright © 2015 luzhao. All rights reserved.
//

#ifndef countencodedbarcodeprocessor_hpp
#define countencodedbarcodeprocessor_hpp

#include "rawbarcodeprocessor.hpp"

#include <string>
namespace barcodeSpace {
class CountEncodedBarcodeProcessor : public RawBarcodeProcessor {
public:
    CountEncodedBarcodeProcessor(const std::string& filename, bool header);
    
private:
    void parseFile();
};
}
#endif /* countencodedbarcodeprocessor_hpp */

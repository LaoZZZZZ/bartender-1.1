//
//  tablerecord.hpp
//  barcode_project
//
//  Created by luzhao on 12/25/15.
//  Copyright © 2015 luzhao. All rights reserved.
//

#ifndef tablerecord_hpp
#define tablerecord_hpp

#include <string>
#include <vector>

namespace barcodeSpace {
    class TableRecord {
        TableRecord() {}
        virtual std::vector<std::string> toStream() = 0;
    };
}   // namespace barcodeSpace
#endif /* tablerecord_hpp */

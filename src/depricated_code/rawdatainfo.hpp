//
//  rawdatainfo.hpp
//  barcode_project
//
//  Created by luzhao on 12/18/15.
//  Copyright © 2015 luzhao. All rights reserved.
//

#ifndef rawdatainfo_hpp
#define rawdatainfo_hpp

#include <string>
struct RawDataInfo {
    RawDataInfo() {}
    RawDataInfo(const std::string& rawdata,
                size_t lineNumber) : _rawdata(rawdata), _lineNumber(lineNumber) {}
    std::string _rawdata;
    size_t _lineNumber;
};
#endif /* rawdatainfo_hpp */

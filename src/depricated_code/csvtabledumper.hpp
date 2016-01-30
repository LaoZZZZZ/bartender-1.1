//
//  tabledumper.hpp
//  barcode_project
//
//  Created by luzhao on 12/25/15.
//  Copyright © 2015 luzhao. All rights reserved.
//

#ifndef tabledumper_hpp
#define tabledumper_hpp

//#include "minicsv.h"
#include "csvoutput.h"

#include <string>
#include <vector>
namespace barcodeSpace {
    class CSVTableDumper {
    public:
        CSVTableDumper(const std::string& filename) : _out(filename.c_str()){
        }
        void dump(const std::vector<std::string>& line) {
            _out.Write(line);
        }
        ~CSVTableDumper() {}
    private:
        CSVOutput<std::string> _out;
    };
}   // namespace barcodeSpace
#endif /* tabledumper_hpp */

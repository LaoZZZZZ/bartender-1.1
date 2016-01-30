//
//  rawbarcodeloader.hpp
//  barcode_project
//
//  Created by luzhao on 1/26/16.
//  Copyright © 2016 luzhao. All rights reserved.
//

#ifndef rawbarcodeloader_hpp
#define rawbarcodeloader_hpp


#include "csvreader.h"
#include "typdefine.h"

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace barcodeSpace {
    class RawBarcodeLoader {
    public:
        typedef std::unordered_map<std::string, std::vector<std::string>> RawTable;
        RawBarcodeLoader(const std::string& filename);
        void process();
        std::pair<size_t, size_t> lengthRange() const {return _barcode_length_range;}
        RawTable& barcodeTable() {return _table;}
        void clear() {_table.clear();}
    private:
        std::unordered_map<std::string, std::vector<std::string>> _table;
        std::unique_ptr<CSVReader<std::string>>  _csv_reader;
        std::pair<size_t, size_t>  _barcode_length_range;
    };
}   // namespace barcodeSpace
#endif /* rawbarcodeloader_hpp */

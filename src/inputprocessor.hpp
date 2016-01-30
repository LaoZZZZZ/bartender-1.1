//
//  inputprocessor.hpp
//  barcode_project
//
//  Created by luzhao on 12/16/15.
//  Copyright © 2015 luzhao. All rights reserved.
//

#ifndef inputprocessor_hpp
#define inputprocessor_hpp

#include "typdefine.h"

#include <list>
#include <string>
#include <vector>
#include <unordered_map>

// Processing the raw input file.
// Three different file formats:
// 1. Fastq file.
// 2. Raw barcode list.
// 3. Barcode frequent table.
namespace barcodeSpace {
class InputProcessor {
public:
    typedef std::unordered_map<kmer, std::vector<std::string>> barcodeToPrimers;
    InputProcessor() {
        _barcode_tables.clear();
        _barcode_2_line.clear();
    }
    void process() {parseFile();}
    const std::vector<barcodeToPrimers>& BarcodeList() const {return _barcode_2_line;}
    const std::vector<barcodeFreqTable>& BarcodeTable() const {return _barcode_tables;}
protected:
    std::vector<barcodeToPrimers>       _barcode_2_line;
    std::vector<barcodeFreqTable>    _barcode_tables;
private:
    virtual void parseFile() = 0;
};
}   // namespace barcodeSpace
#endif /* inputprocessor_hpp */

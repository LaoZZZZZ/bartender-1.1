//
//  barcodetableloader.hpp
//  barcode_project
//
//  Created by luzhao on 1/21/16.
//  Copyright © 2016 luzhao. All rights reserved.
//

#ifndef barcodetableloader_hpp
#define barcodetableloader_hpp

#include "csvreader.h"
#include "kmer_freq.h"
#include "sequenceencoder.hpp"

#include <list>
#include <string>
#include <vector>


namespace barcodeSpace {
    
    class BarcodeTableLoader {
    public:
        BarcodeTableLoader(const std::string& barcode_file_name);
        bool nextBarcodeList(std::list<kmers_freq>& barcode_list, int id);
        ~BarcodeTableLoader() {
        }
    private:
        SequenceEncoder seq_encoder;
        size_t  _total_barcode;
        CSVReader<std::string>   _reader;
        std::list<std::string> _cach;
    };
}   // namespace barcodeSpace
#endif /* barcodetableloader_hpp */

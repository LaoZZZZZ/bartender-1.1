//
//  countencodedbarcodeprocessor.cpp
//  barcode_project
//
//  Created by luzhao on 12/16/15.
//  Copyright © 2015 luzhao. All rights reserved.
//

#include "countencodedbarcodeprocessor.hpp"
#include "kmerdecoder.hpp"
using namespace std;
namespace barcodeSpace {
    CountEncodedBarcodeProcessor::CountEncodedBarcodeProcessor(const std::string& filename, bool header) :
        RawBarcodeProcessor(filename, header) {
    }

    void CountEncodedBarcodeProcessor :: parseFile() {
        vector<string> row;
        KmerDecoder decoder(20);
        while (_csv_reader->nextRow(&row)) {
            assert(row.size() == 3);
            kmer k = stoull(row.front());
            _original_sequence.push_back({decoder.DNASequence(k), stoi(row[1])});
            row.clear();
        }
    }
}   // namespace barcodeSpace

//
//  singlereadsprocessor.hpp
//  barcode_project
//
//  Created by luzhao on 1/1/16.
//  Copyright © 2016 luzhao. All rights reserved.
//

#ifndef singlereadsprocessor_hpp
#define singlereadsprocessor_hpp

#include "barcodeextractor.h"
#include "filebuf.h"
#include "formats.h"
#include "patternparser.h"
#include "typdefine.h"
#include "sequence.h"

#include <memory>
#include <string>
#include <vector>

namespace barcodeSpace {
    // Base class of processing raw reads file.
    class SingleReadsProcessor {
    public:
        SingleReadsProcessor(const std::string& reads_file_name,
                             std::shared_ptr<BarcodeExtractor> extractor,
                             file_format format,
                             const std::string& output,
                             double qual_threshold);
        void extract();
        double errorRate() const {return _barcodeExtractor->ErrorRate();}
        // Total number of reads in the file.
        size_t TotalReads() const {return this->_total_reads;}
        
        // Total number of barcodes that could be extracted from the reads.
        size_t TotalBarcodes() const {return this->_total_barcodes;}
        
        // Total number of barcoes that the average quality is above the threshold.
        size_t TotalQualifiedBarcodes() const {return this->_total_valid_barcodes;}
        
        virtual ~SingleReadsProcessor() {
            _barcode_dumper.close();
        }
    private:
        virtual void processSingleRead(Sequence& read);
    protected:
        std::unique_ptr<patternParser>  _pattern_handler;
        std::shared_ptr<BarcodeExtractor>   _barcodeExtractor;
        OutFileBuf _barcode_dumper;
        file_format _formats;
        std::string _outprefix;
        size_t  _total_reads;
        size_t  _total_barcodes;
        size_t  _total_valid_barcodes;
        double  _quality_threshold;
    };
}   // namespace barcodeSpace

#endif /* readsprocessor_hpp */

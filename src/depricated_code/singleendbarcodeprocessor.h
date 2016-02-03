#ifndef SINGLEENDBARCODEPROCESSOR_H
#define SINGLEENDBARCODEPROCESSOR_H
#include <string>
#include <memory>
#include <unordered_map>
#include <boost/regex.hpp>
#include "barcodecounter.h"
#include "barcodeextractor.h"
#include "pattern.h"
#include "formats.h"
namespace barcodeSpace {
/**
  * This class is designed to extract all barcodes from a single file.
  * ONLY qualified barcodes will be kept. The current standard is that
  * the average quality of the barcode region should be larger or equal
  * to the specified the threshold if there are quality.
  *
*/
class SingleEndBarcodeProcessor
{
public:
    SingleEndBarcodeProcessor(const std::string& filename,
                              const boost::regex& pattern,
                              file_format format,
                              double qualThreshold,
                              const std::string& preceeding = "",
                              const std::string& suceeding = "");
    // Extracts all barcodes and keep track all qualifed barcodes.
    void Extract();
    
    BarcodeCounter* BarcodeFrequentTable(size_t barcode_length){
        // Can not find the barcode with the given length.
        if (this->_barcodes_frequencies.end() == this->_barcodes_frequencies.find(barcode_length)) {
            return NULL;
        } else {
            return this->_barcodes_frequencies[barcode_length];
        }
    }
    const std::unordered_map<size_t, BarcodeCounter*>& AllBarcodeTables() const {return this->_barcodes_frequencies;}
    
    // Total number of reads in the file.
    int TotalReads() const {return this->_totalReads;}
    
    // Total number of barcodes that could be extracted from the reads.
    int TotalBarcodes() const {return this->_totalBarcodes;}
    
    // Total number of barcoes that the average quality is above the threshold.
    int TotalQualifiedBarcodes() const {return this->_totalValidBarcodes;}
    
    ~SingleEndBarcodeProcessor();
private:
    // Extract the barcode from a single read.
    std::unique_ptr<BarcodeExtractor>           _barcode_extractor;
    // Frequency table categorized by the length of the barcode.
    std::unordered_map<size_t, BarcodeCounter*> _barcodes_frequencies;
    // Parse the raw sequence file.
    std::unique_ptr<patternParser>              _pattern_handler;
    double                                      _qualThreshold;
    int                                         _totalBarcodes;
    int                                         _totalReads;
    int                                         _totalValidBarcodes;
};
}   // namespace barcodeSpace
#endif // SINGLEENDBARCODEPROCESSOR_H

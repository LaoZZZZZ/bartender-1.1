#include "singleendbarcodeprocessor.h"
#include "sequence.h"
#include "util.h"

#include <stdexcept>
namespace barcodeSpace {
SingleEndBarcodeProcessor::SingleEndBarcodeProcessor(const std::string& filename,
                                                     const boost::regex& pattern,
                                                     file_format format,
                                                     double qualThreshold,
                                                     const std::string& preceeding,
                                                     const std::string& suceeding)
    : _qualThreshold(qualThreshold), _totalBarcodes(0), _totalReads(0), _totalValidBarcodes(0)
{
    patternParser* handler = CreatePatternParser(filename, format);
    if (!handler) {
        throw std::runtime_error("Can not create the file handler for loading!\n");
    }
    _pattern_handler.reset(handler);
    BarcodeExtractor* extractor = new BarcodeExtractor(pattern, preceeding, suceeding);
    if (!extractor) {
        throw std::bad_alloc();
    }
    _barcode_extractor.reset(extractor);

}
void SingleEndBarcodeProcessor::Extract() {
    bool done = false;
    bool success = false;
    Sequence read;
    while(!done) {
        read.clear();
        this->_pattern_handler->parse(read, success, done);

        // If get a read successfully, then extract the barcode from the read.
        if (success) {
            success = this->_barcode_extractor->ExtractBarcode(read);

            // If extracted a barcode from the read successfully,
            if (success) {
                 // The average quality is above the threshold.
                if (qualityCheck(read.quality(), this->_qualThreshold)) {
                    size_t len = read.fowardSeq().length();
                    if(this->_barcodes_frequencies.end() == this->_barcodes_frequencies.find(len)){
                        BarcodeCounter* temp = new BarcodeCounter(len);
                        assert(temp);
                        this->_barcodes_frequencies[len] = temp;
                        }
                    this->_barcodes_frequencies[len]->insert(read.fowardSeq());
                    ++_totalValidBarcodes;
                }
                // Keep tract all barcodes.
                ++_totalBarcodes;
            }
            ++_totalReads;

        }
    }
}
SingleEndBarcodeProcessor::~SingleEndBarcodeProcessor() {
    for (auto& pair : _barcodes_frequencies) {
        delete pair.second;
        pair.second = NULL;
    }
    _barcodes_frequencies.clear();
}

}   // namespace barcodeSpace

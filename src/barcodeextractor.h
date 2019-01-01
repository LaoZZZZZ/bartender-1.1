#ifndef BARCODEEXTRACTOR_H
#define BARCODEEXTRACTOR_H

#include "sequence.h"
#include "formats.h"

#include <cassert>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

namespace barcodeSpace{
enum ExtractionResultType {
    FORWARD = 1,
    REVERSE_COMPLEMENT = -1,
    FAIL = 0
};

class BarcodeExtractor
{
public:
    BarcodeExtractor(const std::regex& pattern,
                     const std::string& preceeding = "",
                     const std::string& suceeding = "",
                     size_t parts = 0,
                     const StrandDirection strandDirection = FORWARD_DIRECTION)
                : _pattern(pattern), _preceeding(preceeding),
                  _suceeding(suceeding), _parts(parts),
                  _error_bps(0), _total_bps(0), _strandDirection(strandDirection){
                      assert(!_preceeding.empty());
                      assert(!_suceeding.empty());
                      assert(_parts%2);
                      for (size_t i = 1; i <= _parts/2; ++i) {
                          _random_part_index.push_back(2*i);
                      }
                  }
    // Extracts the barcode region from the read.
    // And change the read in place.
    ExtractionResultType ExtractBarcode(Sequence& read);
    // Extracts the barcode region from the given read,
    // return a new read that only contains the barcode region.
    Sequence ExtractBarcode(const Sequence& read, ExtractionResultType& returnType);
    double ErrorRate() const {
        if (_total_bps == 0) {
            return 0;
        }
        return static_cast<double>(_error_bps) / _total_bps;
    }
private:
    ExtractionResultType isMatched(std::string& sequence, std::string& qual);

    ExtractionResultType extractBarcode(std::string& seq, std::string& qual){
        return this->isMatched(seq, qual);
    }

    void combinePieces(std::string& sequence, std::string& qual, std::smatch& result);

private:
    std::regex      _pattern;
    std::string     _preceeding;
    std::string     _suceeding;
    size_t          _parts;
    size_t          _error_bps;
    size_t          _total_bps;
    std::vector<int> _random_part_index;
    StrandDirection _strandDirection;
};
}   // namespace barcodeSpace
#endif // BARCODEEXTRACTOR_H

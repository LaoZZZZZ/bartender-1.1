#ifndef BARCODEEXTRACTOR_H
#define BARCODEEXTRACTOR_H

#include "sequence.h"

#include <boost/regex.hpp>
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

namespace barcodeSpace{
class BarcodeExtractor
{
public:
    BarcodeExtractor(const boost::regex& pattern,
                     const std::string& preceeding = "",
                     const std::string& suceeding = "",
		     size_t parts = 0)
                : _pattern(pattern), _preceeding(preceeding),
                  _suceeding(suceeding), _parts(parts),
                  _error_bps(0), _total_bps(0){
                      
                      assert(_preceeding.size());
                      assert(_suceeding.size());
                      assert(_parts%2);
                      for (int i = 1; i <= _parts/2; ++i) {
                          _random_part_index.push_back(2*i);
                      }
                  }
    // Extracts the barcode region from the read.
    // And change the read in place.
    bool ExtractBarcode(Sequence& read);
    // Extracts the barcode region from the given read,
    // return a new read that only contains the barcode region.
    Sequence ExtractBarcode(const Sequence& read, bool& success);
    double ErrorRate() const {
        if (_total_bps == 0) {
            return 0;
        }
        return static_cast<double>(_error_bps) / _total_bps;
    }
private:
    int isMatched(std::string& sequence, std::string& qual);

    int extractBarcode(std::string& seq, std::string& qual){
        return this->isMatched(seq, qual);
    }

    // TODO(lu): factor out this part from the source code into the
    // input specification. Since this only works for current barcode parttern.
    // Should come out with a smart way to remove the spacers.
    void combinePieces(std::string& sequence, std::string& qual, boost::smatch& result);

private:
    boost::regex    _pattern;
    std::string     _preceeding;
    std::string     _suceeding;
    size_t          _parts;
    size_t          _error_bps;
    size_t          _total_bps;
    std::vector<int> _random_part_index;
};
}   // namespace barcodeSpace
#endif // BARCODEEXTRACTOR_H

#include "barcodeextractor.h"
#include "util.h"
#include "sequence.h"


#include <algorithm>
#include <boost/regex.hpp>
#include <string>
using std::string;

namespace barcodeSpace {
// if no match, the input read will not be changed
ExtractionResultType BarcodeExtractor::ExtractBarcode(Sequence &read)  {
    return isMatched(read.fowardSeq(), read.quality());
}
    
Sequence BarcodeExtractor::ExtractBarcode(const Sequence& read, ExtractionResultType& extractionResult) {
    Sequence result(read.id(), read.fowardSeq().c_str(), read.quality());
    extractionResult = isMatched(result.fowardSeq(), result.quality());
    return result;
}

ExtractionResultType BarcodeExtractor::isMatched(string& sequence, string& qual){
    boost::smatch result;
    // if we consider forward or both, we first check the forward direction match.
    if (_strandDirection != REVERSE) {
        // only consider full matched sequence
        if(boost::regex_search(sequence, result, _pattern, boost::match_flag_type::match_posix) && !result.empty() && result[0].matched){
            this->combinePieces(sequence, qual, result);
            return FORWARD;
        }
    }
    
    if (_strandDirection != FORWARD) {
        reverseComplementInplace(sequence);
        std::reverse(qual.begin(),qual.end());
        if(boost::regex_search(sequence, result, _pattern) && !result.empty()){
            this->combinePieces(sequence, qual, result);
            return REVERSE_COMPLEMENT;
        }
    }
    return FAIL;
}
    
void BarcodeExtractor::combinePieces(string& sequence, string& qual, boost::smatch& result)  {
    assert(result.size() == _parts + 1);
    string temp;
    string tempqual;
    temp = result[1];
    _error_bps += hammingDist(_preceeding, temp);
    temp = result[_parts];
    _error_bps += hammingDist(_suceeding, temp);
    _total_bps += _preceeding.length();
    _total_bps += _suceeding.length();
    temp.clear();
    for (const auto& index : _random_part_index) {
        temp += result[index];
        tempqual += qual.substr(result.position(index), result[index].length());
    }
    sequence.assign(temp);
    qual.assign(tempqual);
}
}   // namespace barcodeSpace

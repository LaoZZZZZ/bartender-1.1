#include "barcodeextractor.h"
#include "util.h"
#include "sequence.h"


#include <algorithm>
#include <boost/regex.hpp>
#include <string>
using std::string;

namespace barcodeSpace {

bool BarcodeExtractor::ExtractBarcode(Sequence &read)  {
    int position = isMatched(read.fowardSeq(), read.quality());
    return position != -1;
}
    
Sequence BarcodeExtractor::ExtractBarcode(const Sequence& read, bool& success) {
    success = false;
    Sequence result(read.id(), read.fowardSeq().c_str(), read.quality());
    int pos = isMatched(result.fowardSeq(), result.quality());
    success = pos != -1;
    return result;
}

int BarcodeExtractor::isMatched(string& sequence, string& qual){
    boost::smatch result;
    // only consider full matched sequence
    if(boost::regex_search(sequence, result, _pattern, boost::match_flag_type::match_posix) && !result.empty() && result[0].matched){
        this->combinePieces(sequence,qual,result);
        return static_cast<int>(result.position());
    }

    reverseComplementInplace(sequence);
    //string rev_sequence = reverseComplement(sequence);
    std::reverse(qual.begin(),qual.end());
    if(boost::regex_search(sequence, result, _pattern) && !result.empty()){
        this->combinePieces(sequence,qual,result);
        return static_cast<int>(result.position());
    }
    return -1;
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

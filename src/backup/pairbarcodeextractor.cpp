#include "pairbarcodeextractor.h"
using std::unordered_map;
using std::string;
using boost::regex;
namespace prefixMatching {
PairBarcodeExtractor::PairBarcodeExtractor(const string& left_sequence_file,
                                           const string& right_sequence_file,
                                           const regex& left_pattern,
                                           const regex& right_pattern,
                                           int left_barcode_length,
                                           int right_barcode_length,
                                           file_format format,
                                           double qualThres):
    _left_pattern(left_pattern),
    _right_pattern(right_pattern),
    _total_pair(0), _total_valid_pair(0),
    _left_table(left_barcode_length),
    _right_table(right_barcode_length),
    _qualThres(qualThres)
{
    if (format == FASTQ) {
        _left_file_handler.reset(FastqReader::CreateSequenceReader(
                                     left_sequence_file));
        _right_file_handler.reset(FastqReader::CreateSequenceReader(
                                     right_sequence_file));
    } else if (format == FASTA) {
        _left_file_handler.reset(FastaReader::CreateSequenceReader(
                                     left_sequence_file));
        _right_file_handler.reset(FastaReader::CreateSequenceReader(
                                     right_sequence_file));
    } else {
        throw invalid_argument("Does not support the file format!\n");
    }

}

bool PairBarcodeExtractor::extractSingleBarcode(SequenceReader& filehandler,
                                                const boost::regex pattern,
                                                Sequence &read,
                                                bool& success,
                                                bool& done) {
    filehandler.NextRead(read,success,done);
    if (success) {

    }
    return false;
}
/*
bool PairBarcodeExtractor::qualityCheck(const string& qual){
    double average(0);
    int len = qual.length();
    for(int i = 0; i < len; i++){
        average += static_cast<int>(qual[i]);
    }
    return average/len > this->_qualThres;
}
int PairBarcodeExtractor::isMatched(string& sequence,string& qual){
    boost::smatch result;
    if(boost::regex_search(sequence,result,_pattern)){
        this->combinePieces(sequence,qual,result);
        // if average quality is larger thant qualThres_
        if(this->qualityCheck(qual))
            return result.position();
    }
    sequence = reverseComplement(sequence);
    std::reverse(qual.begin(),qual.end());
    if(boost::regex_search(sequence,result,_pattern)){
        this->combinePieces(sequence,qual,result);
        // if average quality is larger thant qualThres_
        if(this->qualityCheck(qual))
            return result.position();
    }
    return -1;
}
// TODO(lu): factor out this part from the source code into the
// input specification. Since this only works for current barcode parttern.
// Should come out with a smart way to remove the barcode's fixed part.
void combinePieces(string& sequence,string& qual,boost::smatch& result){
    assert(result.size() == 10);
    //sequence.clear();
    //cout<<sequence<<endl;
    string temp;
    string tempqual;
    //first barcode
    size_t start = result.position();
    temp += sequence.substr(start+4,result.position(3)-start-4);
    tempqual += qual.substr(start+4,result.position(3)-start-4);
    //second barcode
    start = result.position(3);
    temp += sequence.substr(start +2,result.position(5) - start - 2);
    //third barcode
    tempqual += qual.substr(start +2,result.position(5) - start - 2);

    start = result.position(5);
    temp += sequence.substr(start+2,result.position(7) - start -2);
    // last barcode
    tempqual += qual.substr(start+2,result.position(7) - start -2);

    start = result.position(7);
    temp += sequence.substr(start +2, result.position(9)-start -2);
    tempqual += qual.substr(start +2, result.position(9)-start -2);
    sequence.assign(temp);

    qual.assign(tempqual);
}
*/
} // namespace prefixMatching

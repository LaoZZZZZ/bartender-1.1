#ifndef PAIRBARCODEEXTRACTOR_H
#define PAIRBARCODEEXTRACTOR_H

#include "sequencereader.h"
#include "fastareader.h"
#include "fastqreader.h"

#include <boost/regex.hpp>
#include <string>
#include <unordered_map>


using boost::regex;
namespace prefixMatching {
// This class is used for extracting paired end barcode.
// The following are possible situations:
// Case 1: Both reads have valid barcode, 
// then this forms a valid pair of barcode.
// case 2: One read have valid barcode but the other one does not have,
// then this pair of read will be discarded.
// case 3: Neither of the read has valid barcode,
// then then same with case 2.
class PairBarcodeExtractor {
public:
  // Table that holds the pair information between
  // left barcode and right barcode.
  typedef kmers_sparseHash<kmer,kmer>   pairBarcodeTable;
  PairBarcodeExtractor(const string& left_sequence_file,
                       const string& right_sequence_file,
                       const boost::regex &left_pattern,
                       const regex &right_pattern,
                       int left_barcode_length,
                       int right_barcode_lenth,
                       file_format format,
                       double qualThres);

  const BarcodeTable& leftBarcodeTable()const {
      return _left_table.barcodes();
  }

  const BarcodeTable& rightBarcodeTable()const {
      return _right_table.barcodes();
  }

  const pairBarcodeTable& pairInforTable()const {
      return _pair_barcode_table;
  }

  /*
  const unordered_map<size_t,std::shared_ptr<barcodeCounting>> &bins() const {return this->_stats;}                                                               
  int totalReads() const {return this->_totalReads;}
  int totalValid() const {return this->_totalValid;}
  ~barcodeExtractor();
    */
private:
  bool extractSingleBarcode(SequenceReader& filehandler,
                            const regex pattern,
                            Sequence& read,
                            bool& success,
                            bool& done);
  bool qualityCheck(const string& qual);
  int isMatched(string& sequence, string& qual);
  void combinePeices(const string& seq, const string& qual, boost::smatch& result);
private:
  // Sequence file handler
  std::unique_ptr<SequenceReader>   _left_file_handler;
  std::unique_ptr<SequenceReader>   _right_file_handler;
  // Barcode pattern
  boost::regex     _left_pattern;
  boost::regex     _right_pattern;
  // Stats for whole extraction
  int              _total_pair;
  int              _total_valid_pair;
  // Tables contain pair information
  pairBarcodeTable _pair_barcode_table;
  // Tables that hold barcode frequencies
  barcodeCounting  _left_table;
  barcodeCounting  _right_table;
  double _qualThres;
};
}  // namespace prefixMatching
#endif // PAIRBARCODEEXTRACTOR_H

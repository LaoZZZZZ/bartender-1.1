#ifndef BARCODEEXTRACTOR_H
#define BARCODEEXTRACTOR_H
#include "boost/regex.hpp"

#include "suffix_commons.h"
#include "pattern.h"
#include "barcodecounting.h"
using namespace boost;
namespace prefixMatching{
/**
  * Given read file and a specific pattern patternParser* parser = NULL;
  * this class could extract the matched sequence.
  * And count the frequency of each matched sequence
  * using the google hash table or the standard unordered_map.
*/
class barcodeExtractor
{
public:
    barcodeExtractor(std::string,const boost::regex&,file_format format,double qualThreshold);
    const unordered_map<size_t,std::shared_ptr<barcodeCounting>> &bins() const {return this->_stats;}
    int totalReads() const {return this->_totalReads;}
    int totalValid() const {return this->_totalValid;}
    ~barcodeExtractor();
private:
    void init();
    void extract();
private:
    std::string                                             _readfile;
    boost::regex                                            _pattern;
    std::shared_ptr<patternParser>                          _seqFileHandler;
    int                                                     _totalReads;
    int                                                     _totalValid;
    unordered_map<size_t,std::shared_ptr<barcodeCounting> >  _stats;
    file_format                                              _format;
    double                                                  _qualThres;
};
}
#endif // BARCODEEXTRACTOR_H

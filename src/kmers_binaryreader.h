#ifndef KMERS_BINARYREADER_H
#define KMERS_BINARYREADER_H
#include "binaryinfilebuf.h"
#include <unordered_map>
#include <string>
namespace barcodeSpace{

/**
  * \brief a class that read a specific format of binary file
  */
class kmers_binaryReader
{
public:
    typedef std::unordered_map<kmer,freq> hashSet;
public:
    kmers_binaryReader(std::string filename);
    void read(hashSet&);
    void close(){
        if(_pin)
            delete _pin;
        _pin = NULL;
        this->_isopen = false;
#ifdef DEBUG_DETAIL
        cerr<<this->_filename<<" is closed!"<< endl;
        cerr.flush();
#endif
    }
    //! a destructor
    ~kmers_binaryReader(){
        this->close();
    }
    //! check if the source file is open
    bool isopen() const{ return this->_isopen;}
    std::string classname(){return std::string("kmers_binaryReader");}
private:
    /**
      * \brief the actual reading action take place in this function
      */
    void disk_read(hashSet& set);
    /**
      * \brief open the source file if the file exist and openable
      */
    void open();
private:
    std::string                          _filename;
    BinaryInFileBuf*                _pin;
    bool                            _isopen;
}; //end of class of kmers_binaryReader
} //namespace kmers_ppseq
#endif // KMERS_BINARYREADER_H

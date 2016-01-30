#ifndef BINARYINFILEBUF_H
#define BINARYINFILEBUF_H
#include "suffix_commons.h"
#include <stdexcept>
using std::runtime_error;
namespace barcodeSpace{
/**
  * \brief this class is a buffered class that read binary files
  * \brief with a specific format
*/
class BinaryInFileBuf
{
public:
    BinaryInFileBuf(FILE* in);
    BinaryInFileBuf(std::string);
    void setNewFile(FILE *in);
    string classname(){return std::string("BinaryInFileBuf");}
//read and get
    bool eof()const{ return (this->_cur >= this->_end) && this->_done;}
    bool isOpen() const{return !this->_closed; }
    void close();
    ~BinaryInFileBuf();
    void brewind();
public:
    /**
      * read a strech of byte which is len at most
      * return the actual number of bytes that is read
      */
    size_t gets(char* buf,size_t len);
private:
    BinaryInFileBuf(const BinaryInFileBuf& cp);
    BinaryInFileBuf& operator=(const BinaryInFileBuf& cp);
private:
    void init();
    void nultify();
    void load();
private:
    static const size_t BUF_SZ = 8 * 1024;
private:
    FILE*               _pIn;
    bool                _done;
    bool                _closed;
    char                _buf[BUF_SZ];
    size_t              _cur;
    size_t              _end;

}; //end of class

} //end of namespace
#endif // BINARYINFILEBUF_H

#ifndef DNASTRING_H
#define DNASTRING_H
#include <ostream>
#include <string.h>
namespace prefixMatching{

class DnaString
{
public:
    DnaString();
    void installReverseComp(const DnaString&);
    void installChars(const char*);
    size_t  length()const{return _cur;}
    int operator[](size_t idx)const{
        assert(idx < _cur && this->_cstr);
        int pos = idx/4;
        int off = idx%4;
        return 3&(_cstr[pos]>>(8-off+2));
    }
    std::ostream& operator<< (std::ostream& os)const {
        os << this->toZBuf();
        return os;
    }
    char* toZBuf()const{

        return NULL;
    }
    const UINT8*  codedDna()const{return this->_cstr;}
    DnaString(const DnaString&);
    DnaString& operator=(const DnaString&);
private:
    void add(char c);
private:
    UINT8*                  _cstr;  // the coded equence
    int                     _cur;   // the last position of the dna sequence, coded by 2 bits
};
}
#endif // DNASTRING_H

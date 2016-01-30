#include "dnastring.h"

namespace prefixMatching{
DnaString::DnaString():_cstr(NULL),_cur(0)
{
}
void DnaString::installReverseComp(const DnaString& seq){
    size_t dlen = seq.length();
    int intlen = dlen/4 + (dlen%4)>0;
    if(this->_cstr && this->_cur < dlen){
        delete [](this->_cstr);
        this->_cstr = new UINT8[intlen];
    }
    memset(this->_cstr,0,sizeof(UINT8)*intlen);
    if(dlen){
        this->_cur = 0;
        const UINT8* fw = seq.codedDna();
        for(size_t end = dlen - 1; end >= 0; end--){
            int pos = end/4;
            int off = end%4;
            this->_cstr[this->_cur/4] |= ((fw[pos]>>(8-2*(off + 1))) << (2*(this->_cur%4)));
            this->_cur++;
        }
    }

}
void DnaString::installChars(const char* str){
    if(str){
        size_t len = strlen(str);
        size_t intlen = len/4;
        this->_cur = len;
        intlen += (len%4>0);
        if(this->_cstr)
            delete [](this->_cstr);
        this->_cstr = new UINT8[intlen];
        memset(this->_cstr,0,sizeof(UINT8)*intlen);
        for(size_t start = 0; start < len; start++){
            int pos = start/4;
            int off = start%4;
            this->_cstr[pos] |= (asc2dna[start]<<(8-2*(off + 1)));
        }
    }
}
}

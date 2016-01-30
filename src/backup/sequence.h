#ifndef SEQUENCE_H
#define SEQUENCE_H
#include "suffix_commons.h"
#include "ds.h"
#include "sstring.h"
#include "filebuf.h"
#include "util.h"
namespace prefixMatching{
class sequence
{
public:
    sequence(const char *seq);
    sequence(){this->patFw.clear();}
    /// Return true iff the read (pair) is empty
    bool empty() const {
        return patFw.empty();
    }

    /// Return length of the read in the buffer
    size_t length() const {
        return patFw.length();
    }
    void set(const char* seq){
        this->patFw.clear();
        this->patFw.install(seq);
    }
    const BTDnaString& fowardSeq()const{return this->patFw;}
    BTDnaString& fowardSeq(){return this->patFw;}
    BTString& quality(){return this->qual;}
    const BTString& quality()const{return this->qual;}
    void        reverseComSeq(BTDnaString& rc)const{
        rc.installReverseComp(patFw);
    }
    int  operator[](size_t pos)const{
        return this->patFw[pos];
    }
    void clear(){this->patFw.clear();}
    virtual ~sequence(){}
private:
    void init(const char* seq){
        this->patFw.install(seq);
    }
private:
    BTDnaString                 patFw;            // forward-strand sequence
    BTString                    qual;
};
}
#endif // SEQUENCE_H

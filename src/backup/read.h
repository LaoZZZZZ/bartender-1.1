/*
 * Copyright 2014, Lu Zhao <luzhao1986@gmail.com>
 *
 * This file is part of suffix matching project.
 */

#ifndef READ_H_
#define READ_H_

#include <stdint.h>
#include <sys/time.h>
#include "sequence.h"

namespace prefixMatching {

typedef UINT64 TReadId;
typedef size_t TReadOff;
typedef int64_t TAlScore;


/**
 * A buffer for keeping all relevant information about a single read.
 */
class Read{
public:
	Read() { reset(); }

	Read(const char *nm, const char *seq, const char *ql) { init(nm, seq, ql); }
    ~Read(){
        this->reset();
    }
    BTString& id(){return this->name;}
    BTString& quality(){return this->qual;}
    const BTDnaString& fowardSeq()const{return this->patFw;}
    BTDnaString& fowardSeq(){return this->patFw;}
    size_t length()const{return this->patFw.length();}
private:
    void reset() {
        this->patFw.clear();
        qual.clear();
        name.clear();
    }
    /**
     * Simple init function, used for testing.
     */
    void init(
        const char *nm,
        const char *seq,
        const char *ql)
    {
        reset();
        this->patFw.clear();
        this->patFw.install(seq);
        qual.install(ql);
        assert(this->length() == qual.length());
        if(nm != NULL) name.install(nm);
    }

private:
    BTDnaString                 patFw;            // forward-strand sequence
    BTString                    qual;             // quality values
    BTString                    name;      // read name
};


}
#endif /*READ_H_*/

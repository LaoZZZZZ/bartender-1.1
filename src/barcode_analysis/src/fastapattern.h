/*
 * Copyright 2015, Lu Zhao <luzhao1986@gmail.com>
 *
 * This file is part of bartender project.
 */

#ifndef FASTAPATTERN_H
#define FASTAPATTERN_H
#include "patternparser.h"

namespace barcodeSpace{
class fastaPattern : public patternParser
{
public:
    fastaPattern(const string&);
private:
    void parseImp(Sequence &r, bool &success, bool &done);
private:
    /**
     * Scan to the next FASTA record (starting with >) and return the first
     * character of the record (which will always be >).
     */
    static int skipToNextFastaRecord(FileBuf& in) {
        int c;
        while((c = in.get()) != '>') {
            if(in.eof()) return -1;
        }
        return c;
    }
private:
    bool                    first_;
};
}
#endif // FASTAPATTERN_H

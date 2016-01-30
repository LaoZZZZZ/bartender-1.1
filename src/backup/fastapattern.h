/*
 * Copyright 2014, Lu Zhao <luzhao1986@gmail.com>
 *
 * This file is part of suffix matching project.
 */

#ifndef FASTAPATTERN_H
#define FASTAPATTERN_H
#include "patternparser.h"

namespace prefixMatching{
class fastaPattern : public patternParser
{
public:
    fastaPattern(const string&, const boost::regex&);
private:
    void parseImp(string&,std::shared_ptr<sequence> &r, bool &success, bool &done);
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

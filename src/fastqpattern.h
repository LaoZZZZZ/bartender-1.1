/*
 * Copyright 2014, Lu Zhao <luzhao1986@gmail.com>
 *
 * This file is part of suffix matching project.
 */

#ifndef FASTQPATTERN_H
#define FASTQPATTERN_H

#include "patternparser.h"
namespace barcodeSpace{
class fastqPattern : public patternParser
{
public:
    fastqPattern(const std::string&);
private:
    void parseImp(Sequence& read,
                  bool& success,
                  bool& done);
private:
    bool                    first_;
};
}
#endif // FASTQPATTERN_H

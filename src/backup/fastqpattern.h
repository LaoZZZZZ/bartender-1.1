/*
 * Copyright 2014, Lu Zhao <luzhao1986@gmail.com>
 *
 * This file is part of suffix matching project.
 */

#ifndef FASTQPATTERN_H
#define FASTQPATTERN_H

#include "patternparser.h"
namespace prefixMatching{
class fastqPattern : public patternParser
{
public:
    fastqPattern(const string&,const boost::regex&,double);
private:
    void parseImp(string& r_id,
                  std::shared_ptr<sequence>& sequence,
                  bool& success,
                  bool& done);
private:
    bool                    first_;
};
}
#endif // FASTQPATTERN_H

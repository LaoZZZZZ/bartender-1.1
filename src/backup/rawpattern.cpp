#include "rawpattern.h"

namespace prefixMatching{
rawPattern::rawPattern(const std::string& file,const boost::regex& pattern):patternParser(file,pattern,0)
{
}
void rawPattern::parseImp(std::shared_ptr<sequence> & read, bool &succ, bool &done){
    if(this->fb_->eof()){
        succ = false;
        done = true;
    }else{
        char* buf = new char[rawPattern::MAXLEN];
        memset(buf,0,rawPattern::MAXLEN);
        sequence* r = new sequence();
        int len = this->fb_->gets(buf,rawPattern::MAXLEN);
        BTDnaString& sbuf = r->fowardSeq();
        sbuf.installChars(buf,len);
        read.reset(r);
        succ = true;
        done = this->fb_->eof();
        delete buf;
    }
}
}

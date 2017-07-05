/*
 * Copyright 2015, Lu Zhao <luzhao1986@gmail.com>
 *
 * This file is part of bartender project.
 */
#ifndef PATTERNPARSER_H
#define PATTERNPARSER_H

#include "filebuf.h"
#include "sequence.h"


#include <memory>
#include <string>
#include <fstream>
#include <algorithm>

namespace barcodeSpace{
/**
  * A general pattern that parses the sequence file.
  * Its subclass is designed to handle a specific file format,like fastq,fasta..
*/
class patternParser
{
public:
    patternParser(const string& file):
         line_num_(0), _file(file), _nakeHandler(NULL){
        _nakeHandler = fopen(this->_file.c_str(),"r");
        if(!(_nakeHandler)){
            throw runtime_error(string("can not open file ") + _file);
        }
        fb_.reset(new FileBuf(this->_nakeHandler));
    }
    void parse(Sequence& r,bool& success,bool& done){
        this->parseImp(r,success,done);
    }
    void reset(){
        fclose(this->_nakeHandler);
        this->fb_.reset();
        _nakeHandler = fopen(this->_file.c_str(),"r");
        if(!(_nakeHandler)){
            throw runtime_error(string("can not open file ") + _file);
        }
        fb_.reset(new FileBuf(this->_nakeHandler));
    }
    virtual ~patternParser(){
        fclose(this->_nakeHandler);
    }
    size_t CurrentLine()const {return line_num_ + 1;}
private:
    virtual void parseImp(Sequence&,bool&,bool&) = 0;
    patternParser(const patternParser&);
    patternParser& operator=(const patternParser&);
protected:
    std::unique_ptr<FileBuf>                fb_;
    size_t                                  line_num_;
private:
    std::string                             _file;
    FILE*                                   _nakeHandler;
};
}
#endif // PATTERNPARSER_H

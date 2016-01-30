/*
 * Copyright 2014, Lu Zhao <luzhao1986@gmail.com>
 *
 * This file is part of suffix matching project.
 */
#ifndef PATTERNPARSER_H
#define PATTERNPARSER_H

#include <memory>
#include <string>
#include <fstream>
#include <stdio.h>
#include <boost/regex.hpp>
#include <algorithm>
#include "filebuf.h"
#include "sequence.h"
#include "util.h"
using std::shared_ptr;
using std::unique_ptr;
using std::string;
using std::ifstream;
using std::reverse;
namespace prefixMatching{
/**
  * a general pattern that parse the sequence file
  * Its subclass is designed to handle a specific file format,like fastq,fasta..
  * Only average quality that is larger than the specified quality threshold are considered valid
*/
class patternParser
{
public:
    patternParser(const string& file,const boost::regex& pattern,double quaThres):
                                        fb_(),qualThres_(quaThres),_file(file),_nakeHandler(NULL),_pattern(pattern){
        _nakeHandler = fopen(this->_file.c_str(),"r");
        if(!(_nakeHandler)){
            throw runtime_error(string("can not open file ") + _file);
        }
        fb_.reset(new FileBuf(this->_nakeHandler));
    }
    void parse(string& id,std::shared_ptr<sequence>& r,bool& success,bool& done){
        this->parseImp(id,r,success,done);
    }
    /**
      * only load those reads that start with the specified pattern
    */
    void parse(string& id,std::shared_ptr<sequence> &r, BTDnaString& pattern,bool &success, bool &done){
        size_t len = pattern.length();
        bool found = false;
        size_t i = 0;
        do{
            id.clear();
            //load one read
            this->parseImp(id,r,success,done);
            // check if the read match the pattern
            for(i = 0; i < len; i++){
                if(r->fowardSeq()[i] != pattern[i]){
                    break;
                }
            }
            found = (i == len); // if matched the pattern
            if(done && !found){
                r.reset();
                success = false;
                break;
            }
            if(found){
                success = true;
                break;
            }

        }while(true);
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
private:
    virtual void parseImp(string&,std::shared_ptr<sequence>&,bool&,bool&) =0;
    patternParser(const patternParser&);
    patternParser& operator=(const patternParser&);
protected:
    std::unique_ptr<FileBuf>                fb_;
    double                                  qualThres_;
protected:
    inline bool qualityCheck(const string& qual){
        double average(0);
        int len = qual.length();
        for(int i = 0; i < len; i++){
            average += static_cast<int>(qual[i]);
        }
        return average/len > this->qualThres_;
    }
    inline int isMatched(string& sequence,string& qual){
        boost::smatch result;
        if(boost::regex_search(sequence,result,_pattern)){
            this->combinePieces(sequence,qual,result);
            // if average quality is larger thant qualThres_
            if(this->qualityCheck(qual))
                return result.position();
        }
        sequence = reverseComplement(sequence);
        std::reverse(qual.begin(),qual.end());
        if(boost::regex_search(sequence,result,_pattern)){
            this->combinePieces(sequence,qual,result);
            // if average quality is larger thant qualThres_
            if(this->qualityCheck(qual))
                return result.position();
        }
        return -1;
    }
    int extractBarcode(string& seq,string& qual){
        return this->isMatched(seq,qual);
    }
    // TODO(lu): factor out this part from the source code into the
    // input specification. Since this only works for current barcode parttern.
    // Should come out with a smart way to remove the barcode's fixed part.
    inline void combinePieces(string& sequence,string& qual,boost::smatch& result){
        assert(result.size() == 10);
        //sequence.clear();
        //cout<<sequence<<endl;
        string temp;
        string tempqual;
        //first barcode
        size_t start = result.position();
        temp += sequence.substr(start+4,result.position(3)-start-4);
        tempqual += qual.substr(start+4,result.position(3)-start-4);
        //second barcode
        start = result.position(3);
        temp += sequence.substr(start +2,result.position(5) - start - 2);
        //third barcode
        tempqual += qual.substr(start +2,result.position(5) - start - 2);

        start = result.position(5);
        temp += sequence.substr(start+2,result.position(7) - start -2);
        // last barcode
        tempqual += qual.substr(start+2,result.position(7) - start -2);

        start = result.position(7);
        temp += sequence.substr(start +2, result.position(9)-start -2);
        tempqual += qual.substr(start +2, result.position(9)-start -2);
        sequence.assign(temp);

        qual.assign(tempqual);
    }
private:
    string                                  _file;
    FILE*                                   _nakeHandler;
    boost::regex                            _pattern;
};
}
#endif // PATTERNPARSER_H

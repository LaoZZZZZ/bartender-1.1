/*
 * Copyright 2014, Lu Zhao <luzhao1986@gmail.com>
 *
 * This file is part of suffix matching project.
 */
#include "fastqpattern.h"
namespace prefixMatching{

fastqPattern::fastqPattern(const string& file,const boost::regex& pattern,double qual):patternParser(file,pattern,qual),first_(true)
{
}

void fastqPattern::parseImp(string& id,
                            std::shared_ptr<sequence>& read,
                            bool& success,
                            bool& done){

    char c;

    sequence* r = new sequence();
  //  success = false;
    //done = false;
    // Pick off the first at
    if(this->first_) {
        c = this->fb_->get();
        if(c != '@') {
            c = this->fb_->getPastNewline();
            if(c < 0) {
                delete r; r = NULL;success = false; done = true;
            }
        }
        if(c != '@') {
            std::cerr << "Error: reads file does not look like a FASTQ file" << std::endl;
            throw 1;
        }
        assert_eq('@', c);
        first_ = false;
    }
    // Read to the end of the id line, sticking everything after the '@'
    // into *name

    //BTString& name = r->id();
    //BTString name;

    while(true) {

        c = this->fb_->get();
        if(c == '@')
            continue;
        if(c < 0) {
            delete r; r= NULL; success = false; done = true;
            std::cerr << "Error: reads file does not look like a FASTQ file" << std::endl;
            throw 1;
        }
        if(c == '\n' || c == '\r') {
            // Break at end of line, after consuming all \r's, \n's
            while(c == '\n' || c == '\r') {
                c = this->fb_->get();
                if(c < 0) {
                    delete r; r = NULL; success = false; done = true;
                    std::cerr << "Error: reads file does not look like a FASTQ file" << std::endl;
                    throw 1;
                }
            }
            break;
        }
        id += c;
        //name.append(c);
    }
    // fb_ now points just past the first character of a
    // sequence line, and c holds the first character
    //BTDnaString& sbuf = r->fowardSeq();
    string sequence;
    while(true){
        c = toupper(c);
        if(c < 0){
            delete r; r= NULL; success = false; done = true;
            std::cerr << "Error: reads file does not look like a FASTQ file" << std::endl;
            throw 1;
        }
        else{
            if(c == '\n' || c == '\r') {
                // Break at end of line, after consuming all \r's, \n's
                while(c == '\n' || c == '\r') {
                    c = this->fb_->get();
                    if(c < 0) {
                        delete r; r = NULL; success = false; done = true;
                        std::cerr << "Error: reads file does not look like a FASTQ file" << std::endl;
                        throw 1;
                    }
                }
                break;
            }
            //sbuf.append(asc2dna[c]);
            // convert N to A
            if(toupper(c) == 'N')
                c = 'A';
            sequence += c;
            c = this->fb_->get();
        }
    }
    assert_eq('+', c);
    // skip the option sequence at '+' line
    c = this->fb_->getPastNewline();
    if(c <0){
        delete r; r = NULL; success = false; done = true;
        std::cerr << "Error: reads file does not look like a FASTQ file" << std::endl;
        throw 1;
    }
    string qual;
    while(true){
        //qual.append(c);
        qual += c;
        c = this->fb_->get();
        if(c == '\n' || c == '\r')
            break;
    }
    assert(qual.length() == sequence.length());
    int start = this->extractBarcode(sequence,qual);
    if(start >=0){
        r->fowardSeq().installChars(sequence.c_str(),sequence.length());
        r->quality().install(qual.c_str(),qual.length());
        read.reset(r);
        success = true;
    }
    else{
        success = false;
    }
    done = this->fb_->eof();
}

}

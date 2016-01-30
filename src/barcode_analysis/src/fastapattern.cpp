/*
 * Copyright 2015, Lu Zhao <luzhao1986@gmail.com>
 *
 * This file is part of bartender project.
 */
#include "fastapattern.h"

namespace barcodeSpace{
fastaPattern::fastaPattern(const string& file):patternParser(file),first_(true)
{
}
void fastaPattern::parseImp(Sequence& read, bool &success, bool &done){

    int c;

    success = true;
    done = false;
    // Pick off the first at
    if(this->first_) {
        c = this->fb_->get();
        if(c != '>') {
            c = this->fb_->getPastNewline();
            if(c < 0) {
                success = false; done = true;
            }
            line_num_ += 1;
        }
        if(c != '>') {
            std::cerr << "Error: reads file does not look like a FASTA file" << std::endl;
            throw 1;
        }
        assert_eq('>', c);
        first_ = false;
    }
    // Read to the end of the id line, sticking everything after the '>'
    // into *name
    std::string& id = read.id();
    //BTString name;
    while(true) {
        c = this->fb_->get();
        if(c < 0) {
            read.clear(); success = false; done = true;
            std::cerr << "Error: reads file does not look like a FASTA file" << std::endl;
            throw 1;
        }
        if(c == '\n' || c == '\r') {
            // Break at end of line, after consuming all \r's, \n's
            while(c == '\n' || c == '\r') {
                c = this->fb_->get();
                if(c < 0) {
                    read.clear(); success = false; done = true;
                    std::cerr << "Error: reads file does not look like a FASTA file" << std::endl;
                    throw 1;
                }
            }
            line_num_ += 1;
            break;
        }
        if(c == '>')
            continue;
        id += c;
    }
    // fb_ now points just past the first character of a
    // sequence line, and c holds the first character
    string& seq = read.fowardSeq();
    while(true){
        c = toupper(c);
        if(c < 0){
            read.clear(); success = false; done = true;
            std::cerr << "Error: reads file does not look like a FASTA file" << std::endl;
            throw 1;
        }
        else{
            if(c == '\n' || c == '\r') {
                line_num_ += 1;
                break;
            }
            seq += c;
            c = this->fb_->get();
        }
    }
    success = true;
    done = this->fb_->eof();
}
}

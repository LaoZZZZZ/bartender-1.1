/*
 * Copyright 2014, Lu Zhao <luzhao1986@gmail.com>
 *
 * This file is part of barcode project.
 */
#include "fastqpattern.h"
namespace barcodeSpace{

fastqPattern::fastqPattern(const string& file):patternParser(file),first_(true)
{
}

void fastqPattern::parseImp(Sequence& read,
                            bool& success,
                            bool& done){

    char c;

    success = false;
    done = false;
    // Pick off the first at
    if(this->first_) {
        c = this->fb_->get();
        if(c != '@') {
            c = this->fb_->getPastNewline();
            if(c < 0) {
                success = false; done = true;
            }
            line_num_ += 1;
        }
        if(c != '@') {
            std::cerr << "Error: reads file does not look like a FASTQ file" << std::endl;
            throw 1;
        }
        assert_eq('@', c);
        first_ = false;
    }
    // Read to the end of the id line, sticking everything after the '@'
    // into id

    std::string& id = read.id();

    while(true) {

        c = this->fb_->get();
        if(c == '@')
            continue;
        if(c < 0) {
            read.clear(); success = false; done = true;
            std::cerr << "Error: reads file does not look like a FASTQ file" << std::endl;
            throw 1;
        }
        if(c == '\n' || c == '\r') {
            // Break at end of line, after consuming all \r's, \n's
            while(c == '\n' || c == '\r') {
                c = this->fb_->get();
                if(c < 0) {
                    read.clear(); success = false; done = true;
                    std::cerr << "Error: reads file does not look like a FASTQ file" << std::endl;
                    throw 1;
                }
            }
            line_num_ += 1;
            break;
        }
        id += c;
    }
    // fb_ now points just past the first character of a
    // sequence line, and c holds the first character
    //BTDnaString& sbuf = r->fowardSeq();
    std::string& sequence = read.fowardSeq();
    while(true){
        c = toupper(c);
        if(c < 0){
            read.clear(); success = false; done = true;
            std::cerr << "Error: reads file does not look like a FASTQ file" << std::endl;
            throw 1;
        }
        else{
            if(c == '\n' || c == '\r') {
                // Break at end of line, after consuming all \r's, \n's
                while(c == '\n' || c == '\r') {
                    c = this->fb_->get();
                    if(c < 0) {
                        read.clear(); success = false; done = true;
                        std::cerr << "Error: reads file does not look like a FASTQ file" << std::endl;
                        throw 1;
                    }
                }
                line_num_ += 1;
                break;
            }
            // convert N to A
            //if(toupper(c) == 'N')
            //    c = 'A';
            sequence += c;
            c = this->fb_->get();
        }
    }
    assert_eq('+', c);
    // skip the option sequence at '+' line
    c = this->fb_->getPastNewline();
    line_num_ += 1;
    if(c <0){
        read.clear(); success = false; done = true;
        std::cerr << "Error: reads file does not look like a FASTQ file" << std::endl;
        throw 1;
    }
    string& qual = read.quality();
    while(true){
        qual += c;
        c = this->fb_->get();
        if(c == '\n' || c == '\r')
            break;
    }
    line_num_ += 1;
    success = true;
    assert(qual.length() == sequence.length());
    done = fb_->eof();
}

}

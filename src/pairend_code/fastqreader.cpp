#include <stdio.h>
#include "fastqreader.h"

namespace prefixMatching {
FastqReader::FastqReader(const string &filename) : SequenceReader(filename)
{
}
void FastqReader::NextRead(Sequence& r,bool& success, bool& done) {
    int c;
    success = false;
    done = false;
    // Pick off the first at
    if(first_) {
        c = fHandler_->get();
        if(c != '@') {
            c = fHandler_->getPastNewline();
            if(c < 0) {
                r.clear();success = false; done = true;
            }
        }
        if(c != '@') {
            throw invalid_argument("Error: reads file does not look like a FASTQ file\n");
        }
        assert_eq('@', c);
        first_ = false;
    }
    string& id = r.id();
    while(true) {

        c = fHandler_->get();
        if(c == '@')
            continue;
        if(c < 0) {
            r.clear(); success = false; done = true;
            std::cerr << "Error: reads file does not look like a FASTQ file" << std::endl;
            throw 1;
        }
        if(c == '\n' || c == '\r') {
            // Break at end of line, after consuming all \r's, \n's
            while(c == '\n' || c == '\r') {
                c = fHandler_->get();
                if(c < 0) {
                    r.clear(); success = false; done = true;
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
    BTDnaString& sbuf = r.fowardSeq();
    //string sequence;
    while(true){
        c = toupper(c);
        if(c < 0){
            r.clear(); success = false; done = true;
            std::cerr << "Error: reads file does not look like a FASTQ file" << std::endl;
            throw 1;
        }
        else{
            if(c == '\n' || c == '\r') {
                // Break at end of line, after consuming all \r's, \n's
                while(c == '\n' || c == '\r') {
                    c = fHandler_->get();
                    if(c < 0) {
                        r.clear(); success = false; done = true;
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
            sbuf.append(asc2dna[c]);
            c = fHandler_->get();
        }
    }
    assert_eq('+', c);
    // skip the option sequence at '+' line
    c = fHandler_->getPastNewline();
    if(c <0){
        r.clear(); success = false; done = true;
        std::cerr << "Error: reads file does not look like a FASTQ file" << std::endl;
        throw 1;
    }
    BTString& qual = r.quality();
    while(true){
        qual.append(c);
        c = fHandler_->get();
        if(c == '\n' || c == '\r')
            break;
    }
    assert(qual.length() == r.length());
    done = fHandler_->eof();
    is_done_ = done;
    success = true;
}
}   // namespace prefixMatching

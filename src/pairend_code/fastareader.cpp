#include "fastareader.h"

namespace prefixMatching {
FastaReader::FastaReader(const string &filename) : SequenceReader(filename)
{
}
void FastaReader::NextRead(Sequence &r, bool &success, bool &done) {

    if (!r.empty())
        r.clear();
    int c;

    success = false;
    done = false;
    // Pick off the first at
    if(this->first_) {
        c = fHandler_->get();
        if(c != '>') {
            c = fHandler_->getPastNewline();
            if(c < 0) {
               r.clear();success = false; done = true;
            }
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
    string& id = r.id();
    while(true) {
        c = fHandler_->get();
        if(c < 0) {
            r.clear(); success = false; done = true;
            std::cerr << "Error: reads file does not look like a FASTA file" << std::endl;
            throw 1;
        }
        if(c == '\n' || c == '\r') {
            // Break at end of line, after consuming all \r's, \n's
            while(c == '\n' || c == '\r') {
                c = fHandler_->get();
                if(c < 0) {
                    r.clear(); success = false; done = true;
                    std::cerr << "Error: reads file does not look like a FASTA file" << std::endl;
                    throw 1;
                }
            }
            break;
        }
        if(c == '>')
            continue;
        id += c;
        //name.append(c);
    }
    // fb_ now points just past the first character of a
    // sequence line, and c holds the first character
    BTDnaString& sbuf = r.fowardSeq();
    while(true){
        c = toupper(c);
        if(c < 0){
            r.clear(); success = false; done = true;
            std::cerr << "Error: reads file does not look like a FASTA file" << std::endl;
            throw 1;
        }
        else{
            if(c == '\n' || c == '\r') {
                break;
            }
            sbuf.append(asc2dna[c]);
            c = fHandler_->get();
        }
    }
    success = true;
    done = fHandler_->eof();
    is_done_ = done;
}
}

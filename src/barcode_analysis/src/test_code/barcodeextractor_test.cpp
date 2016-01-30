#include <iostream>
#include <cassert>
#include <memory>
#include <boost/regex.hpp>
#include "../barcodeextractor.h"
#include "../util.h"
using std::string;
using std::vector;
using std::endl;
using std::cout;
using std::unique_ptr;
using barcodeSpace::BarcodeExtractor;
using barcodeSpace::Sequence; 
using boost::regex;
using barcodeSpace::reverseComplement;

int main(void) {

    // Real data example.
    boost::regex pattern("(.ACC|T.CC|TA.C|TAC.)([ATCGN]{4,7})(AA)([ATCGN]{4,7})(AA)([ATCGN]{4,7})(TT)([ATCGN]{4,7})(.TAA|A.AA|AT.A|ATA.)",boost::regex::ECMAScript);
    //string seq("TGCTGAACGCCCTCTTACGGATATTCGCGATGAGTATCATTACCCCAAAAAGAAAGGTATTAAGGATGAGTGTTCAAGATTGCTGGAGGCCTCCACTA");
    string seq("AACGACGATGTTTAATATGGACTAAAGGAGGCTTTTGTCGACGGATCCGATATCGGTACCACATAAACAATAAAGTCTATTATGNAATNACTTCGNAT");
    string qual("@@@@@FFFFDHFHFDHIJIIGGHIJJJIIJJIGDHDGCHIIJGI@HIIBA<EEFFAAB@BBBDDDDDDCCCD:>A:ACCA:>CCCCCDD#+22#++8?");
    Sequence read("read1", seq.c_str(), qual);
    string preceeding = "TACC";
    string succeeding = "ATAA";
    size_t parts = 9;
    unique_ptr<BarcodeExtractor> extractor(new BarcodeExtractor(pattern, preceeding, succeeding, parts));
    Sequence read_cp(read.id(), seq, qual);    
    extractor->ExtractBarcode(read_cp);
    cout << read_cp.fowardSeq() << '\t' <<  read_cp.quality() <<endl; 
    bool success = false;
    Sequence barcode = extractor->ExtractBarcode(read, success); 
    cout << success << '\t' << barcode.fowardSeq() << '\t' << barcode.quality() <<endl;

    // ONLY have one random part with fixed length. 
    pattern.assign("(TACC)([ATCGN]{4})(ATAA)", boost::regex::ECMAScript);
    seq.assign("AACTTACCACGTATAACCT");
    qual.assign("@@@@@@@@####@@@@@@@");
    preceeding.assign("TACC");
    succeeding = "ATAA";
    parts = 3;
    Sequence read_1("read2", seq.c_str(), qual);
    extractor.reset(new BarcodeExtractor(pattern, preceeding, succeeding,parts));
    extractor->ExtractBarcode(read_1);
    cout << read_1.fowardSeq() << '\t' << read_1.quality() << endl;

    // Two random parts with fixed length;
    pattern.assign("(TACC)([ATCGN]{4})(TT)([ATCGN]{4})(ATAA)", boost::regex::ECMAScript);
    seq.assign("AACTTACCACGTTTTCGAATAACCT");
    qual.assign("@@@@@@@@####@@####@@@@@@@");
    preceeding.assign("TACC");
    succeeding = "ATAA";
    parts = 5;
    Sequence read_3("read2", seq.c_str(), qual);
    extractor.reset(new BarcodeExtractor(pattern, preceeding, succeeding,parts));
    extractor->ExtractBarcode(read_3);
    cout << read_3.fowardSeq() << '\t' << read_3.quality() << endl;
    
    // Two random parts with length range;
    pattern.assign("(TACC)([ATCGN]{4})(TT)([ATCGN]{4,7})(ATAA)", boost::regex::ECMAScript);
    seq.assign("AACTTACCACGTTTTCGATTATAACCT");
    qual.assign("@@@@@@@@####@@######@@@@@@@");
    preceeding.assign("TACC");
    succeeding = "ATAA";
    parts = 5;
    Sequence read_4("read3", seq.c_str(), qual);
    extractor.reset(new BarcodeExtractor(pattern, preceeding, succeeding,parts));
    extractor->ExtractBarcode(read_4);
    cout << read_4.fowardSeq() << '\t' << read_4.quality() << endl;
 
    /* 
    std::string fastafile = "/home/laozzzzz/barcode_project/reads/e_coli_10000snp.fa";
    std::unique_ptr<patternParser> parser(new fastaPattern(fastafile));
    bool done = false, success = false;
    Sequence read;
    parser->parse(read, success, done);
    assert(success);
    assert(!done);
    cout << read.fowardSeq() << '\t' << read.quality() << endl;       
    int count = 1;
    while (!done) {
        read.clear();
	parser->parse(read, success, done);
        assert(success);
        ++count;
    }   
    cout << "Total number reads: " << count <<endl;
    // This file contains 10000 reads.
    assert(10000 == count);

    ///////////////////////////Test fastqPattern//////////////////////////////////////
    
    std::string fastqfile = "/home/laozzzzz/barcode_project/reads/1000_reorder.fq";
    parser.reset(new fastqPattern(fastqfile));
    done = false;
    success = false;
    count = 0;
    while (!done) {
	read.clear();
	parser->parse(read, success, done);
        ++count; 
        assert(success || read.empty());
    } 
    // The file contains 1000 reads.
    assert(count == 1000);
    */ 
    return 0;
}



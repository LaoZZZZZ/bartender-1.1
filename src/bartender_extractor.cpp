
//
//  bartender_extractor.cpp
//  barcode_project
//
//  Created by luzhao on 1/1/16.
//  Copyright © 2016 luzhao. All rights reserved.
//

#include <stdio.h>

#include "barcodeextractor.h"
#include "filebuf.h"
#include "singlereadsprocessor.hpp"
#include "timer.h"

#include <cassert>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include <boost/regex.hpp>

using namespace std;
using namespace barcodeSpace;
using boost::regex;

file_format FindInputFormat(const string& readsfile) {
    ifstream raw_handler(readsfile);
    FileBuf handler(&raw_handler);
    file_format format = RAWSEQ;
    string line;
    while (!handler.eof()) {
        handler.getline(line);
        if (!line.empty() && line[0] == '>') {
            format = FASTA;
            break;
        } else if (!line.empty() && line[0] == '+') {
            format = FASTQ;
            break;
        }
        line.clear();
    }
    if (format == RAWSEQ) {
        throw runtime_error("The input reads file does not lookes like fasta or fastq file!\n");
    }
    return format;
}

void drive(const string& reads_file,
           const string& output_prefix,
           double quality_threshold,
           const boost::regex& pattern,
           const string& preceeding,
           const string& suceeding,
	   size_t num_sub_regex) {
    
    Timer* time = new realTimer(cout);
    file_format format = FindInputFormat(reads_file);
    std::shared_ptr<BarcodeExtractor> barcode_extractor(
        new BarcodeExtractor(pattern,preceeding, suceeding, num_sub_regex));
    SingleReadsProcessor processor(reads_file,
                                   barcode_extractor,
                                   format,
                                   output_prefix,
                                   quality_threshold);
    processor.extract();
    
    cout << "Totally there are " << processor.TotalReads() << " reads in " <<  reads_file << " file!" << endl;
    cout << "Totally there are " << processor.TotalBarcodes() << " valid barcodes from " << reads_file << " file" << endl;
    cout << "Totally there are " << processor.TotalQualifiedBarcodes() << " valid barcodes whose quality pass the quality condition " << endl;
    
    cout << "The estimated sequence error from the prefix and suffix parts is " << processor.errorRate() << endl;
    delete time;
}
/*
int main(int argc,char* argv[])
{
    assert(argc >= 3);
    string input_reads_file(argv[1]);
    string output_prefix(argv[2]);
    
    double qual_threshold = 0;
    if (argc >= 4) {
        qual_threshold = atof(argv[3]);
    }
    
    boost::regex pattern;
    if (argc >= 5) {
        pattern.assign(argv[4]);
    }
    string preceeding;
    string suceeding;
    if (argc >= 6) {
        preceeding.assign(argv[5]);
    }
    if (argc >= 7) {
        suceeding.assign(argv[6]);
    }
    size_t num_sub_regex = 3;
    if (argc >= 8) {
        num_sub_regex = atoi(argv[7]);
    }
    drive(input_reads_file,
          output_prefix,
          qual_threshold,
          pattern,
          preceeding,
          suceeding,
	  num_sub_regex);
    return 0;
}*/

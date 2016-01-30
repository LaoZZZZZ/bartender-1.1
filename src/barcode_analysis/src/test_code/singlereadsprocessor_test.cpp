#include "../singlereadsprocessor.hpp"
#include "../barcodeextractor.h"


#include <boost/regex.hpp>

#include <string>
#include <iostream>
#include <memory>
using namespace barcodeSpace;
using namespace std;
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
    handler.close();
    return format;
}
int main() {
    string reads_file = "./reads/test_2M.fq";
    boost::regex pattern("(.ACC|T.CC|TA.C|TAC.)(A|T|C|G|N){4,7}(AA)(A|T|C|G|N){4,7}(AA)(A|T|C|G|N){4,7}(TT)(A|T|C|G|N){4,7}(.TAA|A.AA|AT.A|ATA.)", boost::regex::ECMAScript);
    string preceeding = "TACC";
    string suceeding = "ATAA";
    double quality_threshold = 0.0;
    string output_prefix = "singlereadsprocessor_result";
    size_t parts = 9U;
    file_format format = FindInputFormat(reads_file);
    std::shared_ptr<BarcodeExtractor> barcode_extractor(
        new BarcodeExtractor(pattern,preceeding, suceeding, parts));
    SingleReadsProcessor processor(reads_file,
                                   barcode_extractor,
                                   format,
                                   output_prefix,
                                   quality_threshold);
    processor.extract();
    
    cout << "Totally there are " << processor.TotalReads() << " reads in " <<  reads_file << " file!" << endl;
    cout << "Totally there are " << processor.TotalBarcodes() << " valid barcodes from " << reads_file << " file" << endl;
    cout << "Totally there are " << processor.TotalQualifiedBarcodes() << " valid barcodes whose quality pass the quality condition " << endl;
    
    cout << "The estimated sequence error from the fixed part is " << processor.errorRate() << endl;	
	return 0;
}

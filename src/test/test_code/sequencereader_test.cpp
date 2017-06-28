#include "../backup/fastareader.h"
#include "../backup/fastqreader.h"
#include "../sequence.h"
#include <iostream>
using std::string;
using std::vector;
using std::endl;
using std::cout;
using barcodeSpace::Sequence;
using barcodeSpace::SequenceReader;
using barcodeSpace::FastaReader;
using barcodeSpace::FastqReader;

int main(void) {
    // Test the fastareader
    string filename = "/home/laozzzzz/barcode_project/reads/e_coli_10000snp.fa";
    bool done = false;
    bool success = false;
    Sequence read;
    std::unique_ptr<SequenceReader> reader(FastaReader::CreateSequenceReader(filename));
    while(!success) {
        reader->NextRead(read, success, done);
        cout<< read.fowardSeq() <<endl;
    }
    read.clear();
    filename = "/home/laozzzzz/barcode_project/reads/1000.fq";
    done = false;
    success = false;
    reader.reset(FastqReader::CreateSequenceReader(filename));
    while(!success) {
	reader->NextRead(read, success, done);
        cout<< read.fowardSeq() <<endl;
    }
    return 0;
}

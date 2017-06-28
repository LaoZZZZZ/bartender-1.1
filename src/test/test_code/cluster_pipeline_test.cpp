#include <iostream>
#include <cassert>
#include <memory>
#include <unordered_map>
#include <boost/regex.hpp>
#include "../singleendbarcodeprocessor.h"
#include "../formats.h"
#include "../clusterpipeline.h"
using std::string;
using std::vector;
using std::endl;
using std::cout;
using std::unique_ptr;
using barcodeSpace::SingleEndBarcodeProcessor;
using barcodeSpace::BarcodeCounter;
using boost::regex;
using barcodeSpace::file_format;
using barcodeSpace::clusterPipline;
using barcodeSpace::kmers_freq;
using barcodeSpace::cluster;
void printBarcodeInformation(SingleEndBarcodeProcessor& p) {
    int totalBarcode = 0;
    for(size_t len = 16; len < 28; ++len) {
	BarcodeCounter* table = p.BarcodeFrequentTable(len);
	if (table) {
		cout << "Barcode length: " << len << " total has " << table->size() <<endl;
	        const barcodeFreqTable& ft = table->BarcodeTable();
		for (const auto& p : ft) {
			totalBarcode += p.second;
		}	
	}
    }
    cout << "Total valid barcodes: " << totalBarcode << endl;
}

void cluster_drive(SingleEndBarcodeProcessor& p) {
    int totalBarcode = 0;
    const std::unordered_map<size_t, BarcodeCounter*>& tables = p.AllBarcodeTables();
    //std::list<kmers_freq> result;
    //int total_clusters = 0;
    //int total_barcodes = 0;
    for (const auto& table : tables) {
        std::unique_ptr<clusterPipline> cluster_eng(new clusterPipline(0, 10, 2 * table.first, 10));	
	cluster_eng->clusterDrive(table.second->BarcodeTable());
	const std::list<std::shared_ptr<cluster>>& iden_clusters = cluster_eng->clusters();
	cout << "barcode length: " << iden_clusters.size() << endl;
    }
}
int main(void) {

    const boost::regex pattern("(.ACC|T.CC|TA.C|TAC.)(A|T|C|G|N){4,7}(AA)(A|T|C|G|N){4,7}(AA)(A|T|C|G|N){4,7}(TT)(A|T|C|G|N){4,7}(.TAA|A.AA|AT.A|ATA.)",boost::regex::ECMAScript);
    const string filename = "../reads/test.fq";
    const file_format format = file_format::FASTQ; 
    const double threshold = 30;
    SingleEndBarcodeProcessor processor(filename, pattern, format, threshold);
    processor.Extract();
    
    cout << processor.TotalReads() << '\t' << processor.TotalBarcodes() << '\t' << processor.TotalQualifiedBarcodes() << endl;
    cluster_drive(processor); 

    return 0;
}



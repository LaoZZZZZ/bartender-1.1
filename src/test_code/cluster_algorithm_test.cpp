#include <iostream>
#include <cassert>
#include <memory>
#include <unordered_map>
#include <boost/regex.hpp>
#include "../singleendbarcodeprocessor.h"
#include "../formats.h"
#include "../clusteralgorithm.h"
using std::string;
using std::vector;
using std::endl;
using std::cout;
using std::unique_ptr;
using barcodeSpace::SingleEndBarcodeProcessor;
using barcodeSpace::BarcodeCounter;
using boost::regex;
using barcodeSpace::file_format;
using barcodeSpace::clusterAlgorithm;
using barcodeSpace::kmers_freq;
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

void BarcodeInformation(SingleEndBarcodeProcessor& p) {
    int totalBarcode = 0;
    const std::unordered_map<size_t, BarcodeCounter*>& tables = p.AllBarcodeTables();
    std::list<kmers_freq> result;
    int total_clusters = 0;
    int total_barcodes = 0;
    for (const auto& table : tables) {
	result.clear();
	for ( const auto& pair : table.second->BarcodeTable()) {
		result.push_back(kmers_freq(pair.first, pair.second));
	}	
        std::unique_ptr<clusterAlgorithm> cluster_eng(new clusterAlgorithm(4, table.first, result));	
	cluster_eng->clusterImp();
	cout << result.size() << '\t' << cluster_eng->clusters().size() <<endl;	
	total_clusters += cluster_eng->clusters().size();
	for (const auto& cluster : cluster_eng->clusters()) {
		total_barcodes += cluster->size();
	}
    }
    cout << "Total clusters: " << total_clusters << "\t" << "Total barcode: " << total_barcodes << endl;
}
int main(void) {

    const boost::regex pattern("(.ACC|T.CC|TA.C|TAC.)(A|T|C|G|N){4,7}(AA)(A|T|C|G|N){4,7}(AA)(A|T|C|G|N){4,7}(TT)(A|T|C|G|N){4,7}(.TAA|A.AA|AT.A|ATA.)",boost::regex::ECMAScript);
    const string filename = "../reads/test.fq";
    const file_format format = file_format::FASTQ; 
    const double threshold = 30;
    SingleEndBarcodeProcessor processor(filename, pattern, format, threshold);
    processor.Extract();
    
    cout << processor.TotalReads() << '\t' << processor.TotalBarcodes() << '\t' << processor.TotalQualifiedBarcodes() << endl;
    BarcodeInformation(processor); 
    return 0;
}



#include "../minicsv.h"
#include "../csvreader.h"
#include <iostream>
#include <cassert>
#include <memory>
#include <string>
#include <vector>

using std::string;
using std::endl;
using std::cout;
using std::unique_ptr;
using std::vector;
using barcodeSpace::CSVReader;

int main() {
    
    string filename = "../../data/entropy_table.csv";

    CSVReader reader(filename,true);
    std::vector<std::string> header = reader.Header();
    for(const auto& h : header) {
	cout << h << ",";
    }
    header.clear();
    
    while (reader.nextRow(&header)) {
	header.clear();
    }
    cout << reader.TotalRecord() << endl; 
    return 0;
}

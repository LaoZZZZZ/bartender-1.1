#include "testSimulation.h"
#include "typdefine.h"
#include "cluster.h"
#include "typdefine.h"
#include "clusterpipeline.h"
#include "kmers_bitwisetransform.h"
#include "csvreader.h"

#include <string>
#include <iostream>
#include <fstream>
using std::endl;
namespace barcodeSpace{
void loadData(const std::string& file,
              barcodeFreqTable& table){
    std::ifstream handler(file.c_str());
    if(handler.is_open()){
        table.clear();
        kmer k;
        freq f;
        while(handler >> k){
           handler >> f;
           table[k] += f;

        }
        
    }else
        throw runtime_error("Invalid file to read!\n");
    handler.close();
}
void loadDataFromCSV(const std::string& file,
                     barcodeFreqTable& table,
                     std::vector<std::unordered_map<kmer, std::list<std::string>>>& b2sequence) {
    // Initialize a loader. Assume the file does not have header.
    CSVReader<kmer>   _cluster_loader(file, false);
    vector<kmer> row;
    table.clear();
    b2sequence.assign(21, {});
    int line = 1;
    while(_cluster_loader.nextRow(&row)) {
        table[row[0]] += row[1];
        row.clear();
        b2sequence[20][row[0]].push_back(to_string(line));
        ++line;
    }
}
void outputCluster(const std::string& outfile,const std::vector<std::shared_ptr<cluster>>& c){
    fstream outhandler(outfile.c_str(), fstream::out);
    for(auto iter = c.begin(); iter != c.end(); iter++){
        outhandler<<(*iter)->center()->center()<<','<<(*iter)->size()<<endl;
    }
    outhandler.close();
}

}

#ifndef TESTSIMULATION_H
#define TESTSIMULATION_H
#include <string>
#include <memory>
#include <list>
#include <vector>
#include <unordered_map>

#include "typdefine.h"
#include "cluster.h"
using std::fstream;
namespace barcodeSpace{

    void loadData(const std::string& file,barcodeFreqTable& table);
    void loadDataFromCSV(const std::string& file,
                     barcodeFreqTable& table,
                     std::vector<std::unordered_map<kmer, std::list<std::string>>>& b2sequence);
void outputCluster(const std::string&, const std::vector<std::shared_ptr<cluster>>&);

}
#endif // TESTSIMULATION_H

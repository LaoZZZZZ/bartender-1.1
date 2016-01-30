#ifndef TESTSIMULATION_H
#define TESTSIMULATION_H
#include "suffix_commons.h"
#include "clusterpipeline.h"
#include "kmers_bitwisetransform.h"
#include <fstream>
using std::fstream;
namespace prefixMatching{

void loadData(std::string file,kmers_sparseHash<kmer,freq>& table);

void outputCluster(std::string,const std::list<std::shared_ptr<cluster>>&);
}
#endif // TESTSIMULATION_H

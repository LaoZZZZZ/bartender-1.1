#ifndef CLUSTEROUTPUT_H
#define CLUSTEROUTPUT_H

#include "csvoutput.h"
#include "barcodecluster.hpp"
#include "barcodepool.hpp"


#include <list>
#include <memory>
#include <vector>
#include <string>

namespace barcodeSpace {
// This class is designed to output a list of clusters
// to a csv file format.
// There are at least two files.
// 1. The cluster table that contains the basic information of the cluster.
// 2. The frequency table that contains the base pair frequency for each cluster.
// 3. The barcode table that contains the information for each barcode.
class ClusterOutput
{
    
public:
    typedef BarcodeCluster Cluster;
    typedef std::unordered_map<kmer, std::vector<std::string>> barcode2Sequence;
    
    ClusterOutput(const std::string& filename);
    
    void WriteToFile(const std::list<std::shared_ptr<Cluster>>& clusters,
                     size_t max_barcode_length);
    void WriteToFile(const std::list<std::shared_ptr<Cluster>>& clusters,
                     const std::shared_ptr<BarcodePool>& barcocde_pool,
                     size_t max_barcode_length);
private:
    // Output filename prefix.
    std::string                                       _filename_prefix;
};

}
#endif // CLUSTEROUTPUT_H

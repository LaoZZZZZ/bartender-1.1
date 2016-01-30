#ifndef CLUSTEROUTPUT_H
#define CLUSTEROUTPUT_H

#include "csvoutput.h"
#include "cluster.h"
#include "rawdatainfo.hpp"


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
    typedef std::unordered_map<kmer, std::list<std::string>> barcode2Sequence;
    ClusterOutput(const std::string& filename);

    void WriteToFile(const std::list<std::shared_ptr<cluster>>& clusters,
                     const std::vector<barcode2Sequence>& raw_data_info);
    void WriteToFileWithoutBarcodes(const std::list<std::shared_ptr<cluster>>& clusters,
                                    size_t max_barcode_length);

private:
    // Output filename prefix.
    std::string                                       _filename_prefix;
};

}
#endif // CLUSTEROUTPUT_H

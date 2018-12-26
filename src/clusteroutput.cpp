#include "barcodetabledumper.hpp"

#include "clusteroutput.h"
#include "barcodecluster.hpp"
#include "clustertabledumper.hpp"
#include "qualitytabledumper.hpp"
#include "barcodetabledumper.hpp"

#include <array>
#include <cassert>
#include <list>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

using std::list;
using std::string;
using std::shared_ptr;
using std::array;
using std::vector;

namespace barcodeSpace {

ClusterOutput::ClusterOutput(const string& prefix):_filename_prefix(prefix)
{
}
    void ClusterOutput::WriteToFile(const std::list<std::shared_ptr<Cluster>>& clusters,
                                    const std::shared_ptr<BarcodePool>& barcode_pool,
                                    size_t max_barcode_length) {
    if (clusters.empty())
        return;
    size_t num_time_points = clusters.front()->columns().size();
        std::cout << "Dumping result in parallel ******" << std::endl;
    ClusterTableDumper cluster_dumper(_filename_prefix + "_cluster.csv", num_time_points, clusters);
    
    QualityTableDumper quality_dumper(_filename_prefix + "_quality.csv", max_barcode_length, clusters);
    BarcodeTableDumper barcode_dumper(_filename_prefix + "_barcode.csv", clusters, barcode_pool);
    cluster_dumper.start();
    quality_dumper.start();
    barcode_dumper.start();
    cluster_dumper.join();
    quality_dumper.join();
    barcode_dumper.join();
}
    
void ClusterOutput::WriteToFile(const std::list<std::shared_ptr<Cluster>>& clusters,size_t max_barcode_length) {
    if (clusters.empty())
        return;
    size_t num_time_points = clusters.front()->columns().size();
    ClusterTableDumper cluster_dumper(_filename_prefix + "_cluster.csv", num_time_points, clusters);
    
    QualityTableDumper quality_dumper(_filename_prefix + "_quality.csv", max_barcode_length, clusters);
    cluster_dumper.start();
    quality_dumper.start();
    cluster_dumper.join();
    quality_dumper.join();
}

}   //namespace barcodeSpace

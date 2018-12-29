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
        if (clusters.empty()) {
            std::cout << "There is no cluster in the result list." << std::endl;
            return;
        }
        size_t num_time_points = clusters.front()->columns().size();
        std::vector<std::unique_ptr<ThreadWrapper>> dumpers;
            dumpers.push_back(std::unique_ptr<ThreadWrapper>(new ClusterTableDumper(_filename_prefix + "_cluster.csv", num_time_points, clusters)));
        dumpers.push_back(std::unique_ptr<ThreadWrapper>(new QualityTableDumper(_filename_prefix + "_quality.csv", max_barcode_length, clusters)));
        dumpers.push_back(std::unique_ptr<ThreadWrapper>(new BarcodeTableDumper(_filename_prefix + "_barcode.csv", clusters, barcode_pool)));
        for (auto& dumper : dumpers) {
            dumper->start();
        }
        for (auto& dumper : dumpers) {
            dumper->join();
        }
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

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
    ClusterTableDumper cluster_dumper(_filename_prefix + "_cluster.csv", num_time_points);
    
    QualityTableDumper quality_dumper(_filename_prefix + "_quality.csv", max_barcode_length);
    BarcodeTableDumper barcode_dumper(_filename_prefix + "_barcode.csv");
    
    int id = 1;
    for (auto& c : clusters) {
        c->SetClusterID(id);
        ++id;
        cluster_dumper.WriteCluster(c);
        quality_dumper.WritePWM(c->ClusterID(), c->bpFrequency());
        barcode_dumper.writeBarcodeLine(c->ClusterID(), c->barcodes(), barcode_pool);
    }
}
    
void ClusterOutput::WriteToFile(const std::list<std::shared_ptr<Cluster>>& clusters,size_t max_barcode_length) {
    if (clusters.empty())
        return;
    size_t num_time_points = clusters.front()->columns().size();
    ClusterTableDumper cluster_dumper(_filename_prefix + "_cluster.csv", num_time_points);
    
    QualityTableDumper quality_dumper(_filename_prefix + "_quality.csv", max_barcode_length);
    
    int id = 1;
    for (auto& c : clusters) {
        c->SetClusterID(id);
        ++id;
        cluster_dumper.WriteCluster(c);
        quality_dumper.WritePWM(c->ClusterID(), c->bpFrequency());
    }
}

}   //namespace barcodeSpace

#include "barcodetabledumper.hpp"

#include "clusteroutput.h"
#include "cluster.h"
#include "clustertabledumper.hpp"
#include "kmerdecoder.hpp"
#include "qualitytabledumper.hpp"

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
void ClusterOutput::WriteToFileWithoutBarcodes(const std::list<std::shared_ptr<cluster>>& clusters,
                                               size_t max_barcode_length) {
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

void ClusterOutput::WriteToFile(const std::list<std::shared_ptr<cluster>>& clusters,
                                const std::vector<barcode2Sequence>& raw_data_info) {
    if (clusters.empty())
        return;
    size_t max_length(0);
    vector<KmerDecoder> decoders;
    // Get the maximum length of barcode
    for (size_t i = 0; i < raw_data_info.size(); ++i) {
        if (!raw_data_info[i].empty()) {
            max_length = i;
        }
        decoders.push_back(KmerDecoder(i));
    }
    size_t num_time_points = clusters.front()->columns().size();
    ClusterTableDumper cluster_dumper(_filename_prefix + "_cluster.csv", num_time_points);
    
    QualityTableDumper quality_dumper(_filename_prefix + "_quality.csv", max_length);
    BarcodeTableDumper barcode_link_dumper(_filename_prefix + "_barcode.csv");
   
    int id = 1; 
    for (auto& c : clusters) {
        c->SetClusterID(id);
        ++id;
        cluster_dumper.WriteCluster(c);
        quality_dumper.WritePWM(c->ClusterID(), c->bpFrequency());
        for (const auto& edited_barcode : c->barcodes()) {
            const string barcode = decoders[c->center()->klen()].DNASequence(edited_barcode._key);
            barcode_link_dumper.writeBarcodeLine(c->ClusterID(),
                                                 barcode,
                                                 raw_data_info[c->center()->klen()].at(edited_barcode._key));

        }
    }
}
}   //namespace barcodeSpace

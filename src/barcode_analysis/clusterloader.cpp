#include "clusterloader.h"

#include "cluster.h"
#include "dictatorcenter.h"
#include "dictatorcluster.h"
#include "sequenceencoder.hpp"
#include "util.h"

#include <algorithm>
#include <array>
#include <list>
#include <memory>
#include <numeric>
#include <stdexcept>
#include <string>
#include <vector>
#include <unordered_map>

using std::array;
using std::list;
using std::shared_ptr;
using std::string;
using std::vector;
using std::runtime_error;
using std::unordered_map;
using std::stoi;


namespace barcodeSpace {
ClusterLoader::ClusterLoader(const string& cluster_file,
                             const string& frequency_file):
    _cluster_loader(cluster_file, true), _frequency_loader(frequency_file, true),
    _cluster_filename(cluster_file),_frequency_filename(frequency_file), _longest_barcode(0)
{
}
// Needs refactoring.
// TODO(lu):
// 1. Factor out the parsing logic of cluster, quality and barcode file from this class.
// 2. 
bool ClusterLoader::LoadClusters(std::unordered_map<int, list<std::shared_ptr<cluster>>>* clusters) {
    vector<std::string>            cluster_record;  // cluster info
    vector<std::string>             frequency_each;
    SequenceEncoder kmerencoder;
    while (_cluster_loader.nextRow(&cluster_record)) {
        try {
            if (cluster_record.size() < 4) {
                throw std::invalid_argument("The cluster file format seems not right!\n");
            }
            std::string center = cluster_record[1];
            _longest_barcode = std::max(_longest_barcode, center.length());
            vector<freq> time_points;
            for (auto iter = cluster_record.begin() + 3; iter < cluster_record.end(); ++iter) {
                time_points.push_back(stoi(*iter));
            }
            //assert(std::accumulate(time_points.begin(), time_points.end(),0u) == cluster_record[1]);
            vector<array<int,4>>    frequency_record(center.length(), array<int,4>()); // Frequency table for each cluster.
            for(size_t i = 0; i < 4; ++i) {
               if (_frequency_loader.nextRow(&frequency_each)) {
                   if (frequency_each.size() < center.length() + 2) {
                       throw std::invalid_argument("The quality file format seems not right!\n");
                   }
                   for (size_t j = 0; j < center.length(); ++j) {
                       frequency_record[j][i] = stoi(frequency_each[j + 2]);
                   }
               } else {
                    throw std::runtime_error(string("Invalid frequency table, which does not match with cluster file ") + _cluster_filename);
               }

               frequency_each.clear();
            }

            DictatorCenter* temp_center = new DictatorCenter(kmerencoder.encode(center), center.length());
            std::shared_ptr<DictatorCenter> p_center(temp_center);
            cluster* temp_cluster = new dictatorCluster(p_center, time_points, frequency_record, stoi(cluster_record[0]));
            std::shared_ptr<cluster> temp(temp_cluster);
            //temp->SetClusterID(stoi(cluster_record[0]));
            //temp->SetFrequencyTable(frequency_record);
            //temp->SetTimePointFrequency(time_points);
            //temp->SetCenterScore(similarityScore(cluster_record[2], frequency_record));
            cluster_record.clear();
            if (clusters->find(center.length()) == clusters->end()) {
                clusters->insert({center.length(),{temp}});
            } else {
                clusters->at(center.length()).push_back(temp);
            }
        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    }
    return true;
}

}   // namespace barcodeSpace

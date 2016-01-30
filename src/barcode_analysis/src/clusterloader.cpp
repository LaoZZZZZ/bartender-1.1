#include "clusterloader.h"

#include "cluster.h"
#include "dictatorcenter.h"
#include "dictatorcluster.h"
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

namespace barcodeSpace {
ClusterLoader::ClusterLoader(const string& cluster_file,
                             const string& frequency_file):
    _cluster_loader(cluster_file, true), _frequency_loader(frequency_file, true),
    _cluster_filename(cluster_file),_frequency_filename(frequency_file)
{
}
bool ClusterLoader::LoadClusters(std::unordered_map<int, list<std::shared_ptr<cluster>>>* clusters) {
    vector<kmer>            cluster_record;  // cluster info
    vector<int>             frequency_each;
    while (_cluster_loader.nextRow(&cluster_record)) {
        size_t klen = cluster_record[3];
        vector<freq> time_points(cluster_record.begin() + 4, cluster_record.end());
        assert(std::accumulate(time_points.begin(), time_points.end(),0u) == cluster_record[1]);
        vector<array<int,4>>    frequency_record(klen, array<int,4>()); // frequency table for each cluster.
        for(size_t i = 0; i < 4; ++i) {
           if (_frequency_loader.nextRow(&frequency_each)) {
               for (size_t j = 0; j < klen; ++j) {
                   frequency_record[j][i] = frequency_each[j + 1];
               }
           } else {
                throw std::runtime_error(string("Invalid frequency table, which does not match with cluster file ")
                                         + _cluster_filename);
           }

           frequency_each.clear();
        }

        DictatorCenter* temp_center = new DictatorCenter(cluster_record[2], klen);
        std::shared_ptr<DictatorCenter> p_center(temp_center);
        cluster* temp_cluster = new dictatorCluster(p_center, time_points, frequency_record, cluster_record[0]);
        std::shared_ptr<cluster> temp(temp_cluster);
        temp->SetClusterID(cluster_record[0]);
        //temp->SetFrequencyTable(frequency_record);
        //temp->SetTimePointFrequency(time_points);
        //temp->SetCenterScore(similarityScore(cluster_record[2], frequency_record));
        cluster_record.clear();
        if (clusters->find(klen) == clusters->end()) {
            clusters->insert({klen,{temp}});
        } else {
            clusters->at(klen).push_back(temp);
        }
    }
    return true;
}

}   // namespace barcodeSpace

#ifndef SINGLETIMEPOINTDATA_H
#define SINGLETIMEPOINTDATA_H

#include "cluster.h"
#include "centerstracker.h"
#include "centerclustermapper.h"

#include <algorithm>
#include <array>
#include <list>
#include <memory>
#include <vector>

namespace barcodeSpace {
/**
  * A class holds the result of single data point:
  * 1. The cluster result.
  * 2. The sequence error.
  * 3. The centers and cluster linker.
  * 4. The entropy matrix.
  * 5. The number of center histogram.
*/
class SingleTimePointData
{
public:
    SingleTimePointData(int time_point);
    void addCluster(const std::shared_ptr<cluster>& c) {
        std::vector<double> entropy;
        for (const auto& bps : c->bpFrequency()) {
            entropy.push_back(Entropy(bps));
        }
        _entropies.insert({c->ClusterID(), std::move(entropy)});
    }
    // Gets the cluster by cluster id
    const std::shared_ptr<cluster>& getCluster(int id) {
        return _clusters[id];
    }
    // Gets the entropy matrix by cluster id
    const std::vector<double>& getEntropy(int id) { return _entropies[id];}
    // Number of clusters
    size_t size() const {return _clusters.size();}

private:
    inline double Entropy(const std::array<int, 4>& base_freq) {
        int majority = *std::max_element(base_freq.begin(), base_freq.end());

        int total = std::accumulate(base_freq.begin(), base_freq.end(), 0);
        if(total == majority) {
            return 0.0;
        } else {
            double ratio = majority / total;
            return -1 * (ratio * log2(ratio) + (1-ratio)*log2(1-ratio));
        }
    }
    
    std::unordered_map<int, std::shared_ptr<cluster>>   _clusters;
    std::unordered_map<int, std::vector<double>> _entropies;
    std::shared_ptr<CenterClusterMapper>         _center_cluster_link;
    double  _error_rate;
    int _time_point;
};
}   // namespace barcodeSpace
#endif // SINGLETIMEPOINTDATA_H

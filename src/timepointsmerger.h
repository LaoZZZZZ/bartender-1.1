#ifndef TIMEPOINTSMERGER_H
#define TIMEPOINTSMERGER_H

#include "barcodemutationgenerator.h"
#include "barcodecluster.hpp"
#include "centerclustermapper.h"
#include "clustermergertester.h"
#include "typdefine.h"

#include <list>
#include <memory>
#include <set>
#include <vector>

namespace barcodeSpace {
// Merge the clusters of two different time points.
// Generate a new clusters results.
class TimePointsMerger
{
public:
    typedef BarcodeCluster Cluster;
    // Assume the clusters are sorted in terms of the cluster size.
    // Assume t1 > t2 cause we merge result from later time point.
    TimePointsMerger(const std::shared_ptr<CenterClusterMapper>& t1_link,
                     const std::shared_ptr<CenterClusterMapper>& t2_link,
                     size_t k_len,
                     size_t num_time_points,
                     size_t filter_csize_threshold = 3);
    // Merge the
    void merge();
    const std::list<std::shared_ptr<Cluster>>& mergedClusters() const{
        return _result;
    }
private:
    std::shared_ptr<CenterClusterMapper> _t1_center_cluster_linker;
    std::shared_ptr<CenterClusterMapper> _t2_center_cluster_linker;
    size_t  _k_len;
    std::list<std::shared_ptr<Cluster>> _result;
    size_t  _num_time_points;
    BarcodeMutationGenerator _mutator;
    // Used to filter out those clusters that show up in t time points but does not
    // appear in the t+1 time points and its cluster size is smaller than this value.
    size_t _filter_csize_threshold;
};
}
#endif // TIMEPOINTSMERGER_H

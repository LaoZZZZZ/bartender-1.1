#ifndef CLUSTERCENTERLINKGENERATOR_H
#define CLUSTERCENTERLINKGENERATOR_H

#include "barcodecluster.hpp"
#include "centerrecalibrator.h"
#include "centerclustermapper.h"

#include <algorithm>
#include <array>
#include <list>
#include <memory>
#include <vector>
namespace barcodeSpace {
/**
  * This class is used to generate the map between
  * centers and clusters, which will be used for combining
  * the clusering result of two different time points.
*/
class ClusterCenterLinkGenerator
{
public:
    typedef BarcodeCluster Cluster;
    ClusterCenterLinkGenerator(double entropy_threshold,
                               int maximum_centers,
                               double p_value,
                               double error_rate);
    // Generates the links between centers and clusters.
    void Generate(const std::list<std::shared_ptr<Cluster>>& clusters,
                  const std::list<std::vector<double>>& entropies) {
        _center_cluster_link.reset();
        if (clusters.empty()) {
            return;
        }
        assert(clusters.size() == entropies.size());

        _clusters = &clusters;
        _entropies = &entropies;
        int max_id = 0;
        for (const auto& c : clusters) {
            max_id = std::max(max_id, c->ClusterID());
        }
        CenterClusterMapper* temp_mapper = new CenterClusterMapper(max_id + 1);
        assert(temp_mapper != NULL);
        _center_cluster_link.reset(temp_mapper);
        generateCenters();
    }
    const std::shared_ptr<CenterClusterMapper>& CenterClusterLink()const {
        return _center_cluster_link;
    }
private:
    void generateCenters();
    // temporary function used for debugging.
    void printFrequency(const std::vector<std::array<int,4>>& frequencies);

    const std::list<std::shared_ptr<Cluster>>* _clusters;
    const std::list<std::vector<double>>* _entropies;
    CenterRecalibrator  _recalibrator;
    std::shared_ptr<CenterClusterMapper> _center_cluster_link;
};
}   // namespace barcodeSpace
#endif // CLUSTERCENTERLINKGENERATOR_H

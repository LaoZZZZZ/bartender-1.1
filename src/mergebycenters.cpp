#include "mergebycenters.h"
#include "centerrecalibrator.h"
#include "centerclustermapperiterator.h"
#include "split_util.h"
#include "clustermergertester.h"

#include <cassert>
#include <unordered_set>
namespace barcodeSpace {
    MergeByCenters::MergeByCenters():
    _linker()

{
}
void MergeByCenters::merge(const std::list<std::shared_ptr<Cluster>>& clusters) {
    _linker.reset();
    _merged_clusters.clear();
    if (clusters.empty()) {
        return;
    }
    size_t klen = 0;
    klen = clusters.front()->center().length();
    int max_id = 0;
    for (const auto& c : clusters) {
        max_id = std::max(max_id, c->ClusterID());
	klen = std::max(klen, c->center().length());
    }
    CenterClusterMapper* temp_mapper = new CenterClusterMapper(max_id + 1);
    assert(temp_mapper != NULL);
    _linker.reset(temp_mapper);

    std::vector<double> temp_entropies(klen, 0);
    for (auto& c :  clusters) {

        std::shared_ptr<BarcodeCluster> matched_cluster;
        while (true) {
            if (_linker->hasCenter(c->center())) {
                    matched_cluster = _linker->getClusterByCenter(c->center());
                    _linker->removeCluster(matched_cluster->ClusterID());
            }
            
            if (!matched_cluster.get())
                break;
            c->merge(matched_cluster);
        }
        _linker->removeCluster(c->ClusterID());
        _linker->addPair(c, {c->center()});
    }
    // Get the updated clusters.
    //std::cout << "Finished merging" << std::endl;
    CenterClusterMapperIterator iter(_linker);
    while (iter.has_next()) {
        _merged_clusters.push_back(iter.next());
    }
    //std::cout << "picking up valid clusters for output" << std::endl;
}
}   // namespace barcodeSpace

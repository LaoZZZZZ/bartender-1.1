#include "mergebycenters.h"
#include "centerrecalibrator.h"
#include "centerclustermapperiterator.h"
#include "split_util.h"
#include "clustermergertester.h"

#include <cassert>
#include <unordered_set>
namespace barcodeSpace {
    MergeByCenters::MergeByCenters(const std::shared_ptr<CenterRecalibrator>& calibrator):
    _linker(), _recalibrator(calibrator)

{
}
void MergeByCenters::merge(const std::list<std::shared_ptr<Cluster>>& clusters,
                           const std::list<std::vector<double>>& entropies) {
    _linker.reset();
    _merged_clusters.clear();
    if (clusters.empty()) {
        return;
    }
    assert(clusters.size() == entropies.size());
    size_t klen = 0;
    klen = clusters.front()->center().length();
    int max_id = 0;
    for (const auto& c : clusters) {
        max_id = std::max(max_id, c->ClusterID());
    }
    CenterClusterMapper* temp_mapper = new CenterClusterMapper(max_id + 1);
    assert(temp_mapper != NULL);
    _linker.reset(temp_mapper);

    std::vector<std::string> centers;
    std::vector<double> temp_entropies(klen, 0);
    auto entropy_iter = entropies.begin();
    for (const auto& c :  clusters) {
        _recalibrator->IdentifyCenters(c->bpFrequency(), *entropy_iter, &centers);
        std::list<std::shared_ptr<Cluster>> matched_clusters;
        while (true) {
            matched_clusters.clear();
            for (const auto& cen : centers) {
                if (_linker->hasCenter(cen)) {
                    matched_clusters.push_back(_linker->getClusterByCenter(cen));
                    _linker->removeCluster(matched_clusters.back()->ClusterID());

                }
            }
            if (matched_clusters.empty())
                break;
            while (!matched_clusters.empty()) {
                c->merge(matched_clusters.front());
                matched_clusters.pop_front();
            }
            centers.clear();
            const std::vector<std::array<int, 4>>& ftable = c->bpFrequency();
            for(size_t i = 0; i < ftable.size(); ++i) {
                temp_entropies[i] = Entropy(ftable[i]);
            }
            _recalibrator->IdentifyCenters(ftable, temp_entropies, &centers);

        }

        _linker->removeCluster(c->ClusterID());
        _linker->addPair(c, centers);

        ++entropy_iter;
        centers.clear();
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

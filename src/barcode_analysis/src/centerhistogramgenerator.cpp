#include "centerhistogramgenerator.h"

namespace barcodeSpace {
CenterHistogramGenerator::CenterHistogramGenerator(const std::list<std::shared_ptr<cluster>>* clusters,
                                                   const std::list<std::vector<double>>* entropies,
                                                   double entropy_threshold,
                                                   size_t maximum_centers,
                                                   double p_value,
                                                   double error_rate) :
    _clusters(clusters), _entropies(entropies),
    _recalibrator(entropy_threshold, maximum_centers,
                  p_value, error_rate),
    _center_tracker(maximum_centers)
{
    generateCenters();
}
void CenterHistogramGenerator::generateCenters() {
    std::vector<kmer> centers;
    auto entropy_iter = _entropies->begin();
    for (const auto& c : *_clusters) {
        bool exceeded = _recalibrator.IdentifyCenters(c->bpFrequency(), *entropy_iter, &centers);
        _center_tracker.AddCount(centers.size() + exceeded);
        ++entropy_iter;
        centers.clear();
    }
}
}   // namespace barcodeSpace

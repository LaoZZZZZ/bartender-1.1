#ifndef MERGEBYCENTERS_H
#define MERGEBYCENTERS_H

#include "cluster.h"
#include "centerrecalibrator.h"
#include "centerclustermapper.h"

#include <list>
#include <memory>

namespace barcodeSpace {
class MergeByCenters
{
public:
    MergeByCenters(double entropy_threshold,
                   size_t maximum_centers,
                   double p_value,
                   double error_rate);
    void merge(const std::list<std::shared_ptr<cluster>>& clusters,
               const std::list<std::vector<double>>& entropies);
    const std::list<std::shared_ptr<cluster>>& clusters() const {
        return this->_merged_clusters;
    }
private:
    std::shared_ptr<CenterClusterMapper>    _linker;
    CenterRecalibrator                      _recalibrator;
    std::list<std::shared_ptr<cluster>>     _merged_clusters;
};
}   // namespace barcodeSpace
#endif // MERGEBYCENTERS_H

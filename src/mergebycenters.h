#ifndef MERGEBYCENTERS_H
#define MERGEBYCENTERS_H

#include "barcodecluster.hpp"
#include "centerclustermapper.h"

#include <list>
#include <memory>

namespace barcodeSpace {
class MergeByCenters
{
public:
    typedef BarcodeCluster Cluster;
    MergeByCenters();
    void merge(const std::list<std::shared_ptr<Cluster>>& clusters);
    const std::list<std::shared_ptr<Cluster>>& clusters() const {
        return this->_merged_clusters;
    }
private:
    std::shared_ptr<CenterClusterMapper>    _linker;
    std::list<std::shared_ptr<Cluster>>     _merged_clusters;
};
}   // namespace barcodeSpace
#endif // MERGEBYCENTERS_H

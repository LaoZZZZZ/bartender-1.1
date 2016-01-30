#ifndef CLUSTERSPLITTER_H
#define CLUSTERSPLITTER_H

#include "centerrecalibrator.h"
#include "barcodecluster.hpp"
#include "idgenerator.h"

#include <memory>
#include <vector>

namespace barcodeSpace {
class ClusterSplitter
{
public:
    typedef BarcodeCluster Cluster;
    ClusterSplitter(const std::shared_ptr<CenterRecalibrator>& _calibrator,
                    const std::shared_ptr<IDGenerator>& id_pool);
    
    // Given a cluster, this function will identify potential centers,
    // and split this cluster into several well separated clusters.
    // Each cluster will be assigned an unique ID.
    // It is possible that the return array has null pointer.
    std::vector<std::shared_ptr<Cluster>>   split(const std::shared_ptr<Cluster>& c);
    
    std::shared_ptr<IDGenerator>    IDPool()const {return _id_pool;}
private:
    std::shared_ptr<CenterRecalibrator>  _calibrator;
    std::shared_ptr<IDGenerator>    _id_pool;
};
}
#endif // CLUSTERSPLITTER_H

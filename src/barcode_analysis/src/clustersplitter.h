#ifndef CLUSTERSPLITTER_H
#define CLUSTERSPLITTER_H

#include "centerrecalibrator.h"
#include "cluster.h"
#include "idgenerator.h"

#include <memory>
#include <vector>

namespace barcodeSpace {
class ClusterSplitter
{
public:
    ClusterSplitter(const std::shared_ptr<CenterRecalibrator>& _calibrator,
                    const std::shared_ptr<IDGenerator>& id_pool);
    std::vector<std::shared_ptr<cluster>>   split(const std::shared_ptr<cluster>& c);
    std::shared_ptr<IDGenerator>    IDPool()const {return _id_pool;}
private:
    std::shared_ptr<CenterRecalibrator>  _calibrator;
    std::shared_ptr<IDGenerator>    _id_pool;
};
}
#endif // CLUSTERSPLITTER_H

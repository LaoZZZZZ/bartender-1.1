#include "clustersplitter.h"
#include "cluster.h"

#include <cassert>
#include <memory>
#include <vector>
using namespace std;
namespace barcodeSpace {

    ClusterSplitter::ClusterSplitter(const std::shared_ptr<CenterRecalibrator>& calibrator,
                                     const std::shared_ptr<IDGenerator>& id_pool):
        _calibrator(calibrator), _id_pool(id_pool)
    {
        assert(_calibrator.get());
        assert(_id_pool.get());
    }
    
    vector<shared_ptr<cluster>> ClusterSplitter::split(const shared_ptr<cluster>& c) {
        vector<shared_ptr<cluster>> result;
        std::vector<kmer> centers;
        vector<double> entropies;
        _calibrator->IdentifyCenters(c->bpFrequency(), entropies, &centers);
        return result;
    }
}   // namespace barcodeSpace

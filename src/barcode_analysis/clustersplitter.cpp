#include "clustersplitter.h"
#include "centerrecalibrator.h"
#include "cluster.h"
#include "dictatorcenter.h"
#include "dictatorcluster.h"
#include "idgenerator.h"
#include "kmerdecoder.hpp"
#include "split_util.h"
#include "util.h"

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
        
        _calibrator->IdentifyCenters(c->bpFrequency(), &centers);
        
        result.assign(centers.size(), nullptr);
        
        // Go through each unique barcode.
        // Assign it to its closest center.
        // It is possible that the distance between this observed barcode between
        // any identified center are same.
        // In this case, it will be assigned to the center with smallest index.
        // By this strategy, it might possible that this splitting strategy might
        // fail for some cases. That is, it return the original cluster.
        for (const auto& k_freq : c->barcodes()) {
            
            // Initialize this unique barcode as one cluster.
            std::shared_ptr<DictatorCenter> center_temp(new DictatorCenter(k_freq._key,
                                                                           c->center()->klen()));
            std::shared_ptr<cluster> c_temp(new dictatorCluster(center_temp,{k_freq._freq}));
            
            size_t dist = c->center()->klen();
            size_t closest_center = 0;
            for (size_t i = 0; i < centers.size(); ++i) {
                size_t d = kmerDistance(k_freq._key, centers[i], c->center()->klen());
                if (dist > d) {
                    closest_center = i;
                    dist = d;
                }
            }
            if (result[closest_center].get()) {
                result[closest_center]->merge(*c_temp);
            } else {
                result[closest_center] = c_temp;
                result[closest_center]->SetClusterID(_id_pool->nextID());
            }
        }
        return result;
    }
}   // namespace barcodeSpace

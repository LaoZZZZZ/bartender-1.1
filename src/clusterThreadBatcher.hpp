//
//  clusterThreadBatcher.hpp
//  barcode_project
//
//  Created by luzhao on 1/30/16.
//  Copyright © 2016 luzhao. All rights reserved.
//

#ifndef clusterThreadBatcher_hpp
#define clusterThreadBatcher_hpp

#include "barcodecluster.hpp"
#include "clusteralgorithm.h"
#include "threadwrapper.hpp"

#include <list>
#include <memory>
#include <vector>
namespace barcodeSpace {
    class ClusterThreadBatcher : public ThreadWrapper {
    public:
        typedef std::vector<std::list<std::shared_ptr<BarcodeCluster>>> Bucket;
        ClusterThreadBatcher(std::shared_ptr<clusterAlgorithm>& engine,
                             std::pair<size_t, size_t>& bucket_range,
                             const Bucket& bucket);
        const std::list<std::shared_ptr<BarcodeCluster>>& clusters()const {return _clusters;}
    private:
        void run();
        
        std::shared_ptr<clusterAlgorithm>   _cluster_engine;
        std::pair<size_t, size_t> _bucket_range;
        std::list<std::shared_ptr<BarcodeCluster>>   _clusters;
        const Bucket& _buckets;
    };
}

#endif /* clusterThreadBatcher_hpp */

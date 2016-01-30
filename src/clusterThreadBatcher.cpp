//
//  clusterThreadBatcher.cpp
//  barcode_project
//
//  Created by luzhao on 1/30/16.
//  Copyright © 2016 luzhao. All rights reserved.
//

#include "clusterThreadBatcher.hpp"

#include <cassert>
namespace barcodeSpace {
    ClusterThreadBatcher::ClusterThreadBatcher(
                std::shared_ptr<clusterAlgorithm>& engine,
                std::pair<size_t, size_t>& range,
                const Bucket& bucket) :
    _cluster_engine(engine), _bucket_range(range), _buckets(bucket){
        assert(engine.get());
    }
    void ClusterThreadBatcher::run() {
        _clusters.clear();
        for (size_t i = _bucket_range.first; i < _bucket_range.second; ++i) {
            _cluster_engine->clusterImp(_buckets[i]);
            _clusters.insert(_clusters.end(),
                             _cluster_engine->clusters().begin(),
                             _cluster_engine->clusters().end());
        }
    }
}
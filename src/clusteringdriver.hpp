//
//  clusteringdriver.hpp
//  barcode_project
//
//  Created by luzhao on 1/27/16.
//  Copyright © 2016 luzhao. All rights reserved.
//

#ifndef clusteringdriver_hpp
#define clusteringdriver_hpp

#include "barcodecluster.hpp"
#include "barcodepool.hpp"
#include "clustersbucketer.hpp"
#include "clustermergertester.h"
#include "formats.h"

#include <list>
#include <memory>
#include <vector>
#include <unordered_map>

namespace barcodeSpace {
    class ClusteringDriver {
    public:
        ClusteringDriver(size_t barcode_length,
                         size_t seed_len = 5,
			 size_t step = 1,
                         size_t num_threads = 1,
                         double error_rate = 0.01,
                         double zvalue = 3.08,
                         TESTSTRATEGY test_method = TWOPROPORTIONUNPOOLED,
			 size_t distance = 2,
                         double trim = 0.02,
                         double stopThres = 0.01
                         );
        bool clusterDrive(const std::shared_ptr<BarcodePool>& barcode_pool);
        const std::list<std::shared_ptr<BarcodeCluster>>& clusters(){return this->_clusters;}
    private:
        void init();
        void generateBucketRange();
        void transform(const std::shared_ptr<BarcodePool>& barcode_pool);
        void crossBinClustering(const std::vector<std::list<std::shared_ptr<BarcodeCluster>>>& cbins);
        ////////////////params for shattering the barcode into bins////////////////////////////
        size_t                                          _barcode_length;
        size_t                                          _seed_length;
        size_t                                          _step;
        size_t                                          _num_threads;
        std::unique_ptr<ClusterBucketer>                _shatter_machine;
        
        double                                          _error_rate;
        //////////////params for clusting the bins and barcodes clusters////////////////////////
        std::list<std::shared_ptr<BarcodeCluster>>      _clusters;
        double                                          _trim;
        size_t                                          _dist_threshold; // used to define whether two barcodes are similar.
        double                                          _zvalue; //used for merge test
        TESTSTRATEGY                                    _pool; // indicate whether use the pooled test or indepedent test.
        std::shared_ptr<ClusterMergerTester>            _tester;
        // the cluster size difference between current clustring step with the previous step
        // if the difference/previous cluster size smaller than this threshold
        // the clustering algorithm stop
        double                                          _stopThres;
        // used to tell which cluster is low frequency and which is high frequency
        int                                             _splitThreshold;
        std::vector<std::pair<size_t, size_t>>          _bucket_ranges;
    };
}   // namespace barcodeSpace

#endif /* clusteringdriver_hpp */

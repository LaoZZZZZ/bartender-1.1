//
//  clusteringdriver.cpp
//  barcode_project
//
//  Created by luzhao on 1/27/16.
//  Copyright © 2016 luzhao. All rights reserved.
//

#include "clusteralgorithm.h"
#include "clusteringdriver.hpp"
#include "clustermergerunpooltester.h"
#include "clustermergerpooltester.h"
#include "clustermergeronesampletester.hpp"
#include "clusteringwithtest.h"
#include "distanceselector.h"
#include "formats.h"
#include "meanestimator.hpp"
#include "meansequentialestimator.h"
#include "seedselector.hpp"
#include "clusterThreadBatcher.hpp"

#include "timer.h"
#include "typdefine.h"



#include <cassert>
#include <list>
#include <vector>

using namespace std;
namespace barcodeSpace {
    ClusteringDriver::ClusteringDriver(size_t barcode_length,
                                       size_t seed_len,
				       size_t step,
                                       size_t num_threads,
                                       double error_rate,
                                       double zvalue,
                                       TESTSTRATEGY test_method,
				       size_t distance,
                                       double trim,
                                       double stopThres):
    _barcode_length(barcode_length), _seed_length(seed_len),
    _step(step),
    _num_threads(num_threads), _error_rate(error_rate),
    _zvalue(zvalue), _pool(test_method),
    _trim(trim), _stopThres(stopThres), _dist_threshold(distance)
    {
        assert(this->_stopThres < 1);
        init();
    }
    void ClusteringDriver::init() {
        this->_clusters.clear();
        if (_pool == TWOPROPORTIONPOOLED) {
            std::cout << "Using two sample pooled test" << std::endl;
            _tester.reset(new ClusterMergerPoolTester(_zvalue));
        } else if (_pool == TWOPROPORTIONUNPOOLED) {
            std::cout << "Using two sample unpooled test" << std::endl;
            _tester.reset(new ClusterMergerUnPoolTester(_zvalue));
        } else  {
            std::cout << "Using one sample test" << std::endl;
            assert(_pool == ONEPROPORTION);
            _tester.reset(new ClusterMergerOneSampleTester(_zvalue, _error_rate));
        }
        generateBucketRange();
    }
    bool ClusteringDriver::clusterDrive(const std::shared_ptr<BarcodePool>& barcode_pool) {

        Timer* t = new realTimer(cout);

        SeedSelector selector(_barcode_length);
        selector.addBarcode(barcode_pool);
        vector<int> seeds_positions = selector.getSeedsPositions();
        _shatter_machine.reset(new ClusterBucketer(seeds_positions, _seed_length,_step));
        
        std::cout << "transforming the barcodes into clusters" << std::endl;
        // 1. Transform the barcode table into a list of clusters
        this->transform(barcode_pool);
        MeanEstimator mean_estimator(_trim);
        
        this->_splitThreshold = ceil(mean_estimator.mean(_clusters)) + 1;
        
        std::cout << "Initial number of unique reads:  " << _clusters.size() <<std::endl;
        
        std::cout << "The distance threshold is " << _dist_threshold << std::endl;
        std::cout << "Cluster splitting threshold is  " << _splitThreshold << std::endl;
        
        // 2. First try to assign low frequency barcode to high frequency barcode
        // only consider those barcode distance is equal to 1.
        size_t sz(this->_clusters.size());
        
        size_t total(0);
        
        while (!_shatter_machine->done()) {
            if (sz == 0)
                break;
            total++;
            cout<<"Clustering iteration "<<total<<endl;
            _shatter_machine->shatter(_clusters);
            this->crossBinClustering(_shatter_machine->Bins());
            //size_t tmp(this->_clusters.size());
            //if(static_cast<double>(sz - tmp)/sz < this->_stopThres)
            //    break;
           // sz = tmp;
        }

        std::cout<<"Clustering took  ";
        
        delete t;
	std::cout << std::endl;
        return true;
    }
    void ClusteringDriver::transform(const std::shared_ptr<BarcodePool>& barcode_pool) {
        _clusters.clear();
        size_t max_size = 0;
        
        for(size_t index = 0; index < barcode_pool->size(); ++ index){
            
            if (barcode_pool->barcode(index).length() != _barcode_length)
                continue;
            std::shared_ptr<BarcodeCluster> ptemp(new BarcodeCluster(index));
            _clusters.push_back(ptemp);
        }

        //DistanceSelector selector(_error_rate, 0.5, _barcode_length);
        //_dist_threshold = selector.calculateDistance(max_size) + 1;
    }
    void ClusteringDriver::crossBinClustering(const vector<list<shared_ptr<BarcodeCluster>>>& cbins){
        if(!this->_clusters.empty()){
            this->_clusters.clear();
            vector<std::shared_ptr<ClusterThreadBatcher>> batchers;
            for (size_t i = 0; i < _bucket_ranges.size(); ++i) {
                clusterAlgorithm* temp = new ClusteringWithTest(_splitThreshold,_barcode_length,
                    _dist_threshold,_tester);
                std::shared_ptr<clusterAlgorithm> ptem(temp);
                std::shared_ptr<ClusterThreadBatcher> batcher(new ClusterThreadBatcher(ptem,_bucket_ranges[i],cbins));
                batchers.push_back(batcher);
                batchers.back()->start();
            }
            for (const auto& b : batchers) {
                b->join();
                this->_clusters.insert(this->_clusters.end(),b->clusters().begin(),b->clusters().end());
            }
        }
    }
    void ClusteringDriver::generateBucketRange() {
        size_t total = static_cast<size_t>(pow(4,_seed_length));
        size_t range_size = total / _num_threads;
        if (total % _num_threads) {
            range_size += 1;
        }
        size_t start = 0;
        while (start < total) {
            size_t end = min(total, start + range_size);
            _bucket_ranges.push_back({start, end});
            start = end;
        }
    }
}   // namespace barcodeSpace

//
//  clusterpruner.cpp
//  barcode_project
//
//  Created by luzhao on 1/16/16.
//  Copyright © 2016 luzhao. All rights reserved.
//

#include "clusterpruner.hpp"
#include "centerrecalibrator.h"
#include "barcodecluster.hpp"
#include "clustersplitter.h"
#include "idgenerator.h"
#include "meansequentialestimator.h"
#include "mergebycenters.h"
#include "split_util.h"
#include <math.h>

#include <algorithm>
#include <cassert>
#include <list>
#include <set>
#include <vector>

using namespace std;
namespace barcodeSpace {
    ClusterPruner::ClusterPruner(double entropy_threshold, // Entropy thresholds that define mixture positions
                                 size_t maximum_centers,   // The maximum centers to be detected.
                                 double z_value,
                                 double percentage_threshold,// The percentage that the bp is viewed as potential center
                                 size_t cutoff,
                                 double error_rate) : _split_size(0), _cutoff(cutoff){
        _recalibrator.reset(new CenterRecalibrator(entropy_threshold, maximum_centers, z_value, percentage_threshold));
        assert(_recalibrator);
        splitSize(z_value, error_rate, percentage_threshold);
    }
    // By calculate the power, to determine the split cluster size.
    void ClusterPruner::splitSize(double zvalue,
                                  double error_rate,
                                  double percentage_threshold) {
        _split_size = static_cast<size_t>(zvalue * zvalue * (1 - error_rate) * error_rate / pow(percentage_threshold - error_rate, 2));
    }
    void ClusterPruner::prune(const std::list<std::shared_ptr<Cluster> > &clusters) {
        
        _clusters.clear();
        
        
        // Calculate the mean size of clusters.
        //MeanSequentialEstimator mean_estimator;
        std::set<int>   taken_ids;
        for (const auto& c : clusters) {
          //  mean_estimator.Add(c->size());
            taken_ids.insert(c->ClusterID());
        }
	//cout << "splits the clusters" << endl;
        // Only split clusters whose size is above the mean cutoff.
        //size_t mean_cutoff = static_cast<int>(mean_estimator.GetMean());
        std::shared_ptr<IDGenerator> id_pool(new IDGenerator(taken_ids));
        std::list<std::vector<double>> entropies;
        ClusterSplitter splitter(_recalibrator, id_pool);
        for (const auto& c : clusters) {
            if (c->size() <= _split_size) {
                _clusters.push_back(c);
                entropies.push_back(Entropy(c->bpFrequency()));
            } else {
                vector<shared_ptr<Cluster>> splited_clusters = splitter.split(c);
                for (const auto& s : splited_clusters) {
                    if (s.get()) {
                        _clusters.push_back(s);
                        entropies.push_back(Entropy(s->bpFrequency()));
                    }
                }
            }
        }
	//cout << "merge the clusters" << endl;
        // Merge cluster who has the same centers.
        MergeByCenters merger(_recalibrator);
        merger.merge(_clusters, entropies);
        _clusters = merger.clusters();
        // Filter out cluster
        // 3.Remove those clusters whose size is below the cutoff.
        list<shared_ptr<Cluster>> filtered_clusters;
        for (const auto& c : _clusters) {
            if (c->size() >= this->_cutoff) {
                filtered_clusters.push_back(c);
            }
        }
        //_clusters = std::move(filtered_clusters);
        std::swap(_clusters, filtered_clusters);

    }
}

#include "multipletimepointsprocessor.h"

#include "centerclustermapper.h"
#include "barcodecluster.hpp"
#include "clusterloader.h"
#include "timepointsmerger.h"
#include "clustercenterlinkgenerator.h"
#include "errorrateestimator.h"
#include "mergebycenters.h"
#include "centerclustermapperiterator.h"

#include <cassert>
#include <iostream>
#include <list>
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

using namespace std;
namespace barcodeSpace {

MultipleTimePointsProcessor::MultipleTimePointsProcessor(const InputFile& cluster_result_files,
                                                         std::shared_ptr<ErrorRateEstimator> error_estimator,
                                                         std::shared_ptr<ClusterCenterLinkGenerator> link_generator,
                                                         std::shared_ptr<MergeByCenters>    center_merger,

                                                         size_t csize_filter) :
    _cluster_result_files(cluster_result_files),
    _error_estimator(error_estimator), _link_generator(link_generator),
    _center_merger(center_merger), _csize_filter(csize_filter)
{
    assert(error_estimator.get());
    assert(link_generator.get());
    assert(center_merger.get());
}
bool MultipleTimePointsProcessor::isValidTrajectory(const std::vector<size_t>& traj) {
	bool found_zero = false;
	for (const auto& t : traj) {
		if (t != 0 && found_zero) {
			return false;	
		}
		else if (t == 0) {
			found_zero = true;
		}
	}	
	return true;
}
void MultipleTimePointsProcessor::process() {
    // key is the barcode length
    // value is the clusters in this length.
    unordered_map<int, list<shared_ptr<Cluster>>>    mediate_clusters;
    std::unordered_map<int, shared_ptr<CenterClusterMapper>>    combined_link;
    std::unordered_map<int, shared_ptr<CenterClusterMapper>>    single_link;
    size_t num_time_points = 0;
    // Go through each pair of clustering result file.
    // combine those cluster results in backward manner.
    int len = static_cast<int>(_cluster_result_files.size()) - 1;
    for (int i = len; i >= 0; --i) {
        cout << "Current generation " << i << endl;
        ClusterLoader loader(_cluster_result_files[i].first, _cluster_result_files[i].second);
        std::unordered_map<int, list<shared_ptr<Cluster>>> new_clusters;
        loader.LoadClusters(&new_clusters);
	single_link.clear();
        // Estimate the sequence error for this single time point.
        for (const auto& clusters : new_clusters) {
            _error_estimator->Estimate(clusters.second, true);
            _original_error_rates[clusters.first].push_back(_error_estimator->ErrorRate());
	   
            _link_generator->Generate(clusters.second);

            single_link[clusters.first] = _link_generator->CenterClusterLink();
        }
        // Combined those clusters that have the same length.
        for (const auto& batch : combined_link) {
            TimePointsMerger merger(batch.second, single_link[batch.first],
                    batch.first, num_time_points, _csize_filter);
            merger.merge();
            const list<shared_ptr<Cluster>>& merged_clusters = merger.mergedClusters();
            _center_merger->merge(merged_clusters);
            _error_estimator->Estimate(_center_merger->clusters(), true);
            mediate_clusters[batch.first] = _center_merger->clusters();

            _link_generator->Generate(_center_merger->clusters());

            combined_link[batch.first] = _link_generator->CenterClusterLink();
            if (_combined_error_rates.count(batch.first)) {
                _combined_error_rates[batch.first].push_back(_error_estimator->ErrorRate());
            } else {
                _combined_error_rates.insert({batch.first, {_error_estimator->ErrorRate()}});
            }


        }
        // keep those barcode whose length does not show up in the previously combined result.
        for (const auto& batch: single_link) {
            if (0 == combined_link.count(batch.first)) {
                TimePointsMerger merger(NULL, batch.second ,batch.first, num_time_points, 1);
                merger.merge();
                const list<shared_ptr<Cluster>>& merged_clusters = merger.mergedClusters();
                _center_merger->merge(merged_clusters);
                _error_estimator->Estimate(merged_clusters, true);
                mediate_clusters[batch.first] = _center_merger->clusters();
                _error_estimator->Estimate(_center_merger->clusters(), true);

                _link_generator->Generate(_center_merger->clusters());

                combined_link[batch.first] = _link_generator->CenterClusterLink();
                if (_combined_error_rates.count(batch.first)) {
                    _combined_error_rates[batch.first].push_back(_error_estimator->ErrorRate());
                } else {
                    _combined_error_rates.insert({batch.first, {_error_estimator->ErrorRate()}});
                }
            }
        }
        std::cout << "Finished merging generation " << i << std::endl;

        ++num_time_points;
    }
    std::cout << "Filter out low frequency clusters whose size is below " << _csize_filter << std::endl;
    int mostPervasiveBarcodeLength = -1;
    int numOfCluster = 0;
    for (const auto& clusters : mediate_clusters) {
        std::list<std::shared_ptr<Cluster>> cluster_result;
        for (const auto& c : clusters.second) {
	    if(c->size() >= _csize_filter) {
		cluster_result.push_back(c);
	    } 
        }
        _result_clusters[clusters.first] = cluster_result;
	if (cluster_result.size() > numOfCluster) {
        	mostPervasiveBarcodeLength = clusters.first; 
		numOfCluster = cluster_result.size();
	}
    }
    // Find the most pervasive barcode length group and estimate the error rate from there.
    
    _error_estimator->Estimate(_result_clusters[mostPervasiveBarcodeLength], false);
}
void MultipleTimePointsProcessor::SanityCheck(const unordered_map<int, list<shared_ptr<Cluster>>>& clusters) {
    for (const auto& cls : clusters) {
        // Check the uniqueness of cluster id.
        unordered_set<int> ids;
        std::cout << cls.second.size() << std::endl;
        for(const auto& c : cls.second) {
            if (ids.count(c->ClusterID())) {
                throw std::runtime_error("Duplicate ids " + std::to_string(c->ClusterID()));
            }
            ids.insert(c->ClusterID());
        }
        // Check the center

    }
    return ;
}
}   // namespace barcodeSpace

#include "errorrateestimator.h"
#include "split_util.h"

#include <algorithm>
#include <iostream>
#include <list>
#include <sstream>
#include <stdexcept>
#include <vector>

using namespace std;
namespace barcodeSpace {

ErrorRateEstimator::ErrorRateEstimator(double entropy_threshold, size_t cluster_size, size_t maximum_barcodes)
    : _error_rate(0), _entropy_threshold(entropy_threshold), _cluster_size(cluster_size), _maximum_barcodes(maximum_barcodes)
{
}

//
void ErrorRateEstimator::Estimate(const std::list<std::shared_ptr<Cluster>>& clusters, bool silence) {
    _entropies.clear();
    int majority = 0;
    int total_base_pair = 0;
    int total_barcodes = 0;
    // Start from the largest clusters.
    for (const auto& c : clusters) {
        _entropies.push_back(vector<double>());

        double tmp_total_majority = 0;
        double single_total = 0;
        double tmp_majority = 0;
        double tmp_total = 0;
        double max_entropy = 0;

        for(const auto& bp : c->bpFrequency()) {
            double temp_entropy = Entropy(bp, &tmp_majority, &tmp_total);
            max_entropy = std::max(max_entropy, temp_entropy);
            _entropies.back().push_back(temp_entropy);
            tmp_total_majority += tmp_majority;
            single_total += tmp_total;
        }
        // If the maximum entropy is less than the entropy threshold
        // and the cluster size is larger than the cluster size threshold.
        if (total_barcodes < _maximum_barcodes &&
            max_entropy < _entropy_threshold &&
             c->size() >= _cluster_size) {
            majority += tmp_total_majority;
            total_base_pair += single_total;
            total_barcodes += c->size();
        }

    }
    if (!silence) {
    	// Could not collect enough high quality barcode for estimation of error rate.
    	if (total_barcodes > 0 && total_barcodes < _maximum_barcodes) {
        	std::cerr << "Could not collect enough barcodes from high quality clusters(max entropy < "
                     	<< _entropy_threshold << ", " << "cluster size >= " << _cluster_size << " ) "
                  	<< "to estimate the error rate!" << endl;
        	std::cerr << "The estimated error rate might not be accurate and reliable!" <<endl;
    	}
    	// Can not find a single cluster that satisfy the specified condition.
    	if (total_base_pair == 0) {
        	stringstream log;
        	log << "Could not find any high quality clusters(max entropy < "
                     	<< _entropy_threshold << ", " << "cluster size > " << _cluster_size << " ) "
                  	<< "to estimate the error rate!" << endl;
        	log << "Please use the result cautiously(Better to check the input data)!" <<endl;
        	//throw runtime_error(log.str());
        	std::cerr << log.str() << std::endl;
    	}
    }
    if (total_base_pair > 0){
        _error_rate = 1 - static_cast<double>(majority) / total_base_pair;
    } else {
       	_error_rate = 0;
    }
}

}   // namespace barcodeSpace

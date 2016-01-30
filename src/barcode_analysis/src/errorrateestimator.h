#ifndef ERRORRATEESTIMATOR_H
#define ERRORRATEESTIMATOR_H

#include "cluster.h"

#include <algorithm>
#include <array>
#include <cassert>
#include <list>
#include <memory>
#include <vector>

namespace barcodeSpace {

// It is used to estimate the error rate using top high quality clusters.
// In the meantime, the entropy of each cluster will be calculated.
// The entropy value will be used to evaluate the data quality in the next step.
// TODO(lu): Add the iterative way to estimate the error rate!.
class ErrorRateEstimator
{
public:
    ErrorRateEstimator(double entropy_threshold, size_t cluster_size, size_t maximum_barcodes);

    // Estimates the error rate given a list of clusters.
    void Estimate(const std::list<std::shared_ptr<cluster>>&, bool silence);

    // Gets the error rate.
    double ErrorRate()const{return _error_rate;}

    // Gets the entropies.
    const std::list<std::vector<double>>& Entropies() const { return _entropies;}

private:
    std::list<std::vector<double>> _entropies;  // The entropies for all clusters. for later usage.
    double _error_rate;         // The estimated error rate.
    double _entropy_threshold;  // The entropy threshold that is used to label the quality of a clusters.
    size_t _cluster_size;
    int _maximum_barcodes;  // The minimum number of barcoeds that are used to estimate the error rate.
};
}   // namespace barcodeSpace

#endif // ERRORRATEESTIMATOR_H

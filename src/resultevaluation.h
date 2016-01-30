#ifndef RESULTEVALUATION_H
#define RESULTEVALUATION_H

#include "centerstracker.h"
#include "cluster.h"
#include "errorrateestimator.h"
#include "centerhistogramgenerator.h"

#include <list>
#include <memory>

namespace barcodeSpace {

// Evaluates the clustering result.
// 1. Estimates the true sequence error.
// 2. Calculates the entropies of al clusters.
// 3. Tell whether the data is good enough for running our algorithm.

class ResultEvaluation
{
public:
    ResultEvaluation(std::list<std::shared_ptr<cluster>>* clusters);
    void EstimateError(double entropy_threshold_for_error,
                       int cluster_size,
                       int minimum_barcodes);
    // Identify potential centers for each clusters.
    void RecalibrateCenters(double entropy_threshold,
                            size_t maximum_center,
                            double p_value);
private:
    std::list<std::shared_ptr<cluster>>* _clusters;
    double  _error_rate;
    std::unique_ptr<ErrorRateEstimator> _estimator;
    std::unique_ptr<CentersTracker>     _center_number_hist;
};
}   // namespace barcodeSpace
#endif // RESULTEVALUATION_H

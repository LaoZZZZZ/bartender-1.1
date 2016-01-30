#include "resultevaluation.h"

#include "errorrateestimator.h"
#include "centerhistogramgenerator.h"

#include <algorithm>
#include <array>
#include <vector>

using std::vector;
using std::array;
namespace barcodeSpace {
ResultEvaluation::ResultEvaluation(std::list<std::shared_ptr<cluster>>* clusters)
    :_clusters(clusters), _error_rate(0.0)
{
}
void ResultEvaluation::EstimateError(double entropy_threshold_for_error,
                                     int cluster_size,
                                     int minimum_barcodes) {
    ErrorRateEstimator* estimator = new ErrorRateEstimator(entropy_threshold_for_error,
                                                           cluster_size,
                                                           minimum_barcodes);
    _estimator.reset(estimator);
    _estimator->Estimate(*_clusters, false);
    _error_rate = _estimator->ErrorRate();

}

// Identify potential centers for each clusters.
void ResultEvaluation::RecalibrateCenters(double entropy_threshold,
                        size_t maximum_center,
                        double p_value) {
    CenterHistogramGenerator center_histogram_gen(_clusters,
                                                  &(_estimator->Entropies()),
                                                  entropy_threshold,
                                                  maximum_center,
                                                  p_value,
                                                  _estimator->ErrorRate());

    _center_number_hist.reset(&(center_histogram_gen.Histogram()));
}
}   // namespace barcodeSpace

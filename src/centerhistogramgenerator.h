#ifndef CENTERHISTOGRAMGENERATOR_H
#define CENTERHISTOGRAMGENERATOR_H

#include "barcodecluster.hpp"
#include "centerrecalibrator.h"
#include "centerclustermapper.h"
#include "centerstracker.h"

#include <list>
#include <memory>
#include <vector>


namespace barcodeSpace {
/**
  * This class is used to generate the histogram of number of CentersTracker
  * This result will be used for evaluating the clustering result of single time point.
*/
class CenterHistogramGenerator
{
public:
    typedef BarcodeCluster Cluster;
    CenterHistogramGenerator(const std::list<std::shared_ptr<Cluster>>* clusters,
                             const std::list<std::vector<double>>* entropies,
                             double entropy_threshold,
                             size_t maximum_centers,
                             double p_value,
                             double error_rate);
    const CentersTracker& Histogram()const {return _center_tracker;}
    CentersTracker& Histogram() {return _center_tracker;}
private:
    void generateCenters();

    const std::list<std::shared_ptr<Cluster>>* _clusters;
    const std::list<std::vector<double>>* _entropies;
    CenterRecalibrator  _recalibrator;
    CentersTracker      _center_tracker;
};
}
#endif // CENTERHISTOGRAMGENERATOR_H

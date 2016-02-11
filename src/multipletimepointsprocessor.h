#ifndef MULTIPLETIMEPOINTSPROCESSOR_H
#define MULTIPLETIMEPOINTSPROCESSOR_H

#include "barcodecluster.hpp"
#include "clustercenterlinkgenerator.h"
#include "errorrateestimator.h"
#include "mergebycenters.h"
#include "typdefine.h"

#include <list>
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace barcodeSpace {
// Given a list of paired cluster result
// combine those cluster result in backward manner
class MultipleTimePointsProcessor
{
public:
    typedef BarcodeCluster Cluster;
    typedef std::vector<std::pair<std::string, std::string>> InputFile;
    //TODO(lu): since link_generator partially depends on the error_estimator,
    // the ResultEvaluation have the errorestimator and linkgenerator together,
    // update this interface once the mixturebptester is finalized.
    // The current version is only used to test the Sashy's data.
    MultipleTimePointsProcessor(const InputFile& cluster_result_files,
                                std::shared_ptr<ErrorRateEstimator> error_estimator,
                                std::shared_ptr<ClusterCenterLinkGenerator> link_generator,
                                std::shared_ptr<MergeByCenters>    center_merger,
                                size_t csize_filter = 3);
    void process();
    const std::unordered_map<int, std::list<std::shared_ptr<Cluster>>>&   result() {return _result_clusters;}
    const std::unordered_map<int, std::vector<double>>&  compoundErrorRate() const {return _combined_error_rates;}
    const std::unordered_map<int, std::vector<double>>&  singleErrorRate() const {return _original_error_rates;}
private:
    void SanityCheck(const std::unordered_map<int, std::list<std::shared_ptr<Cluster>>>&);
    bool isValidTrajectory(const std::vector<size_t>& traj);
    std::vector<std::pair<std::string, std::string>>    _cluster_result_files;
    std::shared_ptr<ErrorRateEstimator> _error_estimator;
    std::shared_ptr<ClusterCenterLinkGenerator> _link_generator;
    std::shared_ptr<MergeByCenters> _center_merger;

    std::unordered_map<int, std::list<std::shared_ptr<Cluster>>> _result_clusters;
    std::unordered_map<int, std::vector<double>>     _combined_error_rates;
    std::unordered_map<int, std::vector<double>>     _original_error_rates;
    // Used to filter out those clusters that show up in t time points but does not
    // appear in the t+1 time points and its cluster size is smaller than this value.
    size_t                                           _csize_filter;
};
}   // namespace barcodeSpace
#endif // MULTIPLETIMEPOINTSPROCESSOR_H

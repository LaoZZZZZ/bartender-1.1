#ifndef CENTERRECALIBRATOR_H
#define CENTERRECALIBRATOR_H

#include "typdefine.h"
#include "mixturebptester.h"
#include "kmers_dictionary.h"

#include <array>
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

namespace barcodeSpace {

class CenterRecalibrator
{
public:
    CenterRecalibrator(double entropy_threshold,
                       size_t maximum_center,
                       double p_value,
                       double error_rate);
    // Identify the centers for the given base pair frequncy.
    // Return true if the given cluster has more than then maximum_centers threshold.
    // Just for logging the low quality clusters.
    bool IdentifyCenters(const std::vector<std::array<int, 4>>& cluster_bp_frequency,
                         const std::vector<double>& entropies,
                         std::vector<std::string>* centers);
    bool IdentifyCenters(const std::vector<std::array<int, 4>>& cluster_bp_frequency,
                         std::vector<std::string>* centers);
private:
    std::vector<kmer> IdentifyCentersImp(const std::vector<std::array<int, 4>>& base_freq,
                                         const std::vector<double>& entropies,
                                         bool& truncated);
     
    std::vector<std::string> IdentifyCentersOptimalImp(const std::vector<std::array<int, 4>>& base_freq,
                                         const std::vector<double>& entropies,
                                         bool& truncated);
    double _entropy_threshold;    //
    size_t _maximum_centers; // The maximum centers that will be reported for each cluster
                            // If the number of potential centers is larger than this threshold,
                            // Only report the first maximum_centers.
    double _p_value;
    double _error_rate;
    MixtureBPTester _mixture_bp_tester;
    std::shared_ptr<kmersDictionary> kmersDictionaryPtr;
};
}   // namespace barcodeSpace
#endif // CENTERRECALIBRATOR_H

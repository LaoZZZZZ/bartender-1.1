#include "centerrecalibrator.h"
#include "split_util.h"
#include "typdefine.h"

#include <algorithm>
#include <array>
#include <cassert>
#include <vector>

namespace barcodeSpace {

CenterRecalibrator::CenterRecalibrator(double entropy_threshold,
                                       size_t maximum_center,
                                       double p_value,
                                       double error_rate)
    : _entropy_threshold(entropy_threshold), _maximum_centers(maximum_center),
      _p_value(p_value), _error_rate(error_rate), _mixture_bp_tester(_p_value, _error_rate)
{

}

bool CenterRecalibrator::IdentifyCenters(const std::vector<std::array<int, 4>>& base_freq,
                                         const std::vector<double>& entropies,
                                         std::vector<kmer>* centers) {
    bool more_centers = false;
    //std::vector<kmer> identified_centers = IdentifyCentersImp(base_freq, entropies, more_centers);
    std::vector<kmer> identified_centers =
            IdentifyCentersOptimalImp(base_freq, entropies, more_centers);

    centers->swap(identified_centers);
    //_center_tracker.AddCount(centers->size() + more_centers);
    return more_centers;

}
bool CenterRecalibrator::IdentifyCenters(
        const std::vector<std::array<int, 4>>& cluster_bp_frequency,
        std::vector<kmer>* centers) {
    std::vector<double> entropies;
    for (const auto& bp_freq : cluster_bp_frequency) {
        entropies.push_back(Entropy(bp_freq));
    }
    return IdentifyCenters(cluster_bp_frequency, entropies, centers);
}
// This function is the replacement of function IdentifyCentersImp.
// Cause this function will pick up the most representative centers.
std::vector<kmer> CenterRecalibrator::IdentifyCentersOptimalImp(const std::vector<std::array<int, 4>>& base_freq,
                                                         const std::vector<double>& entropies,
                                                         bool& truncated) {
    std::vector<kmer>   centers;
    centers.push_back(0);
    std::vector<bool> checked(entropies.size(), false);
    std::vector<std::pair<double, size_t>> heap_entropy;
    for(size_t i = 0; i < entropies.size(); ++i) {
        heap_entropy.push_back({entropies[i],i});
    }
    std::make_heap(heap_entropy.begin(), heap_entropy.end(), [](const std::pair<double, size_t>& v1, const std::pair<double, size_t>& v2) {return v1.first < v2.first;});
    kmer mask = pow(4, entropies.size()) - 1;
    while(!truncated && !heap_entropy.empty()) {
        size_t index = heap_entropy.front().second;
        if (heap_entropy.front().first > _entropy_threshold) {
            checked[index] = true;
            // The majority base pair at index position.
            int majority = static_cast<int>(std::max_element(base_freq[index].begin(),base_freq[index].end()) - base_freq[index].begin());
            kmer majority_bits = majority;
            majority_bits = majority_bits << (entropies.size() * 2 - 2*(index + 1));
            for(auto& c : centers) {
                c |= majority_bits;
            }
            size_t sz = centers.size();
            for (int bp = 0; bp < 4; ++bp) {
                if (bp == majority) continue;
                //proportion = base_freq[index][bp] / total;
                // mixture base pair position
                if (_mixture_bp_tester.IsPotentialMixture(base_freq[index], bp)) {
                    for (size_t cp = 0; cp < sz; ++cp) {
                        if (centers.size() >= _maximum_centers) {
                            truncated = true;
                            bp = 4;
                            break;
                        }
                        kmer remain_bits = centers[cp] & (mask - majority_bits);
                        kmer mut = bp;
                        mut = mut << (entropies.size() * 2 - 2*(index + 1));
                        centers.push_back(remain_bits | mut);
                    }
                }
            }
            // get the next largest entropies position.
            std::pop_heap(heap_entropy.begin(), heap_entropy.end());
            heap_entropy.pop_back();
        } else {
            break;
        }
    }
    // complete the remaining positions with the majority base pair.
    for (size_t index = 0; index < entropies.size(); ++index) {
        if (!checked[index]) {
            size_t majority = std::max_element(base_freq[index].begin(), base_freq[index].end()) - base_freq[index].begin();
            kmer k = majority;
            k = k << (entropies.size() * 2 - 2*(index + 1));
            for(auto& c : centers) {
                c |= k;
            }
        }
    }
    return centers;
}
// (Deprecated)
// Identify all potential centers based on the entropy values
// and the propotion of each base pair at the potential mixture position.
// TODO(lu): The current method just find the first k centers starting from the beginning.
//           Improvement: Find the first k most representative centers(those centers that
//           represent the most base pairs.
std::vector<kmer> CenterRecalibrator::IdentifyCentersImp(const std::vector<std::array<int, 4>>& base_freq,
                                                         const std::vector<double>& entropies,
                                                         bool& truncated) {
    std::vector<kmer>   centers;
    centers.push_back(0);

    // Keep track the low qality base pair positions.
    for(size_t index = 0; index < entropies.size(); ++index) {

        // The majority base pair at index position.
        size_t majority = std::max_element(base_freq[index].begin(), base_freq[index].end()) - base_freq[index].begin();
        for(auto& c : centers) {
            c = c << 2;
            c |= majority;
        }

        // There are potential multiple centers since the entropy value is larger than the threshold.
        // and the number of centers is under the maximum centers threshold.
        if (entropies[index] >= _entropy_threshold && !truncated) {
            //double total = std::accumulate(base_freq[index].begin(), base_freq[index].end(), 0);
            //assert(total > 0);
            //double proportion = 0.0f;
            size_t sz = centers.size();
            for (int bp = 0; bp < 4; ++bp) {
                if (bp == majority) continue;
                //proportion = base_freq[index][bp] / total;
                // mixture base pair position
                if (_mixture_bp_tester.IsPotentialMixture(base_freq[index], bp)) {
                    for (size_t cp = 0; cp < sz; ++cp) {
                        if (centers.size() >= _maximum_centers) {
                            truncated = true;
                            bp = 4;
                            break;
                        }
                        kmer k = (centers[cp] >> 2);
                        k = k << 2;
                        k |= bp;
                        centers.push_back(k);
                    }
                }
            }
        }
    }
    if (centers.size() > _maximum_centers) {
        truncated = true;
        centers.resize(_maximum_centers);
    }
    return centers;
}
}   // namespace barcodeSpace

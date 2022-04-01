#ifndef SPLIT_UTIL_H
#define SPLIT_UTIL_H

#include <algorithm>
#include <array>
#include <cmath>
#include <numeric>
#include <stdint.h>

namespace barcodeSpace {

inline double Entropy(const std::array<uint64_t, 4>& base_freq) {
    double majority = *std::max_element(base_freq.begin(), base_freq.end());

    double sum = std::accumulate(base_freq.begin(), base_freq.end(), 0);
    if(sum == majority) {
        return 0.0;
    } else {
        double ratio = majority / sum;
        return -1 * (ratio * log2(ratio) + (1 - ratio)*log2(1-ratio));
    }
}

inline std::vector<double> Entropy(const std::vector<std::array<uint64_t, 4>>& bpfrequencies) {
    std::vector<double> entropies;
    for (const auto bp_freq : bpfrequencies) {
        entropies.push_back(Entropy(bp_freq));
    }
    return entropies;
}
inline double Entropy(const std::array<uint64_t, 4>& base_freq, double* majority, double* total) {
    *majority = *std::max_element(base_freq.begin(), base_freq.end());

    *total = std::accumulate(base_freq.begin(), base_freq.end(), 0);
    if(*total == *majority) {
        return 0.0;
    } else {
        double ratio = *majority / *total;
        return -1 * (ratio * log2(ratio) + (1 - ratio) * log2(1 - ratio));
    }
}

inline uint64_t majorityFrequency(const std::array<uint64_t, 4>& base_freq) {
    return *std::max_element(base_freq.begin(), base_freq.end());
    
}
inline uint64_t majorityBP(const std::array<uint64_t, 4>& base_freq) {
    return std::max_element(base_freq.begin(), base_freq.end()) - base_freq.begin();
}

inline std::vector<uint64_t> center(const std::vector<std::array<uint64_t, 4>>& bpfrequencies) {
    std::vector<uint64_t> center;
    for(const auto& b : bpfrequencies) {
        center.push_back(majorityBP(b));
    }
    return center;
}
    
}   // namespace barcodeSpace
#endif // SPLIT_UTIL_H

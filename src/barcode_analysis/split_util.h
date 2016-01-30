#ifndef SPLIT_UTIL_H
#define SPLIT_UTIL_H
#include <algorithm>
#include <array>
#include <numeric>
inline double Entropy(const std::array<int, 4>& base_freq) {
    double majority = *std::max_element(base_freq.begin(), base_freq.end());

    double sum = std::accumulate(base_freq.begin(), base_freq.end(), 0);
    if(sum == majority) {
        return 0.0;
    } else {
        double ratio = majority / sum;
        return -1 * (ratio * log2(ratio) + (1 - ratio)*log2(1-ratio));
    }
}

inline std::vector<double> Entropy(const std::vector<std::array<int, 4>>& bpfrequencies) {
    std::vector<double> entropies;
    for (const auto bp_freq : bpfrequencies) {
        entropies.push_back(Entropy(bp_freq));
    }
    return entropies;
}
inline double Entropy(const std::array<int, 4>& base_freq, double* majority, double* total) {
    *majority = *std::max_element(base_freq.begin(), base_freq.end());

    *total = std::accumulate(base_freq.begin(), base_freq.end(), 0);
    if(*total == *majority) {
        return 0.0;
    } else {
        double ratio = *majority / *total;
        return -1 * (ratio * log2(ratio) + (1-ratio)*log2(1-ratio));
    }
}


#endif // SPLIT_UTIL_H

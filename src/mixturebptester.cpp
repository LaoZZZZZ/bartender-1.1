#include "mixturebptester.h"

#include <algorithm>
#include <array>
#include <numeric>
namespace barcodeSpace {

MixtureBPTester::MixtureBPTester(double p_value, double error_rate)
    : _p_value(p_value), _error_rate(error_rate)
{}
/**
  * TODO(lu): Finish this function. Try to use a hypothysis test instead of
  * the gating strategy.
*/
bool MixtureBPTester::IsPotentialMixture(const std::array<int, 4>& frequency, size_t bp) {
    int total = std::accumulate(frequency.begin(), frequency.end(), 0);
    return frequency[bp] * 1.0 / total > _error_rate;
    //return true;
}

}   // namespace barcodeSpace

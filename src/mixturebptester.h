#ifndef MIXTUREBPTESTER_H
#define MIXTUREBPTESTER_H

#include <array>
namespace barcodeSpace {
// This class is designed to test if the given frequency array
// and the position interested
class MixtureBPTester
{
public:
    MixtureBPTester(double p_value, double error_rate);
    bool IsPotentialMixture(const std::array<uint64_t, 4>& frequency, size_t bp);
private:
    bool    _p_value;
    double  _error_rate;
};
}   //  namespace barcodeSpace
#endif // MIXTUREBPTESTER_H

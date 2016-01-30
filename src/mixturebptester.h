#ifndef MIXTUREBPTESTER_H
#define MIXTUREBPTESTER_H

#include "proportiontest.h"
#include <array>
namespace barcodeSpace {
// This class is designed to test if the given frequency array
// and the position interested
class MixtureBPTester
{
public:
    MixtureBPTester(double p_value, double error_rate);
    bool IsPotentialMixture(const std::array<int, 4>& frequency, int bp);
private:
    bool    _p_value;
    double  _error_rate;
};
}   //  namespace barcodeSpace
#endif // MIXTUREBPTESTER_H

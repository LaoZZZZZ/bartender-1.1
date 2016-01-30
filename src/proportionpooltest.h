#ifndef PROPORTIONPOOLTEST_H
#define PROPORTIONPOOLTEST_H

#include "hypothesistester.h"

namespace barcodeSpace {
class ProportionPoolTest : public HypothesisTester
{
public:
    ProportionPoolTest(double zvalue);
    bool isSignificant(double e1, double e2, double n1, double n2);
private:
    double  _zvalue;
    double numerator;
    double denominator;
    double common_variance;
};
}   // namespace barcodeSpace
#endif // PROPORTIONPOOLTEST_H

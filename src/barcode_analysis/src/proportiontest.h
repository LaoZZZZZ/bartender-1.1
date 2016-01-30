#ifndef PROPORTIONTEST_H
#define PROPORTIONTEST_H

#include "hypothesistester.h"
namespace barcodeSpace {
class ProportionTest : public HypothesisTester
{
public:
    ProportionTest(double z_value);
    double ZValue()const {return z_value;}
    bool isSignificant(double e1, double e2, double n1, double n2);
    ~ProportionTest(){}
private:
    double  z_value;
    double numerator;
    double denominator;
    double common_variance;
};
}   // namespace barcodeSpace
#endif // PROPORTIONTEST_H


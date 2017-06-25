#include "proportiontest.h"

#include <cassert>
#include <math.h>
#include <stdlib.h>
namespace barcodeSpace {
ProportionTest::ProportionTest(double zvalue) : HypothesisTester(), z_value(zvalue), numerator(0),denominator(0), common_variance(0)
{
}
bool ProportionTest::isSignificant(double e1, double e2, double n1, double n2) {

    assert(n1 > 0 && n2 > 0);
    if (e1 == 0 && e2 == 0) {
        return false;
    }
    double p1 = e1/n1;
    double p2 = e2/n2;
    numerator = p1 - p2;
    denominator = sqrt(p1 * (1 - p1)/n1 + p2*(1-p2)/n2);
    return fabs(numerator / denominator) >= z_value;
}
}

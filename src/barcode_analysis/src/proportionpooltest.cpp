#include "proportionpooltest.h"

#include <cassert>
#include <math.h>
#include <stdlib.h>
#include <cmath>
namespace barcodeSpace {
ProportionPoolTest::ProportionPoolTest(double zvalue) : HypothesisTester(), _zvalue(zvalue), numerator(0),
    denominator(0), common_variance(0)
{
}
bool ProportionPoolTest::isSignificant(double e1, double e2, double n1, double n2) {

    assert(n1 > 0 && n2 > 0);
    if (e1 == 0 && e2 == 0) {
        return false;
    }

    numerator = e1/n1 - e2/n2;
    common_variance = (e1 + e2)/(n1 + n2);
    denominator = sqrt(common_variance * (1 - common_variance) * (1/n1 + 1/n2));
    return fabs(numerator / denominator) >= _zvalue;
    return true;
}
}   // namespace barcodeSpace

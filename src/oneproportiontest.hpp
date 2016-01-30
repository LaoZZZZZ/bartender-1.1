//
//  oneproportiontest.hpp
//  barcode_project
//
//  Created by luzhao on 1/13/16.
//  Copyright © 2016 luzhao. All rights reserved.
//

#ifndef oneproportiontest_hpp
#define oneproportiontest_hpp

#include "hypothesistester.h"

namespace barcodeSpace {
    class OneProportionTest : public HypothesisTester {
    public:
        OneProportionTest(double zvalue, double error_rate);
        bool isSignificant(double e1, double e2, double n1, double n2);
    private:
        double _zvalue;
        double _error_rate;
        double _p0_product;
        double _denominator;
        double _numerator;
    };
}   // namespace barcodeSpace
#endif /* oneproportiontest_hpp */

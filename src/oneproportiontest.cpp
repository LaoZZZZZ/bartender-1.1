//
//  oneproportiontest.cpp
//  barcode_project
//
//  Created by luzhao on 1/13/16.
//  Copyright © 2016 luzhao. All rights reserved.
//

#include "oneproportiontest.hpp"

#include <cassert>
#include <cmath>
#include <iostream>
namespace barcodeSpace {
    OneProportionTest::OneProportionTest(double zvalue, double error_rate) : HypothesisTester(),
    _zvalue(zvalue), _error_rate(error_rate),_numerator(0), _denominator(0){
        assert(_error_rate > 0);
        _p0_product = _error_rate * (1 - _error_rate);
    }
    bool OneProportionTest::isSignificant(double e1, double e2, double n1, double n2) {
        
        assert(n2 == 0 && e2 == 0 && n1 > 0);
        _numerator = fabs(e1/n1 - _error_rate);
        _denominator = sqrt(_p0_product / n1);
//        std::cout << _numerator/_denominator << std::endl;
        return _numerator/_denominator >= _zvalue;
    }
    
}   // namespace barcodeSpace

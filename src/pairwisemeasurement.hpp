//
//  pairwisemeasurement.hpp
//  barcode_project
//
//  Created by luzhao on 5/30/16.
//  Copyright © 2016 luzhao. All rights reserved.
//

#ifndef pairwisemeasurement_hpp
#define pairwisemeasurement_hpp

#include <stdio.h>

namespace barcodeSpace {
    struct PairwiseMeasurement {
        PairwiseMeasurement(int pos1, int pos2, double m) : position_1(pos1), position_2(pos2), measurement(m) {}
        
        int position_1;
        int position_2;
        double measurement;
        
    };

    // Put higher joint entropy pairs on the top.
    bool JointEntropyComparison(const PairwiseMeasurement& p1, const PairwiseMeasurement& p2);

    // Put lower mutual information pairs on the top.
    bool MutualInformationComparison(const PairwiseMeasurement& p1, const PairwiseMeasurement& p2);
}   // namespace barcodeSpace
#endif /* pairwisemeasurement_hpp */

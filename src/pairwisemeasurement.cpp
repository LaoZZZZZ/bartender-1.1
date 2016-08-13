//
//  pairwisemeasurement.cpp
//  barcode_project
//
//  Created by luzhao on 5/30/16.
//  Copyright © 2016 luzhao. All rights reserved.
//

#include "pairwisemeasurement.hpp"


namespace barcodeSpace {
    // Put higher joint entropy pairs on the top.
    bool JointEntropyComparison(const PairwiseMeasurement& p1, const PairwiseMeasurement& p2) {
        return p1.measurement < p2.measurement;
    }

    // Put lower mutual information pairs on the top.
    bool MutualInformationComparison(const PairwiseMeasurement& p1, const PairwiseMeasurement& p2) {
        return p1.measurement > p2.measurement;
    }
}



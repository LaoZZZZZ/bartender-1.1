//
//  errorestimator.hpp
//  barcode_project
//
//  Created by luzhao on 2/27/16.
//  Copyright © 2016 luzhao. All rights reserved.
//

#ifndef errorestimator_hpp
#define errorestimator_hpp

#include <array>
#include <vector>

namespace barcodeSpace {
    /**
     * This class is designed to estimate the sequence error 
     * from the spacers.
     */
    class ErrorEstimator {
    public:
        ErrorEstimator(int numb_base_pairs,
                       double entropy_threshold);
        double estimate(const std::vector<std::array<int,4>>& frequency_table,
                        const std::vector<double>& entropies);
    private:
        double _num_base_pairs;
        double _entropy_threshold;
    };
}   // namespace barcodeSpace
#endif /* errorestimator_hpp */

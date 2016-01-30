//
//  meanestimator.hpp
//  barcode_project
//
//  Created by luzhao on 1/16/16.
//  Copyright © 2016 luzhao. All rights reserved.
//

#ifndef meanestimator_hpp
#define meanestimator_hpp

#include "cluster.h"

#include <list>
#include <memory>
namespace barcodeSpace {
    
    class MeanEstimator {
    public:
        MeanEstimator(double trim_percentage);
        double mean(std::list<std::shared_ptr<cluster>>& clusters);
    private:
        double _trim_percentage;
    };
}   // namespace barcodeSpace

#endif /* meanestimator_hpp */

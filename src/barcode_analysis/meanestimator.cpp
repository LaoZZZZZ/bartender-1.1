//
//  meanestimator.cpp
//  barcode_project
//
//  Created by luzhao on 1/16/16.
//  Copyright © 2016 luzhao. All rights reserved.
//

#include "meanestimator.hpp"
#include "cluster.h"
#include "meansequentialestimator.h"

#include <algorithm>
#include <cassert>
#include <math.h>
#include <list>
#include <memory>
#include <stdio.h>

using namespace std;
namespace barcodeSpace {
    MeanEstimator::MeanEstimator(double trim_percentage) : _trim_percentage(trim_percentage){
        assert(_trim_percentage <= 1);
        assert(_trim_percentage >=0 );
    }
    
    double MeanEstimator::mean(std::list<std::shared_ptr<cluster> > &clusters) {
        
        clusters.sort([](const std::shared_ptr<cluster>& c1,
                         const std::shared_ptr<cluster>& c2) {return c1->size() < c2->size();});
        
        size_t left_trimed = floor(clusters.size() * _trim_percentage);
        size_t right_trimed = clusters.size() - floor(clusters.size() * _trim_percentage);
        
        size_t i = 0;
        MeanSequentialEstimator esimator;
        for (const auto& c : clusters) {
            if (i >= left_trimed && i <= right_trimed) {
                esimator.Add(c->size());
            }
            ++i;
        }
        return esimator.GetMean();
    }
    
}

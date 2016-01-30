//
//  clustermergeronesampletester.hpp
//  barcode_project
//
//  Created by luzhao on 1/13/16.
//  Copyright © 2016 luzhao. All rights reserved.
//

#ifndef clustermergeronesampletester_hpp
#define clustermergeronesampletester_hpp

#include "clustermergertester.h"
#include "hypothesistester.h"

#include <memory>
namespace barcodeSpace {
   

class ClusterMergerOneSampleTester : public ClusterMergerTester {
    
public:
    ClusterMergerOneSampleTester(double zvalue, double error);
    bool shouldMerge(const std::vector<std::array<int,4>>& freq_c1,
                     const std::vector<std::array<int,4>>& freq_c2);
private:
    std::unique_ptr<HypothesisTester> _tester;
};

}   // namespace barcodeSpace
#endif /* clustermergeronesampletester_hpp */

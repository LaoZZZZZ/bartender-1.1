//
//  seedselector.hpp
//  barcode_project
//
//  Created by luzhao on 12/13/15.
//  Copyright © 2015 luzhao. All rights reserved.
//

#ifndef seedselector_hpp
#define seedselector_hpp

#include "barcodepool.hpp"
#include "kmers_dictionary.h"
#include "typdefine.h"

#include <array>
#include <list>
#include <string>
#include <vector>

namespace barcodeSpace {
class SeedSelector {
public:
    // Given the total number of bp for seed
    // this class will pick up the first seed_length bps as the seed pattern
    // If the total number of qualified positions is less than the seed_length,
    // then all qualified position will be included.
    SeedSelector(size_t barcode_length);
    //void GenerateSeeds(const std::vector<std::pair<std::string, freq>>& barcodes);
    void addBarcode(const std::shared_ptr<BarcodePool>& barcode_pool) {
        CalculatePositionWeightMatrix(barcode_pool);
    }
    // In the result value. The first element is the start position. The second element
    // is the span of this part of consecutive seed part.
    std::vector<int> getSeedsPositions() {
        CalculateEntropy();
        SelectSeeds();
        return _seeds;
    }
    const std::vector<std::array<int,4>>& positionWeightMatrix() const {return _position_weight_matrix;}
private:
    void CalculatePositionWeightMatrix(const std::shared_ptr<BarcodePool>& barcode_pool);
    void CalculateEntropy();
    void SelectSeeds();
    
    size_t  _barcode_length;
    
    std::vector<std::array<int, 4>> _position_weight_matrix;
    std::vector<double> _entropy;
    std::vector<int>  _seeds;
    double  _entropy_threshold;
    std::shared_ptr<kmersDictionary>    _dict;
};
    
}
#endif /* seedselector_hpp */

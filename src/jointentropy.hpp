//
//  jointentropy.hpp
//  barcode_project
//
//  Created by luzhao on 4/27/16.
//  Copyright © 2016 luzhao. All rights reserved.
//

#ifndef jointentropy_hpp
#define jointentropy_hpp

#include <array>
namespace barcodeSpace {
    
    /**
     * This class is used to calcualte the joint entropy between two positions.
     * Assumptions and constraints:
     * 1. Each position only have four possible nucleotides.
     * 2. The joint count of each possible combination is given
     * 3. The entropy is log2 based.
     */
    class JointEntropy {
    public:
        typedef std::array<std::array<int,4>,4>  ConditionFrequencyTable;
        JointEntropy(int total_num_barcodes);
        double pairedEntropy(const ConditionFrequencyTable& cond_table);
        void reset(int total);
    private:
        // Total number reads
        int         _total;
        // The log2 scale for computation convenience.
        double      _log_total;

    };
    
}   // namespace barcodeSpace

#endif /* jointentropy_hpp */

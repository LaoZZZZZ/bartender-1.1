//
//  sequenceencoder.hpp
//  barcode_project
//
//  Created by luzhao on 12/13/15.
//  Copyright © 2015 luzhao. All rights reserved.
//

#ifndef sequenceencoder_hpp
#define sequenceencoder_hpp


#include "kmers_bitwisetransform.h"
#include "typdefine.h"

#include <cassert>
#include <string>

namespace barcodeSpace {
class SequenceEncoder {
public:
    SequenceEncoder() {
        _trans = kmersBitwiseTransform::getInstance();
        assert(_trans);
    }
    
    inline kmer encode(const std::string& seq) const {
        kmer result;
        _trans->seq_2_bitwise(seq, result, seq.length());
        return result;
    }

private:
    kmersBitwiseTransform*   _trans;
};
    
}   // namespace barcodeSpace
#endif /* sequenceencoder_hpp */

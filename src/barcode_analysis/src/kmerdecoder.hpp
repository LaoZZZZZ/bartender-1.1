//
//  kmerdecoder.hpp
//  barcode_project
//
//  Created by luzhao on 12/13/15.
//  Copyright © 2015 luzhao. All rights reserved.
//

#ifndef kmerdecoder_hpp
#define kmerdecoder_hpp

#include "kmers_bitwisetransform.h"

#include <algorithm>
#include <string>
#include <vector>

namespace barcodeSpace {
class KmerDecoder {
public:
    KmerDecoder(size_t k_len);
    
    // Gets the original DNA sequence
    inline std::string DNASequence(const kmer& k) {
        std::string result;
        _trans->bitwise_2_seq(k, result, _k_len);
        return result;
    }
    
    // Gets the encoded base pair value
    inline void bpSequence(const kmer& k, std::vector<size_t>& bps) {
        kmer t = k;
        for (size_t i = 0; i < _k_len; ++i) {
            bps.push_back(t & 3);
            t >>= 2;
        }
        std::reverse(bps.begin(), bps.end());
    }
private:
    kmersBitwiseTransform*  _trans;
    size_t  _k_len;
};
}   // namespace barcodeSpace
#endif /* kmerdecoder_hpp */

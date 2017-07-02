//
//  UmiExtractor.hpp
//  bartender
//
//  Created by luzhao on 6/29/17.
//  Copyright © 2017 luzhao. All rights reserved.
//

#ifndef UmiExtractor_hpp
#define UmiExtractor_hpp

#include "sequence.h"
#include <vector>
#include <string>

using std::vector;
using std::string;
namespace barcodeSpace {
    class UmiConfig {
    private:
        int umiPosition; // 0 based
        int umiLength;
    public:
        UmiConfig(const int umiPosition, const int umiLength) : umiPosition(umiPosition), umiLength(umiLength) {
        }
        
        int getUmiPosition() const {return umiPosition;}
        int getUmiLength() const {return umiLength;}
    };
    /*
     *this class extracts the umi(s) and concantenate them by the order of their positions.
     * For example, (umi1, 10), (umi2, 30), (umi3, 100) => umi1umi2umi3.
     */
    class UmiExtractor {
    public:
        UmiExtractor(const vector<UmiConfig> umiConfigs);
        // extract the umis and concantenate them into single string.
        const string extractUmi(const Sequence& read);
	const vector<UmiConfig> getUmiConfigs() const {return _umiConfigs;}
    private:
        vector<UmiConfig> _umiConfigs;
        size_t _totalCharacters;
    };
} // namespace barcodeSpace

#endif /* UmiExtractor_hpp */

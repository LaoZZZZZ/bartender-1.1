//
//  UmiExtractor.cpp
//  bartender
//
//  Created by luzhao on 6/29/17.
//  Copyright © 2017 luzhao. All rights reserved.
//

#include "UmiExtractor.hpp"
#include "sequence.h"
#include <vector>
#include <string>
#include <iostream>
#include <exception>
#include <algorithm>
#include <sstream> 

using std::vector;
using std::string;
using std::cout;
using std::endl;
using std::cerr;
using std::stringstream;

namespace barcodeSpace {
    struct UmiConfigCompartor {
        bool operator() (UmiConfig left,UmiConfig right) {
            return left.getUmiPosition() < right.getUmiPosition();}
    } UmiConfigCompartor;
    
    UmiExtractor::UmiExtractor(const vector<UmiConfig> umiConfigs): _umiConfigs(umiConfigs), _totalCharacters(0) {
        if(this->_umiConfigs.empty()) {
            stringstream ss;
            ss << "No umi parameter is provided" << endl;
            throw new std::runtime_error(ss.str());
        }
        std::sort(_umiConfigs.begin(), _umiConfigs.end(), UmiConfigCompartor);
        for (const auto& umiConfig : _umiConfigs) {
            if (umiConfig.getUmiLength() <= 0) {
                stringstream ss;
                ss << "the umi length " << umiConfig.getUmiLength() << " is not positive number" << endl;
                throw new std::runtime_error(ss.str());
            }
            
            if (umiConfig.getUmiPosition() < 0) {
                stringstream ss;
                ss << "the umi position " << umiConfig.getUmiPosition() << " is negative number" << endl;
                throw new std::runtime_error(ss.str());
            }
            _totalCharacters += umiConfig.getUmiLength();
        }
    }
    
    // 
    const string UmiExtractor::extractUmi(const Sequence& read) {
        if (read.length() < _totalCharacters) {
            stringstream ss;
            ss << "the read length " << read.length() << " is smaller than the specified umi length " << _totalCharacters << endl;
            throw new std::runtime_error(ss.str());
        }
        string result;
        for (const auto& umiConfig : _umiConfigs) {
            if (static_cast<size_t>(umiConfig.getUmiPosition()) >= read.length()) {
                stringstream ss;
                ss << "The umi position " << umiConfig.getUmiPosition() + 1 << " is larger than the read length " << read.length();
                throw new std::runtime_error(ss.str());
            }
            
            if (static_cast<size_t>(umiConfig.getUmiPosition() + umiConfig.getUmiLength()) > read.length()) {
                stringstream ss;
                ss << "The umi position " << umiConfig.getUmiPosition() + umiConfig.getUmiLength() << " is larger than the read length " << read.length();
                throw new std::runtime_error(ss.str());
            }
            result += read.fowardSeq().substr(umiConfig.getUmiPosition(), umiConfig.getUmiLength());
        }
        assert(result.length() == _totalCharacters);
        return result;
    }
} // namespace barcodeSpace

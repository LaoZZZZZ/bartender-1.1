//
//  barcodepool.cpp
//  barcode_project
//
//  Created by luzhao on 1/27/16.
//  Copyright © 2016 luzhao. All rights reserved.
//

#include "barcodepool.hpp"

#include <algorithm>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;
namespace barcodeSpace {
    std::shared_ptr<BarcodePool> BarcodePool::_instance = nullptr;
    void BarcodePool::createInstance(std::unordered_map<std::string, std::vector<std::string>>& raw_counts) {
        assert(_instance.get() == nullptr);
        _instance.reset(new BarcodePool(raw_counts));
    }
BarcodePool::BarcodePool(std::unordered_map<std::string, std::vector<std::string>>& raw_counts) {
    _num_replicates = 0;
    _total_raw_barcodes = 0;

    for (auto& b_freq : raw_counts) {
        _barcodes.push_back(b_freq.first);
        _total_raw_barcodes += b_freq.second.size();
        _primers.push_back(uniqueElement(b_freq.second));
        std::sort(b_freq.second.begin(), b_freq.second.end());
        _num_replicates += b_freq.second.size() - _primers.back().size();
    }
}
}
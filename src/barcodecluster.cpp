//
//  barcodecluster.cpp
//  barcode_project
//
//  Created by luzhao on 1/27/16.
//  Copyright © 2016 luzhao. All rights reserved.
//

#include "barcodecluster.hpp"
#include "kmers_dictionary.h"

#include <algorithm>
#include <numeric>
#include <memory>

using namespace std;
namespace barcodeSpace {
    std::shared_ptr<kmersDictionary> BarcodeCluster::_dict = kmersDictionary::getAutoInstance();
    std::shared_ptr<BarcodePool>     BarcodeCluster::_barcode_pool = BarcodePool::getAutoInstance();
    int BarcodeCluster::ClusterCount = 0;
    BarcodeCluster::BarcodeCluster(size_t barcode_index) {
        _center = _barcode_pool->barcode(barcode_index);
        _size = _barcode_pool->barcodeFrequency(barcode_index);
        for (const char& c : _center) {
            _bpFrequency.push_back(std::array<uint64_t,4>());
            _bpFrequency.back().fill(0);
            _bpFrequency.back()[_dict->asc2dna(c)] = static_cast<int>(_size);

        }
        _raw_barcodes.push_back(barcode_index);
        ClusterCount += 1;
        this->_cid = ClusterCount;
        _size_at_time.push_back(_size);
    }

    BarcodeCluster::BarcodeCluster(const std::string& center,
                                   const std::vector< std::array<uint64_t, 4> >& frequency_table,
                                   int cluster_id) :
        _center(center), _bpFrequency(frequency_table),_cid(cluster_id){
        assert(frequency_table.size() == center.length());
        _size = std::accumulate(frequency_table[0].begin(), frequency_table[0].end(), 0);
        _size_at_time.push_back(_size);
    }
}

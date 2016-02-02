//
//  pcrprocessor.cpp
//  barcode_project
//
//  Created by luzhao on 1/23/16.
//  Copyright © 2016 luzhao. All rights reserved.
//

#include "pcrprocessor.hpp"
#include "barcodecluster.hpp"

#include <array>
#include <list>
#include <memory>
#include <string>
#include <unordered_map>
using namespace std;
namespace barcodeSpace {
    PCRProcessor::PCRProcessor() : _replicates(0), _total_number_barcodes(0) {}

    void PCRProcessor::removePCR(std::shared_ptr<BarcodeCluster>& c,
                                 std::shared_ptr<BarcodePool>& pool) {
        unordered_map<string, list<size_t>> u_2_b;
        unordered_map<size_t, size_t> b_2_freq;
        std::shared_ptr<BarcodeCluster> updated_result;
        
        for (const auto& b : c->barcodes()) {
            for (const auto& u : pool->primers(b)) {
                if (u_2_b.find(u) == u_2_b.end()) {
                    u_2_b.insert({u,{b}});
                } else {
                    u_2_b[u].push_back(b);
                }
            }
            b_2_freq[b] = pool->barcodeFrequency(b);
        }
        
        // ONLY keep one barcode for each UMI
        for (auto& u_b : u_2_b) {
            size_t max = 0;
            size_t max_size = 0;
            // find the barcode that has the largest frequency.
            for (const auto& b : u_b.second) {
                if (pool->barcodeFrequency(b) > max_size) {
                    max = b;
                    max_size = pool->barcodeFrequency(b);
                }
            }
            // Remove all the barcode except the most frequent barcode for this UMI
            for (const auto& b : u_b.second) {
                if (b != max) {
                    assert(b_2_freq[b] > 0);
                    --b_2_freq[b];
                }
            }
            u_b.second.assign(1,max);
        }
        
        // Now get the updated barcode frequency and form a new cluster.
        for (const auto& b_f : b_2_freq) {
            if (b_f.second > 0) {
                vector<string> my_primes;
                for (const auto& p : pool->primers(b_f.first)) {
                    if (u_2_b[p].front() == b_f.first) {
                        my_primes.push_back(p);
                    }
                }
                std::swap(pool->primers(b_f.first), my_primes);
                std::shared_ptr<BarcodeCluster> ptemp(new BarcodeCluster(b_f.first));
                if (updated_result.get()) {
                    updated_result->merge(ptemp);
                } else {
                    updated_result = ptemp;
                }
            } else {
                pool->primers(b_f.first).clear();
            }
        }
        c = updated_result;
    }
}

//
//  pcrprocessor.hpp
//  barcode_project
//
//  Created by luzhao on 1/23/16.
//  Copyright © 2016 luzhao. All rights reserved.
//

#ifndef pcrprocessor_hpp
#define pcrprocessor_hpp

#include "barcodecluster.hpp"
#include "barcodepool.hpp"
#include "typdefine.h"

#include <list>
#include <memory>
#include <vector>
#include <unordered_map>

namespace barcodeSpace {
    class PCRProcessor {
    public:
        typedef BarcodeCluster Cluster;
        PCRProcessor();
        void process(std::list<std::shared_ptr<Cluster>>& clusters,
                     std::shared_ptr<BarcodePool>& barcode_pool) {
            for(auto& c : clusters) {
                removePCR(c, barcode_pool);
            }
        }
        size_t numberOfReplicates() const { return _replicates; }
        double PCREffect() const {
            if (_total_number_barcodes == 0) {
                return 0;
            }
            return static_cast<double>(_replicates)/ _total_number_barcodes;
        }
        void reset() {
            _replicates = _total_number_barcodes = 0;
        }
    private:
        void removePCR(std::shared_ptr<BarcodeCluster>&, std::shared_ptr<BarcodePool>&);
        size_t uniqueElement(const std::vector<std::string>& p) {
            if (p.empty()) {
                return 0U;
            }
            size_t cur = 0;
            size_t total = 1;

            for (size_t i = 1; i < p.size(); ++i) {
                if (p[i] != p[cur]) {
                    cur = i;
                    total += 1;
                }
            }

            return total;
        }
        size_t  _replicates;
        size_t  _total_number_barcodes;
    };
}

#endif /* pcrprocessor_hpp */

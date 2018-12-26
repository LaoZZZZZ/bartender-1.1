//
//  barcodetabledumper.hpp
//  barcode_project
//
//  Created by luzhao on 12/21/15.
//  Copyright © 2015 luzhao. All rights reserved.
//

#ifndef barcodetabledumper_hpp
#define barcodetabledumper_hpp

#include "barcodecluster.hpp"
#include "barcodepool.hpp"
#include "csvoutput.h"
#include "threadwrapper.hpp"

#include <list>
#include <string>
#include <vector>
namespace barcodeSpace {
    class BarcodeTableDumper : public ThreadWrapper {
    public:
        BarcodeTableDumper(const std::string& filename,
                           const std::list<std::shared_ptr<BarcodeCluster>>& clusters,
                           const std::shared_ptr<BarcodePool>& pool) : _out(filename.c_str()), _clusters(clusters), _pool(pool) {
            generateHeader();
            _out.Write(_cash);
        }
        ~BarcodeTableDumper() {
        }
    private:
        void generateHeader() {
            _cash.push_back("Unique.reads");
            _cash.push_back("Frequency");
            _cash.push_back("Cluster.ID");
        }
        void writeHeader();
        void run();

        CSVOutput<std::string> _out;
        const std::list<std::shared_ptr<BarcodeCluster>>& _clusters;
        const std::shared_ptr<BarcodePool>& _pool;
        std::vector<std::string> _cash;
    };
}   // namespace barcodeSpace
#endif /* barcodetabledumper_hpp */

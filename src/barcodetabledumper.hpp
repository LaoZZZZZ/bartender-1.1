//
//  barcodetabledumper.hpp
//  barcode_project
//
//  Created by luzhao on 12/21/15.
//  Copyright © 2015 luzhao. All rights reserved.
//

#ifndef barcodetabledumper_hpp
#define barcodetabledumper_hpp

#include "barcodepool.hpp"
#include "csvoutput.h"

#include <list>
#include <string>
#include <vector>
namespace barcodeSpace {
    class BarcodeTableDumper {
    public:
        BarcodeTableDumper(const std::string& filename) : _out(filename.c_str()) {
            generateHeader();
            _out.Write(_cash);
        }
        void writeBarcodeLine(int cluster_id, const std::vector<size_t>& barcodes,
                              const std::shared_ptr<BarcodePool>& pool);
        ~BarcodeTableDumper() {
        }
    private:
        void generateHeader() {
            _cash.push_back("Unique.reads");
            _cash.push_back("Frequency");
            _cash.push_back("Cluster.ID");
        }
        void writeHeader();

        CSVOutput<std::string> _out;
        std::vector<std::string> _cash;
    };
}   // namespace barcodeSpace
#endif /* barcodetabledumper_hpp */

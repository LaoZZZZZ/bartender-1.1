//
//  qualitytabledumper.hpp
//  barcode_project
//
//  Created by luzhao on 12/21/15.
//  Copyright © 2015 luzhao. All rights reserved.
//

#ifndef qualitytabledumper_hpp
#define qualitytabledumper_hpp

#include "barcodecluster.hpp"
#include "csvoutput.h"
#include "kmers_dictionary.h"
#include "threadwrapper.hpp"

#include <array>
#include <list>
#include <memory>
#include <vector>

namespace barcodeSpace {
    struct QualityTableElement {
        int cluster_id;
        std::string bp;
        std::string bp_freq;
        
        std::vector<std::string>    toStream() const {
            std::vector<std::string> line;
            line.push_back(std::to_string(cluster_id));
            line.push_back(bp);
            line.push_back(std::move(bp_freq));
            return line;
        }
    };
    // This class could dump position weight matrix to file.
    class QualityTableDumper : public ThreadWrapper {
    public:
        QualityTableDumper(const std::string& filename, size_t max_length_barcode,
                           const std::list<std::shared_ptr<BarcodeCluster>>& clusters) :
        _out(filename), _max_barcode_length(max_length_barcode), _clusters(clusters) {
            _dict = kmersDictionary::getAutoInstance();
            generateHeader();
            _out.Write(_cash);
            
        }
        void WritePWM(int cluster_id, const std::vector<std::array<uint64_t, 4>>& pwm);
        ~QualityTableDumper() {
        }
    private:
        void generateHeader();
        void run();
        
        CSVOutput<std::string> _out;
        std::vector<std::string>    _cash;
        std::shared_ptr<kmersDictionary> _dict;
        size_t _max_barcode_length;
        const std::list<std::shared_ptr<BarcodeCluster>>& _clusters;
    };
}   // namespace barcodeSpace

#endif /* qualitytabledumper_hpp */

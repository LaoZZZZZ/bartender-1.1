//
//  clustertabledumper.hpp
//  barcode_project
//
//  Created by luzhao on 12/21/15.
//  Copyright © 2015 luzhao. All rights reserved.
//

#ifndef clustertabledumper_hpp
#define clustertabledumper_hpp

//#include "csvoutput.h"
#include "cluster.h"
#include "kmerdecoder.hpp"
#include "csvoutput.h"
#include "typdefine.h"

#include <array>
#include <list>
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace barcodeSpace {
    
    struct ClusterTableElement {
        
        int cluster_id;
        double cluster_score;
        std::string center;
        std::vector<freq>   time_points;
        std::vector<std::string> toStream() const{
            std::vector<std::string> stream;
            stream.push_back(std::to_string(cluster_id));
            stream.push_back(center);
            stream.push_back(std::to_string(cluster_score));
            for (const auto& f : time_points) {
                stream.push_back(std::to_string(f));
            }
            return stream;
        }
    };
    
    class ClusterTableDumper{
    public:
        ClusterTableDumper(const std::string& filename, size_t num_time_points): _out(filename)
        {
            generateHeader(num_time_points);
            _out.Write(_cash);
        }
        
        void WriteCluster(const std::shared_ptr<cluster>& c) {
            _cash[0] = std::to_string(c->ClusterID());
            _cash[1] = decodeKmer(c->center()->center(), c->center()->klen());
            //_cash[1] = to_string(c->center()->center());
            _cash[2] = std::to_string(maxEntropy(c->bpFrequency()));
            size_t pos = 3;
            assert(_cash.size() - pos == c->columns().size());
            for (const auto& f : c->columns()) {
                _cash[pos] = std::to_string(f);
                ++pos;
            }
            _out.Write(_cash);
        }
        ~ClusterTableDumper() {
        }
    private:
        // Generates the header correspondingly upon the first cluster.
        void generateHeader(size_t num_time_points);
        std::string decodeKmer(const kmer& k, size_t klen) {
            if (kmer_decoders.find(klen) == kmer_decoders.end()) {
                kmer_decoders[klen].reset(new KmerDecoder(klen));
            }
            return kmer_decoders[klen]->DNASequence(k);
        }
        double maxEntropy(const std::vector<std::array<int, 4>>& frequency_table);

        
        std::vector<std::string>    _cash;
        std::unordered_map<size_t, std::shared_ptr<KmerDecoder>> kmer_decoders;
        CSVOutput<std::string>  _out;
    };
}   // namespace barcodeSpace

#endif /* clustertabledumper_hpp */

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
#include "barcodecluster.hpp"
#include "kmerdecoder.hpp"
#include "csvoutput.h"
#include "threadwrapper.hpp"
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
    
    class ClusterTableDumper : public ThreadWrapper {
    public:
        typedef BarcodeCluster Cluster;
        ClusterTableDumper(const std::string& filename,
                           size_t num_time_points,
                           const std::list<std::shared_ptr<Cluster>>& clusters) : _out(filename), _clusters(clusters)
        {
            generateHeader(num_time_points);
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
        double maxEntropy(const std::vector<std::array<uint64_t, 4>>& frequency_table);
        void run();
        
        std::vector<std::string>    _cash;
        std::unordered_map<size_t, std::shared_ptr<KmerDecoder>> kmer_decoders;
        CSVOutput<std::string>  _out;
        const std::list<std::shared_ptr<Cluster>>& _clusters;
    };
}   // namespace barcodeSpace

#endif /* clustertabledumper_hpp */

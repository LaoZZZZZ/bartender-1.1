//
//  clustersbucketer.hpp
//  barcode_project
//
//  Created by luzhao on 1/27/16.
//  Copyright © 2016 luzhao. All rights reserved.
//

#ifndef clustersbucketer_hpp
#define clustersbucketer_hpp

#include "barcodecluster.hpp"
#include "barcodeindexer.hpp"

#include <list>
#include <memory>
#include <vector>

namespace barcodeSpace {
    class ClusterBucketer {
        
    typedef std::list< std::shared_ptr<BarcodeCluster> >  CBin;
    public:
        ClusterBucketer(const std::vector<int>& seed_pos,
                        size_t seed_len,
			size_t step);

        bool done() const { return _cur == _indexers.size();}
        void shatter(const std::list<std::shared_ptr<BarcodeCluster>>& clusters);
        const std::vector<CBin>& Bins() const {return _bins;}
        size_t round() const {return _indexers.size();}
    private:
        std::vector<int> _seed_positions;
        size_t _seed_len;
        size_t _step; 
        void splitSeedPositions();
        size_t _cur;
        std::vector<std::unique_ptr<BarcodeIndexer>> _indexers;
        std::vector<CBin> _bins;
    };
}   // barcodeSpace

#endif /* clustersbucketer_hpp */

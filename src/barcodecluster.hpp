//
//  barcodecluster.hpp
//  barcode_project
//
//  Created by luzhao on 1/27/16.
//  Copyright © 2016 luzhao. All rights reserved.
//

#ifndef barcodecluster_hpp
#define barcodecluster_hpp

#include "barcodepool.hpp"
#include "kmers_dictionary.h"
#include "typdefine.h"


#include <algorithm>
#include <array>
#include <list>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

namespace barcodeSpace {
    class BarcodeCluster {
    public:
        // used for single time point.
        BarcodeCluster(size_t barcode_index);
        BarcodeCluster(const std::string& center, size_t freq);
        // used for multiple time points
        BarcodeCluster(const std::string& center,
                       const std::vector< std::array<int, 4> >& frequency_table,
                       int cluster_id);
        static int MaximumID() {return ClusterCount;}
        static void setBarcodePool(const std::shared_ptr<BarcodePool>& pool) {
            _barcode_pool = pool;
        }
        size_t barcodeLength() const {return _center.length();}
        const std::vector< std::array<int, 4> >& bpFrequency() const {
            return _bpFrequency;
        }
        
        const std::string& center() const { return _center;}
        size_t size() const {return _size;}
        int ClusterID() const {return _cid;}
        // Used to merge two clusters from same time point.
        void merge(const std::shared_ptr<BarcodeCluster>& c) {
            _size += c->size();
            AddFrequency(c->bpFrequency());
            updateCenter();
            _raw_barcodes.insert(_raw_barcodes.end(), c->barcodes().begin(), c->barcodes().end());
            auto iter = _size_at_time.begin();
            for (const auto& s : c->columns()) {
                *iter += s;
                ++iter;
            }
        }
        // used to combine two clusters from two time points.
        void combine(const std::shared_ptr<BarcodeCluster>& c) {
            merge(c);
            _time_points.push_front(c);
            _size_at_time.insert(_size_at_time.begin(),c->size());
        }
        const std::vector<size_t>& barcodes() const  {return _raw_barcodes;}
        const std::vector<size_t>& columns() const {return _size_at_time;}
        void SetClusterID(int id) {_cid = id;}
        void SetTimePointFrequency(const std::vector<size_t>& size_time_point) {
            _size_at_time = size_time_point;
        }
    private:
        //void updateFrequency(const kmers_freq&);
        void AddFrequency(const std::vector< std::array<int, 4> >& extra) {
            assert(extra.size() == _bpFrequency.size());
            for (size_t pos = 0; pos < extra.size(); ++pos) {
                for (size_t i = 0; i < 4; ++i) {
                    _bpFrequency[pos][i] += extra[pos][i];
                }
            }
        }
        
        // Updates the center after merging with other cluster.
        void updateCenter() {
            for (size_t pos = 0; pos < _bpFrequency.size(); ++pos) {
                long int bp = std::max_element(_bpFrequency[pos].begin(), _bpFrequency[pos].end()) - _bpFrequency[pos].begin();
                _center[pos] = _dict->dna2asc(bp);
            }
        }

        
        std::string                                       _center;
        std::vector<size_t>                               _raw_barcodes;
        std::vector<size_t>                               _size_at_time;
        std::list< std::shared_ptr<BarcodeCluster> >      _time_points;
        size_t                                            _size;
        // The base pair frequency at each position.
        // There are four elements in this vector
        // 0 : A, 1: C, 2: G, 3: T
        std::vector< std::array<int, 4> >                 _bpFrequency;
        int                                               _cid;
        
        static int                                        ClusterCount;
        static std::shared_ptr<kmersDictionary>           _dict;
        static std::shared_ptr<BarcodePool>               _barcode_pool;
    };
}   // namespace barcodeSpace

#endif /* barcodecluster_hpp */

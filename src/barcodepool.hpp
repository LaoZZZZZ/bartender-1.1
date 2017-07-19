//
//  barcodepool.hpp
//  barcode_project
//
//  Created by luzhao on 1/27/16.
//  Copyright © 2016 luzhao. All rights reserved.
//

#ifndef barcodepool_hpp
#define barcodepool_hpp


#include <cassert>
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace barcodeSpace {
    // Holds all original unique barcode
    // as well as its frequency and associated primers
    // This is a global pool, so only one instance is kept all the time.
    class BarcodePool {
    public:
        // Must be called at the very beginning of the program.
        static void createInstance(std::unordered_map<std::string, std::vector<std::string> >& raw_counts);
        const std::string& barcode(size_t index) const{
            assert(index < _barcodes.size());
            return _barcodes[index];
        }
        const std::vector<std::string>& primers(size_t index) const {
            assert(index < _primers.size());
            return _primers[index];
        }
        
        std::vector<std::string>& primers(size_t index) {
            assert(index < _primers.size());
            return _primers[index];
        }
        size_t barcodeFrequency(size_t index) const {
            assert(index < _primers.size());
            return _primers[index].size();
        }
        size_t NumOfRawBarcode() const {return _total_raw_barcodes;}
        size_t NumOfReplicates() const {return _num_replicates;}
        
        static std::shared_ptr<BarcodePool> getAutoInstance() {
            return _instance;
        }
        size_t size() const {return _barcodes.size();}
        
        // if the barcode does not exist, return -1;
        // This is added for testing purpose.
        size_t barcodeIndex(const std::string& barcode) {
            if (_barcodeIndex.find(barcode) == _barcodeIndex.end()) {
                return -1;
            } else {
                return _barcodeIndex[barcode];
            }
        }
    private:
        // The input vecter must be sorted.
        std::vector<std::string> uniqueElement(const std::vector<std::string>& p) {
            std::vector<std::string> result;

            if (!p.empty()) {
                size_t cur = 0;
                result.push_back(p[0]);
                
                for (size_t i = 1; i < p.size(); ++i) {
                    if (p[i] != p[cur]) {
                        cur = i;
                        result.push_back(p[i]);
                    }
                }
            }
            return result;
        }
        BarcodePool(std::unordered_map<std::string, std::vector<std::string> >& raw_counts);
        BarcodePool(const BarcodePool& cp);
        BarcodePool& operator=(const BarcodePool& cp);
        
        std::vector<std::string> _barcodes;
        std::vector<std::vector<std::string> > _primers;
        size_t _total_raw_barcodes;
        size_t _num_replicates;
        static std::shared_ptr<BarcodePool> _instance;
        std::unordered_map<std::string, size_t> _barcodeIndex;
    };
}   // namespace barcodeSpace

#endif /* barcodepool_hpp */

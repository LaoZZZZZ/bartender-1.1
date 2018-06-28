//
//  barcodepoolstatistics.hpp
//  barcode_project
//
//  Created by luzhao on 2/27/16.
//  Copyright © 2016 luzhao. All rights reserved.
//

#ifndef barcodepoolstatistics_hpp
#define barcodepoolstatistics_hpp

#include "barcodepool.hpp"
#include "kmers_dictionary.h"

#include <array>
#include <cassert>
#include <memory>
#include <unordered_map>
#include <vector>
namespace barcodeSpace {
    
    class BarcodeStatistics {
        typedef std::vector<std::array<uint64_t,4>>  PositionWeightMatrix;
        typedef std::unordered_map<int, PositionWeightMatrix> pwmDictionary;
        typedef std::unordered_map<int, std::vector<double>> entropyDictionary;
    public:
        BarcodeStatistics(const std::shared_ptr<BarcodePool>& pool);
        
        /*
         * Return the position weight matrix of all barcodes with the specified length.
         * The frequency table takes into acount the barcode frequency
         */
        const PositionWeightMatrix& FullFrequencyTable(int barcode_length)const {
            assert(_full_pwms.count(barcode_length));
            return _full_pwms.at(barcode_length);
        }
        /*
         * The corresponding entropy array of the full position weight matrix.
         **/
        const std::vector<double>& FullEntropy(int barcode_length)const {
            assert(_full_entropies.count(barcode_length));
            return _full_entropies.at(barcode_length);
        }
        
        
        /*
         * Return the position weight matrix of all barcodes with the specified length.
         * The frequency table takes into acount the barcode frequency
         */
        const std::vector<std::array<uint64_t, 4>>& UniqueFrequencyTable(int barcode_length)const {
            assert(_unique_pwms.count(barcode_length));
            return _unique_pwms.at(barcode_length);
        }
        
        /*
         * The corresponding entropy array of the full position weight matrix.
         **/
        const std::vector<double>& UniqueEntropy(int barcode_length)const {
            assert(_unique_entropies.count(barcode_length));
            return _unique_entropies.at(barcode_length);
        }
        
        double errorRate(int barcode_length) const {
            assert(_error_rate.count(barcode_length));
            return _error_rate.at(barcode_length -1);
        }
        
        const pwmDictionary& AllFullFrequencyTables() const {return _full_pwms;}
    private:
        // Calculates the position weight matrix for all groups of barcodes
        // Each group will have an in dependent pwm.
        
        void calculatePWM();
        // Calculates the entropy for all group of barcodes with different length;
        // Each group will have independent entropy list
        void calculateEntropy();
        
        void calculateError();
        
        std::shared_ptr<BarcodePool>    _barcode_pool;
        std::shared_ptr<kmersDictionary> _dict;
        
        pwmDictionary                   _full_pwms;
        pwmDictionary                   _unique_pwms;
        
        entropyDictionary               _full_entropies;
        entropyDictionary               _unique_entropies;
        std::unordered_map<int,double>             _error_rate;
        
    };
}   // namespace barcodeSpace

#endif /* barcodepoolstatistics_hpp */

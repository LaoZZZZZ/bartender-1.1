//
//  barcodeeditor.hpp
//  barcode_project
//
//  Created by luzhao on 12/17/15.
//  Copyright © 2015 luzhao. All rights reserved.
//

#ifndef barcodeeditor_hpp
#define barcodeeditor_hpp


#include "kmers_dictionary.h"
#include "kmer_freq.h"
#include "rawdatainfo.hpp"
#include "seedselector.hpp"
#include "typdefine.h"

#include <list>
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace barcodeSpace {
class BarcodeEditor {
public:
    // A table that keeps track the relationship between the edited barcode and the original barcode loaded from the raw file.
    // This table is used when saving the clusters so that we could know which cluster each barcode
    // belongs to.
    typedef std::unordered_map<kmer, std::list<RawDataInfo>> BarcodeSequenceTable;
    BarcodeEditor(size_t seed_length) : _seed_length(seed_length) {
        _dict = kmersDictionary::getAutoInstance();
        assert(_dict.get());
    }
    void editBarcodes(const std::vector<std::pair<std::string, freq>>& barcode_list);
    const std::vector<barcodeFreqTable>& barcodeTable() const {return _barcode_tables;}
    const std::vector<BarcodeSequenceTable>& barcode2Sequence() const {return _barcode_2_sequence;}
    ~BarcodeEditor() {
    }
private:
    void reset(size_t seed_length) {
        _seed_length = seed_length;
        _barcode_tables.assign(_seed_length + 1, {});
        _barcode_2_sequence.assign(_seed_length + 1, {});
    }
    // Given a seed poitions list and a barcode.
    // Return a kmers_freq instance that contains the edited barcode and its frequency.
    std::string extractSeed(const std::vector<std::pair<int, int>>&, const std::string&);
    size_t  _seed_length;
    std::vector<BarcodeSequenceTable> _barcode_2_sequence;
    // Each position refer to those edited seeds that have the length equal to the position
    std::vector<barcodeFreqTable>    _barcode_tables;
    std::shared_ptr<kmersDictionary> _dict;
};
}   // namespace barcodeSpace
#endif /* barcodeeditor_hpp */

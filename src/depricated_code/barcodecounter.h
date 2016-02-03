//
//  barcodecounter.h
//  barcode_project
//
//  Created by luzhao on 12/25/15.
//  Copyright © 2015 luzhao. All rights reserved.
//

#ifndef BARCODECOUNTING_H
#define BARCODECOUNTING_H
#include <string>
#include <memory>
#include "sequence.h"
#include "typdefine.h"
#include "kmers_bitwisetransform.h"
namespace barcodeSpace{

/**
  * Counts the frequency of unique barcode.
  * All inserted barcode are assume to have
  * the same length.
  *
*/
class BarcodeCounter
{
public:
  BarcodeCounter(size_t klen);

  // Inserts a raw sequence to the table.
  void insert(const std::string& barcode) {
      kmer k;
      this->_trans->seq_2_bitwise(barcode, k, this->_klen);
      this->insert(k);
  }

  // Inserts a coded sequence to the table.
  void insert(const kmer& k) {
      if(this->_barcode_db.end() != this->_barcode_db.find(k)){
          this->_barcode_db[k] += 1;
      }else {
          this->_barcode_db[k] = 1;
      }
  }
  size_t size(void) const {return this->_barcode_db.size();}
  freq& operator[](const kmer& key){return _barcode_db[key];}
  freq& Get(const kmer &key) { return _barcode_db[key];}
  const barcodeFreqTable &BarcodeTable() const { return this->_barcode_db;}
  size_t BarcodeLength()const {return _klen;}

private:
  barcodeFreqTable                           _barcode_db;
  size_t                                     _klen;
  kmersBitwiseTransform*                     _trans;
};
}

#endif // BARCODECOUNTING_H

#ifndef BARCODECOUNTING_H
#define BARCODECOUNTING_H
#include "kmers_hashset.h"
#include "sequence.h"
#include "kmers_bitwisetransform.h"
namespace prefixMatching{

/**
  * count the frequency of unique barcode extracted from the file
  * Currently it use the dense
*/
class barcodeCounting
{
 public:
  barcodeCounting(size_t klen);
  void insert(const BTDnaString& seq);
  size_t size(void) const {return this->_barcode_db.size();}
  freq& operator[](const kmer& key){return _barcode_db[key];}
  freq& get(const kmer &key) { return _barcode_db[key];}
  const BarcodeTable &barcodes() const { return this->_barcode_db;}
 private:
  /**
    * \brief transform a sequence to bitwise representation
  */
  inline kmer bitwise(const BTDnaString& seq){
      if(seq.length() != this->_klen)
          throw runtime_error("The sequence length does not equal to the specified kmer length!");
      kmer k(0);
      this->_transformer->seq_2_bitwise(seq,k,this->_klen);
      return k;
  }
 private:
  BarcodeTable                           _barcode_db;
  kmers_BitwiseTransform*                _transformer;
  size_t                                 _klen;
};
}

#endif // BARCODECOUNTING_H

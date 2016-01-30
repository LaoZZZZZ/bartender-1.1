#ifndef KMERS_BITWISETRANSFORM_H
#define KMERS_BITWISETRANSFORM_H



#include "assert_helpers.h"
#include "typdefine.h"
#include "kmers_dictionary.h"


#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
namespace barcodeSpace{
/**
  * given a sequence, generate its hashcode
  * given a hashcode, recover it to a sequence
  *

*/
class kmersBitwiseTransform
{
public:
    static kmersBitwiseTransform* getInstance();
    static std::shared_ptr<kmersBitwiseTransform> getAutoInstance();
    /**
      * @param[in]: seq         reads sequence either forward or backward
      * @param[out]: hashcode   generated hashcode from the sequence
      * @param[in]: kmersLen    the length of each kmer

      */
    inline void seq_2_bitwise(const std::string& seq, kmer& hashcode,const size_t& kmersLen){
        if(seq.empty()) {
            std::cerr << "The sequence is empty when encoding it" << std::endl;
            hashcode = ERRORFLAG;
        } else if (seq.length() < kmersLen) {
	   std::cerr << "The sequence length is less than the specified kmerslen" << std::endl;
           hashcode = ERRORFLAG;
	}
        else{
            hashcode = 0;
            for(UINT8 i = 0; i < kmersLen; i++){
                hashcode |= ((kmer)(this->_pDict->asc2dna(seq[i])) << ((kmersLen - i - 1)*2));
            }
        }
    }
    /**
      * @param[in]: seq         reads sequence either forward or backward
      * @param[out]: hashcode   generated hashcode from the sequence
      * @param[in]: kmersLen    the length of each kmer

      */
    inline void seq_rv_2_bitwise(const std::string& seq, kmer& hashcode,const size_t& kmersLen){
        if(seq.empty() || (seq.length() < kmersLen)) {
            std::cerr << "error" << std::endl;
            hashcode = ERRORFLAG;
        }
        else{
            hashcode = 0;
            for(UINT8 i = kmersLen -1 ; i > 0; i--){
                hashcode |= ((kmer)(this->_pDict->asc2dna(seq[kmersLen - i - 1]) ^ 0x3) << (i*2));
            }
        }
    }

    /**
      * @param[out]: seq         sequence recovered from the hashcode
      * @param[in]: hashcode     generated hashcode from the sequence
      * @param[in]: kmersLen     the length of each kmer

      */
    inline void bitwise_2_seq(const kmer& hashcode,std::string& seq,size_t kmersLen){
        assert(kmersLen > 0);
        for(UINT8 i = 0; i < kmersLen; i++ ){
            seq += unpack_2b_from_64b(hashcode, kmersLen - i -1);
        }
    }

    /**
        * @param[in]: seq            input sequence
        * @param[out]: hashcode      generated hashcode from the sequence
        * @param[in]: pos            the start position of the subsequence to be transferd
        * @param[in]: kmersLen       the length of kmers specified
        *
    */
    inline void seq_2_bitwise(const std::string& seq,kmer& hashcode,unsigned short pos,const size_t& kmersLen){
        if(seq.empty() || (seq.length() - pos < kmersLen)){
            hashcode = ERRORFLAG;
        }
        else{
            hashcode = 0U;
            for(size_t i = 0; i <  kmersLen; i++){
                hashcode |= ((UINT64)(this->_pDict->asc2dna(seq[pos+i])) << ((kmersLen - i - 1U)*2U));
               // pack_2b_in_64b(seq[pos+i],hashcode,kmersLen - i - 1);
            }
        }

    }

    /**
      *
    */
    inline void reverseComp(kmer& in,kmer& out,const int& kmerLen){

        assert_gt(kmerLen,0);
        out = 0x0;
        UINT8 i = 0;
        out |= ( (in & 0x3) ^ 0x3);
        for(i = 1; i < kmerLen; i++){
            out = out<<2;
            out |= ((((kmer)in >> i*2)& 0x3) ^ 0x3);

        }


    }
    /**
      * extract the value start from the low end upto off position
      * put the number into value [off,0]
      */
    inline void extract(kmer& hashcode,UINT8 off,kmer& value){
        assert_lt(off,64);
        value = (0x1 << (off)) - 1;
        value &= hashcode;
    }
    /**
      * extract the value start from the low end upto off position
      * put the number into value [high,low+1]
      */
    inline void extract(kmer& hashcode,kmer& value,UINT8 high,UINT8 low){
        assert_lt(high,64U);
        assert_lt(low,high)
        value = (1ULL << (high-low)) - 1U;
        value &= (hashcode>>low);
    }
    /**
      * put the value into hashcode in the specific postion start at off
      */
    inline void compress(kmer& hashcode,UINT8 off,kmer value){
        assert_lt(off,64U);
        kmer tmp = 1;
        assert_lt(value,tmp<<(off+1U));
        hashcode = (hashcode << off)|value;

    }

protected:
    kmersBitwiseTransform(){
        try{
            this->_pDict = kmersDictionary::getInstance();
            if(!_pDict) throw std::bad_alloc();

        }catch(const std::bad_alloc& e){
            std::cerr << e.what() << std::endl;
            std::cerr.flush();
            exit(-1);
        }

    }
    inline void pack_2b_in_64b(const kmer two, kmer& sixty4, const size_t off){
        assert_lt(two, 4);
#ifndef BIGDATA
        assert_lt(off, 32);
#else
        assert_lt(off,two.size());
#endif
        sixty4 |= (two << (off*2));
    }
    inline char unpack_2b_from_64b(const kmer& sixty4, const size_t off){
#ifndef BIGDATA
        assert_lt(off, 32);
        return _pDict->dna2asc((sixty4 >> (off*2)) & 0x3);
#else
        assert_lt(off, sixty4.size());
        return _pDict->dna2asc((sixty4 >> (off*2)) & 0x3).toInt());
#endif
    }
    inline void pack_2b_in_8b(const int two, UINT8& eight, const size_t off) {
        assert_lt(two, 4);
        assert_lt(off, 4);
        eight |= (two << (off*2));
    }

    inline char unpack_2b_from_8b(const UINT8 eight, const size_t off) {
        assert_lt(off, 4);
        return _pDict->dna2asc((eight >> (off*2)) & 0x3);
    }

    inline void pack_2b_in_32b(const UINT32 two, UINT32& thirty2, const size_t off) {
        assert_lt(two, 4);
        assert_lt(off, 16);
        thirty2 |= (two << (off*2));
    }

    inline char unpack_2b_from_32b(const UINT32 thirty2, const size_t off) {
        assert_lt(off, 16);
        return _pDict->dna2asc((thirty2 >> (off*2)) & 0x3);
    }
private:
    static std::shared_ptr<kmersBitwiseTransform>           _pIns;
    kmersDictionary*                        _pDict;
};
} //namespace
#endif // KMERS_BITWISETRANSFORM_H

#ifndef KMERS_BITWISETRANSFORM_H
#define KMERS_BITWISETRANSFORM_H


#include "suffix_commons.h"
#include "sstring.h"
#include "kmers_dictionary.h"
//#include "endian_swap.h"
namespace prefixMatching{
/**
  * given a sequence, generate its hashcode
  * given a hashcode, recover it to a sequence
  *

*/
class kmers_BitwiseTransform
{
public:
    static kmers_BitwiseTransform* getInstance();
    virtual string classname(){return std::string("kmers_BitwiseTransform");}

    /**
      * @param[in]: seq         reads sequence either forward or backward
      * @param[out]: hashcode   generated hashcode from the sequence
      * @param[in]: kmersLen    the length of each kmer

      */
    inline void seq_2_bitwise(const BTDnaString& seq, kmer& hashcode,const size_t& kmersLen){
        if(seq.empty() || (seq.length() < kmersLen)) {
            cerr<<"error"<<endl;
            hashcode = ERRORFLAG;
        }
        else{
            hashcode = 0;
            for(UINT8 i = 0; i < kmersLen; i++){
                hashcode |= ((kmer)(seq[i]) << ((kmersLen - i - 1)*2));
            }
            /*UINT64 rv = 0;
            this->reverseComp(hashcode,rv,kmersLen);
            if(rv < hashcode)
                hashcode = rv;*/
        }
    }
    /**
      * @param[in]: seq         reads sequence either forward or backward
      * @param[out]: hashcode   generated hashcode from the sequence
      * @param[in]: kmersLen    the length of each kmer

      */
    inline void seq_rv_2_bitwise(BTDnaString& seq, kmer& hashcode,const size_t& kmersLen){
        if(seq.empty() || (seq.length() < kmersLen)) {
            cerr<<"error"<<endl;
            hashcode = ERRORFLAG;
        }
        else{
            hashcode = 0;
            for(UINT8 i = kmersLen -1 ; i > 0; i--){
                hashcode |= ((kmer)(seq[kmersLen - i - 1] ^ 0x3) << (i*2));
            }
        }
    }

    /**
      * @param[out]: seq         sequence recovered from the hashcode
      * @param[in]: hashcode     generated hashcode from the sequence
      * @param[in]: kmersLen     the length of each kmer

      */
    inline void bitwise_2_seq(const kmer& hashcode,BTDnaString& seq,size_t kmersLen){
        seq.clear();
        for(UINT8 i = 0; i < kmersLen; i++ ){
            //seq.append(((hashcode >> ((kmersLen-i-1)*2)) & 0x3));
            seq.append(unpack_2b_from_64b(hashcode,kmersLen - i -1));
        }
    }
    /**
        * @param[in]: seq            input sequence
        * @param[out]: hashcode      generated hashcode from the sequence
        * @param[in]: pos            the start position of the subsequence to be transferd
        * @param[in]: kmersLen       the length of kmers specified
        *
    */
    inline void seq_2_bitwise(BTDnaString& seq,kmer& hashcode,unsigned short pos,const size_t& kmersLen){
        if(seq.empty() || (seq.length() - pos < kmersLen)){
            hashcode = ERRORFLAG;
        }
        else{
            hashcode = 0U;
            for(size_t i = 0; i <  kmersLen; i++){
                hashcode |= ((UINT64)(seq[pos+i]) << ((kmersLen - i - 1U)*2U));
               // pack_2b_in_64b(seq[pos+i],hashcode,kmersLen - i - 1);
            }
        }

    }
    /**
        * @param[in]: seq            input sequence
        * @param[out]: hashcode      generated hashcode from the sequence
        * @param[in]: pos            the start position of the subsequence to be transferd
        * @param[in]: kmersLen       the length of kmers specified
        *
    */
    inline void seq_rv_2_bitwise(BTDnaString& seq,kmer& hashcode,unsigned short pos,const int& kmersLen){
        if(seq.empty() || (seq.length() - pos < (size_t)kmersLen)){
            hashcode = ERRORFLAG;
        }
        else{
            hashcode = 0;
            //bug fixed at 01-15-2014 by luzhao
            for(UINT8 i = 1; i <= kmersLen ; i++){
                hashcode |= ((UINT64)(seq[pos + kmersLen - i] ^ 0x3) << ((kmersLen - i)*2));
            }
        }

    }
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
    kmers_BitwiseTransform(){
        try{
            this->_pDict = kmers_Dictionary::getInstance();
            if(!_pDict) throw bad_alloc();

        }catch(const bad_alloc& e){
            cerr<<e.what()<<endl;
            cerr.flush();
            exit(-1);
        }

    }
    inline void pack_2b_in_64b(const kmer two, kmer& sixty4, const int off){
        assert_lt(two, 4);
#ifndef BIGDATA
        assert_lt(off, 32);
#else
        assert_lt(off,two.size());
#endif
        sixty4 |= (two << (off*2));
    }
    inline int unpack_2b_from_64b(const kmer& sixty4, const int off){
#ifndef BIGDATA
        assert_lt(off, 32);
        return (int)((sixty4 >> (off*2)) & 0x3);
#else
        assert_lt(off, sixty4.size());
        return ((sixty4 >> (off*2)) & 0x3).toInt();
#endif
    }
    inline void pack_2b_in_8b(const int two, UINT8& eight, const int off) {
        assert_lt(two, 4);
        assert_lt(off, 4);
        eight |= (two << (off*2));
    }

    inline UINT8 unpack_2b_from_8b(const UINT8 eight, const int off) {
        assert_lt(off, 4);
        return ((eight >> (off*2)) & 0x3);
    }

    inline void pack_2b_in_32b(const UINT32 two, UINT32& thirty2, const int off) {
        assert_lt(two, 4);
        assert_lt(off, 16);
        thirty2 |= (two << (off*2));
    }

    inline UINT8 unpack_2b_from_32b(const UINT32 thirty2, const int off) {
        assert_lt(off, 16);
        return ((thirty2 >> (off*2)) & 0x3);
    }
private:
    static kmers_BitwiseTransform*          _pIns;
    kmers_Dictionary             *          _pDict;
};
} //namespace
#endif // KMERS_BITWISETRANSFORM_H

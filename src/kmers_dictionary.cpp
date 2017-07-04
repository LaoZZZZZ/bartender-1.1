#include "kmers_dictionary.h"
#include <cassert>
#include <memory.h>
#include <memory>
namespace barcodeSpace{
    std::shared_ptr<kmersDictionary> kmersDictionary::_pIns = std::shared_ptr<kmersDictionary>();
    kmersDictionary::kmersDictionary()
    {
         memset(_asc2dna, 20, 256);
         memset(_complement, 0, 256);
        _asc2dna[65] = 0; // A
         _asc2dna[67] = 1; // C
         _asc2dna[71] = 2; // G
         _asc2dna[78] = 0 ; // N
         _asc2dna[84] = 3; // T

        _asc2dna[97] = 0; // a
         _asc2dna[99] = 1; // c
         _asc2dna[103] = 2; // g
         _asc2dna[110] = 0; // n
         _asc2dna[116] = 3; // t

         _complement[65] = 84; // A=>T
         _complement[67] = 71; // C=>G
         _complement[71] = 67; // G=>C
         _complement[78] = 78; // N=>N
         _complement[84] = 65; // T=>A

         _complement[97] = 84;
         _complement[99] = 71;
         _complement[103] = 67;
         _complement[110] = 78; // n => N
         _complement[116] = 65;

         _dna2asc[0] = 'A';
         _dna2asc[1] = 'C';
         _dna2asc[2] = 'G';
         _dna2asc[3] = 'T';
         _dna2asc[4] = 'N';
    }
    kmersDictionary* kmersDictionary::getInstance(){
        if(!_pIns.get()){
            _pIns.reset(new kmersDictionary());
        }
        assert(_pIns.get());
        return _pIns.get();
    }
    std::shared_ptr<kmersDictionary> kmersDictionary::getAutoInstance(){
        if(!_pIns.get()){
            _pIns.reset(new kmersDictionary());
        }
        assert(_pIns.get());
        return _pIns;
    }
}

#include "kmers_dictionary.h"
#include <cassert>
#include <memory.h>
#include <memory>
namespace barcodeSpace{
    std::shared_ptr<kmersDictionary> kmersDictionary::_pIns = std::shared_ptr<kmersDictionary>();
    kmersDictionary::kmersDictionary()
    {
         memset(_asc2dna, 0, 256);
         memset(_complement, 0, 256);
         _asc2dna[67] = 1;
         _asc2dna[71] = 2;
         //_asc2dna[78] = 4 ;
         _asc2dna[84] = 3;

         _asc2dna[99] = 1;
         _asc2dna[103] = 2;
         //_asc2dna[110] = 4;
         _asc2dna[116] = 3;

         _complement[65] = 84;
         _complement[67] = 71;
         _complement[71] = 67;
         _complement[78] = 78;
         _complement[84] = 65;

         _complement[97] = 84;
         _complement[99] = 71;
         _complement[103] = 67;
         _complement[110] = 78;
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

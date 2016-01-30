#include "kmers_bitwisetransform.h"



namespace prefixMatching{
    kmers_BitwiseTransform* kmers_BitwiseTransform:: _pIns = NULL;
    kmers_BitwiseTransform* kmers_BitwiseTransform::getInstance()
    {
        if(!_pIns)
            _pIns = new kmers_BitwiseTransform();
        return _pIns;
    }
}

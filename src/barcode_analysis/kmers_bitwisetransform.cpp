#include "kmers_bitwisetransform.h"

#include <memory>
namespace barcodeSpace{
    std::shared_ptr<kmersBitwiseTransform> kmersBitwiseTransform:: _pIns = NULL;
    kmersBitwiseTransform* kmersBitwiseTransform::getInstance()
    {
        if(!_pIns.get())
            _pIns.reset(new kmersBitwiseTransform());
        return _pIns.get();
    }
    std::shared_ptr<kmersBitwiseTransform> kmersBitwiseTransform::getAutoInstance()
    {
        if(!_pIns.get())
            _pIns.reset(new kmersBitwiseTransform());
        return _pIns;
    }
}

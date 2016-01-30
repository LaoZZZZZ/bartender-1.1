#include "centerclustermapperiterator.h"

namespace barcodeSpace {
CenterClusterMapperIterator::CenterClusterMapperIterator(
        const std::shared_ptr<CenterClusterMapper>& linker) : _data(linker), _cur(0)
{
    this->init();
}
}   // namespace barcodeSpace

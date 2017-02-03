#include "centerclustermapper.h"

#include <list>
namespace barcodeSpace {
CenterClusterMapper::CenterClusterMapper(const int size)
    : _cluster_2_center(size, std::list<std::string>()), _cluster_2_instance(size, NULL)
{
}
}

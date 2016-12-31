#include "centerclustermapper.h"

#include <list>
namespace barcodeSpace {
CenterClusterMapper::CenterClusterMapper(int max_id)
    : _cluster_2_center(max_id + 1, std::list<std::string>()),
      _cluster_2_instance(max_id + 1, NULL)
{
}
}

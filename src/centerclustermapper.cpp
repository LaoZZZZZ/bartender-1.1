#include "centerclustermapper.h"

#include <list>
namespace barcodeSpace {
CenterClusterMapper::CenterClusterMapper(int max_id)
    : _cluster_2_center(max_id, std::list<std::string>()),
      _cluster_2_instance(max_id, NULL)
{
}
}

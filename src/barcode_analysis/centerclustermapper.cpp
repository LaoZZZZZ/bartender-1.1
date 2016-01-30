#include "centerclustermapper.h"

#include "cluster.h"
#include <list>
namespace barcodeSpace {
CenterClusterMapper::CenterClusterMapper(int max_id)
    : _cluster_2_center(max_id, std::list<kmer>()),
      _cluster_2_instance(max_id, NULL)
{
}
}

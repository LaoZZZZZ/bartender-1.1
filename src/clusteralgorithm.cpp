#include "barcodemutationgenerator.h"
#include "clusteralgorithm.h"
#include "clustermergerpooltester.h"
#include "clustermergerunpooltester.h"
#include "barcodemutationgenerator.h"

#include <iostream>
#include <algorithm>
#include <list>
#include <stdexcept>
#include <vector>
#include <unordered_map>

using std::list;
using std::runtime_error;
using std::vector;
using std::cout;
using std::endl;
namespace barcodeSpace{
clusterAlgorithm::clusterAlgorithm(size_t cutoff,size_t klen,
                                   size_t dist_thres):
                                   _cutoff(cutoff),_klen(klen),
                                   _dist_thres(dist_thres)
{
}
}   // namespace barcodeSpace

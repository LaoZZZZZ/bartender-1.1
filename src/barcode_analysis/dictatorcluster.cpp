#include "dictatorcluster.h"
#include <list>
#include "typdefine.h"
#include "kmer_freq.h"
#include "util.h"

#include <algorithm>
#include <array>
#include <memory>
#include <vector>
using std::list;
using std::vector;
using std::array;
using std::max_element;
namespace barcodeSpace{

dictatorCluster::dictatorCluster(const std::shared_ptr<DictatorCenter>& c,const std::vector<freq>& sz) : cluster(c, sz)
{
}
dictatorCluster::dictatorCluster(const std::shared_ptr<DictatorCenter>& center,
                                 const std::vector<freq>& time_points,
                                 const std::vector<std::array<int, 4>>& frequency,
                                 int id) :
    cluster(center, time_points, frequency, id) {}
void dictatorCluster::combine(const cluster& cl) {
    _pointSize.insert(_pointSize.end(), cl.columns().begin(), cl.columns().end());

    updateFrequency(cl);
    this->_barcodes.insert(this->_barcodes.end(),cl.barcodes().begin(),cl.barcodes().end());
    this->_size += cl.size();
    updateCenter();
}
/**
  * Combines two clusters into one cluster.
  * TODO(lu): What if the center of the passed cl exists in the current cluster?
  * How to measure the distance in this case such that the correct center is elected.
  * This function could only merge two clusters that belong to the same time point.
*/
void dictatorCluster::merge(const cluster& cl){
    assert(cl.columnSize() == this->columnSize());
    auto liter = cl.columns().begin();
    for(auto iter = this->_pointSize.begin();
        iter != this->_pointSize.end(); iter++){
        *iter += *liter;
        liter++;
    }

    updateFrequency(cl);
    this->_barcodes.insert(this->_barcodes.end(),cl.barcodes().begin(),cl.barcodes().end());
    this->_size += cl.size();
    updateCenter();
}

}

#ifndef DICTATORCLUSTER_H
#define DICTATORCLUSTER_H

#include "kmer_freq.h"
#include "dictatorcenter.h"
#include "cluster.h"

#include <array>
#include <list>
#include <memory>
#include <vector>

namespace barcodeSpace{
/**
  * For this strategy, one cluster only has one barcode as center
*/
class dictatorCluster : public cluster
{
public:
    dictatorCluster(const std::shared_ptr<DictatorCenter>&, const std::vector<freq>&);
    dictatorCluster(const std::shared_ptr<DictatorCenter>&, const std::vector<freq>&,
                    const std::vector<std::array<int, 4>>&, int cluster_id);
    /**
      * Combines two clusters into one cluster.
      * The cluster id will be set to be equal to the smaller cluster ID.
      *
    */
    void merge(const cluster&);
    void combine(const cluster&);
private:
    double calculateScore(const std::shared_ptr<DictatorCenter>& k);
};
}   // namespace barcodeSpace
#endif // DICTATORCLUSTER_H

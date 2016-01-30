#ifndef DICTATORCLUSTER_H
#define DICTATORCLUSTER_H
#include <list>
using std::list;
#include "kmer_freq.h"
#include "util.h"
#include "dictatorcenter.h"
#include "cluster.h"
namespace prefixMatching{
/**
  * For this strategy, one cluster only has one barcode as center
*/
class dictatorCluster : public cluster
{
public:
    dictatorCluster(std::shared_ptr<Center>,const std::list<freq>&);
    /**
      * combine two clusters into one cluster
    */
    virtual void merge(const cluster&);
    void addBarcode(const kmers_freq&,bool);
private:
    double calculateScore(const std::shared_ptr<Center>& k);
};
}
#endif // DICTATORCLUSTER_H

#ifndef CONSENSUSCLUSTER_H
#define CONSENSUSCLUSTER_H
#include "cluster.h"
#include "kmer_freq.h"
#include "dictatorcenter.h"
#include "kmers_bitwisetransform.h"
namespace prefixMatching{
class consensusCluster : public cluster
{
public:
    consensusCluster(const kmers_freq&,size_t klen,const std::list<freq>& pointSize);
    /**
      * combine two clusters into one cluster
    */
    virtual void merge(const cluster&);
    void addBarcode(const kmers_freq&,bool);
    const vector<vector<int>>&    bpFrequency()const{return this->_bpFrequency;}
private:
    std::vector<std::vector<int>>       _bpFrequency;
};
}
#endif // CONSENSUSCLUSTER_H

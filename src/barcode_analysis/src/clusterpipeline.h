#ifndef CLUSTERPIPELINE_H
#define CLUSTERPIPELINE_H
#include <math.h>
#include <list>
#include <memory>
#include <vector>
#include "kmer_freq.h"
#include "cluster.h"
#include "timer.h"
#include "typdefine.h"
#include "formats.h"
namespace barcodeSpace{
class clusterPipline
{
public:
    typedef std::list<kmers_freq>                Bin;
    typedef std::list<std::shared_ptr<cluster>>  CBin;
public:
    /**
      * pos: The start position of seed.
      * span: The length of seed
      * klen: The length of barcode
      * cutoff: frequency that differential "true" barcode and barcode with "error".
      * stopThres: This threshold
    */
    clusterPipline(size_t pos,size_t span,size_t klen,size_t cutoff = 1,
                   double zvalue = 3.08, bool pool = true, double stopThres = 0.01);
    bool clusterDrive(const barcodeFreqTable&);
    bool clusterDrive(const std::list<std::shared_ptr<cluster>>&);
    const std::list<std::shared_ptr<cluster>>& clusters(){return this->_clusters;}

private:
    /////////////////////////////////////////////////////////////////////
    //////////////////////Steps for clustering//////////////////////////
    ///////////////////////////////////////////////////////////////////

    /**
     * Transform the barcodes table into a list of clusters
    */
    void transform(const barcodeFreqTable& barcodes);

    /**
      * Shatters the barcodes into bins using the given region.
    */
    void shatter(const barcodeFreqTable& table);
    /**
      * Shatters the first round clusters into bins.
    */
    bool shatter(const list<std::shared_ptr<cluster>>&);

    /**
      * First round clustering: starts from individual barcodes
    */
    void ErrorIntroduceStage();
    /**
      * Second round clustering on top of the result from the first round clustering
      *
    */
    void crossBinClustering(size_t);
private:
    void init();
private:
     std::vector<CBin>                               _cbins;   // cluster bins used for second round
    ////////////////params for shattering the barcode into bins////////////////////////////
     size_t                                          _span;  // number of bits of the bin key
     size_t                                          _pos;  // start from 0
     size_t                                          _mask;   // mask used to extract the
     size_t                                          _offset;  // number of bits needs to be
     size_t                                          _klen;  // number of bits that represents the barcodes, start from the low end
     //////////////params for clusting the bins and barcodes clusters////////////////////////
     size_t                                          _cutoff;
     std::list<std::shared_ptr<cluster>>             _clusters;
     CLUSTERTYPE                                     _ctype;

     size_t                                          _dist_threshold; // used to define whether two barcodes are similar.
     double                                          _zvalue; //used for merge test
     bool                                            _pool; // indicate whether use the pooled test or indepedent test.
     // the cluster size difference between current clustring step with the previous step
     // if the difference/previous cluster size smaller than this threshold
     // the clustering algorithm stop
     double                                          _stopThres;
     // used to tell which cluster is low frequency and which is high frequency
     int                                             _splitThreshold;
};

}
#endif // CLUSTERPIPELINE_H

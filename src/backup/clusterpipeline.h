#ifndef CLUSTERPIPELINE_H
#define CLUSTERPIPELINE_H
#include <math.h>
#include "suffix_commons.h"
#include "kmer_freq.h"
#include "barcodecounting.h"
#include "cluster.h"
#include "clusteralgorithm.h"
#include "timer.h"
namespace prefixMatching{
class clusterPipline
{
public:
    typedef list<kmers_freq>                Bin;
    typedef list<std::shared_ptr<cluster>>  CBin;
public:
    clusterPipline(size_t pos,size_t span,size_t klen,size_t cutoff = 10,CLUSTERTYPE ctype=DICTATOR,double stopThres = 0.01);
    bool clusterDrive(const BarcodeTable&);
    bool clusterDrive(const list<std::shared_ptr<cluster>>&);
    const std::list<std::shared_ptr<cluster>>& clusters(){return this->_clusters;}

private:
    /////////////////////////////////////////////////////////////////////
    //////////////////////Steps for clustering//////////////////////////
    /// ///////////////////////////////////////////////////////////////
    /**
      * \brief shatter the barcodes into bins using the given region
    */
    void shatter(const BarcodeTable& table);
    /**
      * \brief shatter the first round clusters into bins
    */
    bool shatter(const list<std::shared_ptr<cluster>>&);

    /**
      * \brief first round clustering that starting from single barcodes
    */
    void innerBinClustering();
    /**
      * \brief second round clustering on top of the result from the first round clustering
      *
    */
    void crossBinClustering();
    const vector<list<kmers_freq>>&  bins()const{return this->_bins;}
private:
    void init();
private:
     vector<CBin>                               _cbins;   // cluster bins used for second round
     vector<Bin>                                _bins;   // bins shattered by the bin key
    ////////////////params for shattering the barcode into bins////////////////////////////
     size_t                                     _span;  // number of bits of the bin key
     size_t                                     _pos;  // start from 0
     size_t                                     _mask;   // mask used to extract the
     size_t                                     _offset;  // number of bits needs to be
     size_t                                     _klen;  // number of bits that represents the barcodes, start from the low end
     //////////////params for clusting the bins and barcodes clusters////////////////////////
     size_t                                     _cutoff;
     std::list<std::shared_ptr<cluster>>        _clusters;
     CLUSTERTYPE                                _ctype;
     double                                     _stopThres; // the cluster size difference between current clustring step with the previous step
                                                             // if the difference/previous cluster size smaller than this threshold
                                                             // the clustering algorithm stop
};

}
#endif // CLUSTERPIPELINE_H

#include "clusterpipeline.h"
#include "dictatorcluster.h"
#include "meansequentialestimator.h"
#include "typdefine.h"
#include "clusteralgorithm.h"
#include "clusteringwithtest.h"
#include "errorintroducer.h"
#include "distanceselector.h"
#include "formats.h"
#include "clustermergerunpooltester.h"
#include "clustermergerpooltester.h"
#include "clustermergeronesampletester.hpp"
#include "clustersplitter.h"
#include "meanestimator.hpp"

#include <algorithm>
namespace barcodeSpace {

clusterPipline::clusterPipline(size_t pos,size_t span,size_t klen,
                               double error_rate,
                               double zvalue,
                               TESTSTRATEGY pool,
                               double trim,
                               double stopThres):
                                _span(span),_pos(pos),_mask(0),
                                _offset(0),_klen(klen),
                                _error_rate(error_rate),
                                _zvalue(zvalue), _pool(pool),
                                _trim(trim), _stopThres(stopThres)
{
    assert(this->_stopThres < 1);
    _ctype = CLUSTERTYPE::DICTATOR;
    this->init();
}
void clusterPipline::init(){
    //
    assert(this->_klen%2 == 0);
    //assert(this->_klen >= (_pos + _span));
    size_t total = static_cast<size_t>(pow(2,this->_span));
    this->_cbins.assign(total,CBin());
    _mask = total -1;
    this->_offset = this->_klen - _pos - _span;
    this->_clusters.clear();
    
    if (_pool == TWOPROPORTIONPOOLED) {
        std::cout << "Using two sample pooled test" << std::endl;
        _tester.reset(new ClusterMergerPoolTester(_zvalue));
    } else if (_pool == TWOPROPORTIONUNPOOLED) {
        std::cout << "Using two sample unpooled test" << std::endl;
        _tester.reset(new ClusterMergerUnPoolTester(_zvalue));
    } else  {
        std::cout << "Using one sample test" << std::endl;
        assert(_pool == ONEPROPORTION);
        _tester.reset(new ClusterMergerOneSampleTester(_zvalue, _error_rate));
    }
}
/**
  * Given a list of raw barcodes, split them into
  * two groups, one contains high frequent barcode
  * The other goup contain low frequent barcode
*/
void clusterPipline::transform(const barcodeFreqTable& barcodes){

    _clusters.clear();
    size_t max_size = 0;
    if(_ctype == DICTATOR){

        for(auto it = barcodes.begin(); it != barcodes.end(); it++){

            DictatorCenter* c = new DictatorCenter(it->first,this->_klen/2);

            std::vector<freq> colum({it->second});
            cluster* temp = new dictatorCluster(std::shared_ptr<DictatorCenter>(c),colum);
            std::shared_ptr<cluster> ptemp(temp);
            max_size = std::max(max_size, it->second);
            _clusters.push_back(ptemp);
        }
    }else{
        throw runtime_error("Does not support the cluster type!\n");
    }

    // probably needs to
    DistanceSelector selector(_error_rate, 0.5, this->_klen);
    _dist_threshold = selector.calculateDistance(max_size) + 1;
}

bool clusterPipline::shatter(const list<std::shared_ptr<cluster>>& clusters){
    if(this->_pos + this->_span > this->_klen)
        return false;
    this->_offset = this->_klen - this->_pos - this->_span;
    //this->_cbins.clear();
    // clear each bin
    for(auto iter = this->_cbins.begin(); iter != this->_cbins.end(); iter++)
        iter->clear();

    for(auto iter = clusters.begin(); iter != clusters.end(); iter++){
        const kmer&k = (*iter)->center()->center();
        size_t index = this->_mask &(k >> this->_offset);
        this->_cbins[index].push_back(*iter);
    }
    this->_pos = this->_pos + this->_span;
    return true;
}

void clusterPipline::crossBinClustering(){
    if(this->_clusters.size()){
        this->_clusters.clear();
        
        clusterAlgorithm* temp = new ClusteringWithTest(this->_splitThreshold,this->_klen/2, _dist_threshold, _tester);
        
        std::shared_ptr<clusterAlgorithm> ptemp(temp);
        for(auto iter = this->_cbins.begin(); iter != this->_cbins.end(); iter++){
            if(iter->size()){
                ptemp->clusterImp(*iter);
                this->_clusters.insert(this->_clusters.end(),ptemp->clusters().begin(),ptemp->clusters().end());
            }
        }
    }
}

void clusterPipline::ErrorIntroduceStage(){
    if(this->_clusters.size()){
        this->_clusters.clear();
        clusterAlgorithm* temp = new ErrorIntroducer(this->_splitThreshold,this->_klen/2,1);
        std::shared_ptr<clusterAlgorithm> ptemp(temp);
        for(auto iter = this->_cbins.begin(); iter != this->_cbins.end(); iter++){
            if(iter->size()){
                ptemp->clusterImp(*iter);
                this->_clusters.insert(this->_clusters.end(),ptemp->clusters().begin(),ptemp->clusters().end());
            }
        }
    }
}
bool clusterPipline::clusterDrive(const barcodeFreqTable& barcodetable){
    std::cout << "transforming the barcodes into clusters" << std::endl;
    // 1. Transform the barcode table into a list of clusters
    this->transform(barcodetable);
    MeanEstimator mean_estimator(_trim);
    
    this->_splitThreshold = ceil(mean_estimator.mean(_clusters)) + 1;
    std::cout << "Initial number of unique barcodes(spacers are removed):  " << _clusters.size() <<std::endl;
    
    std::cout << "The distance threshold is " << _dist_threshold << std::endl;
    std::cout << "Cluster splitting threshold is  " << _splitThreshold << std::endl;
    
    /*std::cout << "The distance threshold is " << _dist_threshold << std::endl;
    
    std::cout << "The cluster split threshold is  " << _splitThreshold << std::endl;
    std::cout << "The test z-value is " << _zvalue << std::endl;
    std::cout << "The test choice is " << (_pool ? "pooled" : "unpooled") << std::endl;
    */
    // 2. First try to assign low frequency barcode to high frequency barcode
    // only consider those barcode distance is equal to 1.
    size_t sz(this->_clusters.size());

    size_t total(0);

    Timer* t = new realTimer(cout);

    while(true){
        if(sz == 0 || !this->shatter(this->_clusters))
            break;

        total++;
        cout<<"Clustering iteration "<<total<<endl;
        this->crossBinClustering();
        size_t tmp(this->_clusters.size());
        if(static_cast<double>(sz - tmp)/sz < this->_stopThres)
            break;
        sz = tmp;
    }

    std::cout<<"Clustering took  ";

    delete t;
    return true;
}
/**
  * \brief need more work to debug this.
 */
bool clusterPipline::clusterDrive(const list<std::shared_ptr<cluster>>& clusters){

    this->shatter(clusters);
    size_t sz(this->_clusters.size());
    // 4. Do the cross bin clustering, until the seed reaches the end of the barcode or
    //    the cluster size does not change much
    size_t total(0);
    while(true){
        total++;
        cout<<"Cross bin clustering iteration "<<total<<endl;
        if(sz == 0)
            break;
        if(this->shatter(this->_clusters)){
            this->crossBinClustering();
            size_t tmp(this->_clusters.size());
            if(static_cast<double>(sz - tmp)/sz < this->_stopThres)
                break;
            sz = tmp;
        }else
            break;
    }
    return true;
}
}

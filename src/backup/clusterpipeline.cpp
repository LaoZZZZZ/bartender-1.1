#include "clusterpipeline.h"

namespace prefixMatching {

clusterPipline::clusterPipline(size_t pos,size_t span,size_t klen,size_t cutoff,CLUSTERTYPE ctype,double stopThres):
                                _span(span),_pos(pos),_mask(0),_offset(0),_klen(klen),_cutoff(cutoff),_ctype(ctype),_stopThres(stopThres)
{
    assert(this->_stopThres < 1);
    this->init();
}
void clusterPipline::init(){
    //
    assert(this->_klen%2 == 0);
    assert(this->_klen > (_pos + _span));
    size_t total = static_cast<size_t>(pow(2,this->_span));
    this->_bins.assign(total,Bin());
    this->_cbins.assign(total,CBin());
    _mask = total -1;
    this->_offset = this->_klen - _pos - _span;
    this->_clusters.clear();
}
void clusterPipline::shatter(const BarcodeTable& table){

    BarcodeTable::const_iterator iter;
    for(iter = table.begin(); iter != table.end(); iter++){
        const kmer& k = iter->first;
        size_t index = _mask &(k >> _offset);
        kmers_freq temp(k,iter->second);
        this->_bins[index].push_back(temp);
    }
}
bool clusterPipline::shatter(const list<std::shared_ptr<cluster>>& clusters){
    this->_pos = this->_pos + this->_span;
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
    return true;
}
void clusterPipline::innerBinClustering(){
    for(auto iter = this->_bins.begin(); iter != this->_bins.end(); iter++){
        if(iter->size()){
            clusterAlgorithm* temp = new clusterAlgorithm(this->_cutoff,this->_klen/2,*iter,this->_ctype);
            std::shared_ptr<clusterAlgorithm> ptemp(temp);
            ptemp->clusterImp();
            this->_clusters.insert(this->_clusters.end(),ptemp->clusters().begin(),ptemp->clusters().end());
        }
    }
    this->_bins.clear();
}
void clusterPipline::crossBinClustering(){
    if(this->_clusters.size()){
        this->_clusters.clear();
        for(auto iter = this->_cbins.begin(); iter != this->_cbins.end(); iter++){
            if(iter->size()){
                clusterAlgorithm* temp = new clusterAlgorithm(this->_cutoff,this->_klen/2,*iter,this->_ctype);
                std::shared_ptr<clusterAlgorithm> ptemp(temp);
                ptemp->clusterImp();
                this->_clusters.insert(this->_clusters.end(),ptemp->clusters().begin(),ptemp->clusters().end());
            }
        }
    }
}
bool clusterPipline::clusterDrive(const BarcodeTable& barcodetable){
    // 1. shatter the barcoe into specified bins
    this->shatter(barcodetable);
    // 2. do the inner bin clustering and pool all clusters together
    Timer* t = new realTimer(cout);
    this->innerBinClustering();

    std::cout<<"Within bin clustering takes "<<std::endl;
    delete t;
    // 3. get the initial size of cluster
    size_t sz(this->_clusters.size());
    // 4. Do the cross bin clustering, until the seed reaches the end of the barcode or
    //    the cluster size does not change much
    size_t total(0);
    t = new realTimer(cout);
    while(true){
        if(sz == 0)
            break;
        if(this->shatter(this->_clusters)){
            total++;
            cout<<"Cross bin clustering iteration "<<total<<endl;
            this->crossBinClustering();
            size_t tmp(this->_clusters.size());
            if(static_cast<double>(sz - tmp)/sz < this->_stopThres)
                break;
            sz = tmp;
        }else
            break;
    }
    std::cout<<"Among bin clustering takes "<<std::endl;
    delete t;
    return true;
}
/**
  * \brief need more work to debug this
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

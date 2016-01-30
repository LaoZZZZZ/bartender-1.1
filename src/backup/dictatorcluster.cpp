#include "dictatorcluster.h"

namespace prefixMatching{
dictatorCluster::dictatorCluster(std::shared_ptr<Center> c,const std::list<freq>& sz):cluster(c,sz)
{
}
/**
  * add barcode and update the center if necessary and intended
*/
void dictatorCluster::addBarcode(const kmers_freq& barcode,bool update){

    this->_barcodes.push_back(barcode);
    this->_score += barcode._freq*kmerDistance(barcode._key,this->_center->center(),this->_center->klen());
    this->_size += barcode._freq;
    if(update){//check if we need to update the center
        double tempscore(0);
        tempscore = similarityScore(barcode._key,this->barcodes(),this->_center->klen());
        if(tempscore < this->_score){
            Center* newcenter = new dictatorCenter(barcode._key,this->_center->klen());
            this->_center.reset(newcenter);
            this->_score = tempscore;
        }
    }
}
/**
  * combine two clusters into one cluster
*/
void dictatorCluster::merge(const cluster& cl){
    assert(cl.columnSize() == this->columnSize());
    auto liter = cl.colums().begin();
    for(auto iter = this->_pointSize.begin();
        iter != this->_pointSize.end(); iter++){
        *iter += *liter;
        liter++;
    }
    std::shared_ptr<Center> c2 = cl.center();
    const list<kmers_freq>& b2 = cl.barcodes();
    //combine the barcodes of these two clusters

    double myscore = this->centerScore() + similarityScore(this->center()->center(),cl.barcodes(),this->center()->klen());
    double c2score = cl.centerScore() + similarityScore(cl.center()->center(),this->barcodes(),c2->klen());
    if(myscore > c2score){
        this->_center = cl.center();
    }
    this->_barcodes.insert(this->_barcodes.end(),b2.begin(),b2.end());
    this->_size += cl.size();
}
}

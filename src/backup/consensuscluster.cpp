#include "consensuscluster.h"

namespace prefixMatching{

consensusCluster::consensusCluster(const kmers_freq& k,size_t klen,
                                   const std::list<freq>& pointsz):
                                    cluster(NULL,pointsz){
    Center* tempc = new dictatorCenter(k._key,klen);
    this->_center.reset(tempc);
    for(size_t i = 0; i < klen; i++){
        std::vector<int> tempf(4,k._freq);
        this->_bpFrequency.push_back(tempf);
    }
}
/**
  * combine two clusters into one cluster
*/
void consensusCluster::merge(const cluster& cl){

    BTDnaString representative;
    // update the base pair frequency for this cluster
    const vector<vector<int>> freqtable = cl.bpFrequency();
    for(size_t i = 0; i < freqtable.size(); i++){
        int biggest = 0;
        int index = 0;
        for(size_t j = 0; j < 4; j++){
            this->_bpFrequency[i][j] += freqtable[i][j];
            if(this->_bpFrequency[i][j] > biggest){
                biggest = this->_bpFrequency[i][j];
                index = j;
            }
        }
        representative.append(index);
    }
    kmer k(0);
    kmers_BitwiseTransform::getInstance()->seq_2_bitwise(representative,k,this->_center->klen());
    Center* c = new dictatorCenter(k,this->_center->klen());
    this->_center.reset(c);
    this->_barcodes.insert(this->_barcodes.end(),cl.barcodes().begin(),cl.barcodes().end());
    //update size
    this->_size += cl.size();
}
void consensusCluster::addBarcode(const kmers_freq&,bool){
    throw 1;
}
}

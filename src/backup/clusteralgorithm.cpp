#include "clusteralgorithm.h"

namespace prefixMatching{

clusterAlgorithm::clusterAlgorithm(size_t cutoff,size_t klen,
                                   const list<kmers_freq>& barcodes,
                                   CLUSTERTYPE ctype):
                                    _cutoff(cutoff),_klen(klen),_ctype(ctype)
{
    this->split(barcodes);
}

clusterAlgorithm::clusterAlgorithm(size_t cutoff,size_t klen,
                                   const list<std::shared_ptr<cluster> >& clusters,
                                   CLUSTERTYPE ctype):
                                   _cutoff(cutoff),_klen(klen),_ctype(ctype)
{
    this->split(clusters);
}
/**
  * Given a list of raw barcodes, split them into
  * two groups, one contains high frequent barcode
  * The other goup contain low frequent barcode
*/
void clusterAlgorithm::split(const list<kmers_freq>& barcodes){

    this->_clusters.clear();
    if(_ctype == DICTATOR){
        for(auto it = barcodes.begin(); it != barcodes.end(); it++){
            Center* c = new dictatorCenter(it->_key,this->_klen);
            std::list<freq> colum({it->_freq});
            cluster* temp = new dictatorCluster(std::shared_ptr<Center>(c),colum);
            std::shared_ptr<cluster> ptemp(temp);
            if(it->_freq < this->_cutoff){
                this->_antenna.push_back(ptemp);
            }else{
                this->_clusters.push_back(ptemp);
            }
        }
    }else if(_ctype == CONSENSUS){
        for(auto it = barcodes.begin(); it != barcodes.end(); it++){
            std::list<freq> colum({this->_klen});
            cluster* temp = new consensusCluster(*it,this->_klen,colum);
            std::shared_ptr<cluster> ptemp(temp);
            if(it->_freq < this->_cutoff){
                this->_antenna.push_back(ptemp);
            }else{
                this->_clusters.push_back(ptemp);
            }
        }
    }

}

/**
  * Given a list of clusters, split them into
  * two groups, one contains high frequent barcode
  * The other goup contain low frequent barcode
*/
void clusterAlgorithm::split(const list<std::shared_ptr<cluster>>& cls){

    this->_clusters.clear();
    for(auto it = cls.begin(); it != cls.end(); it++){
        if((*it)->size() < this->_cutoff){
            this->_antenna.push_back(*it);
        }else{
            this->_clusters.push_back(*it);
        }
    }

}
void clusterAlgorithm::clusterImp(bool pairwise){
    /*
    int total = 0;
    for(auto it = this->_antenna.begin(); it != this->_antenna.end(); it++)
        total += (*it)->size();
    for(auto it = this->_clusters.begin(); it != this->_clusters.end(); it++)
        total += (*it)->size();
    */
    for(auto it = this->_antenna.begin(); it != this->_antenna.end(); it++){
        std::list<list<std::shared_ptr<cluster> >::iterator > temp;
        // find distance within specified distance clusters, them merge them if possilbe
        for(auto init = this->_clusters.begin(); init != this->_clusters.end(); init++){
            if((*it)->center()->isSimilar(*((*init)->center().get()))){
                temp.push_back(init);
            }
        }
        // if there is only one cluster that is similar to the low frequency barcode
        // just merge it to the cluster
        if(temp.size() == 1){
            (*temp.front())->merge(*(it->get()));
        }

        // if there are multiple clusters that is within 2-mismatch with the low frequent barcode
        // in this case, merge them if two clusters are within 2-mismatch distance
        else if(temp.size() >= 2){
            auto firstcluster = temp.begin();
            std::shared_ptr<cluster> c = *(*firstcluster);
            c->merge(*(it->get()));
            //pop out the first element
            temp.pop_front();
            // compare the first cluster in the temp list with the remaining clusters in the temp list
            // merge them if necessary and erase the merges clusters from the global cluster list (_cluster)
            while(temp.size()){
                firstcluster = temp.begin();
                while(firstcluster != temp.end()){
                    // the two clusters are similar
                    //merge them and
                    if(c->center()->isSimilar(*((*(*firstcluster))->center()))){
                        c->merge(*((*firstcluster)->get()));
                        this->_clusters.erase(*firstcluster);
                        firstcluster = temp.erase(firstcluster);
                    }else
                        firstcluster++;
                }
                if(temp.size()){
                    c = *(temp.front()); //pick up the first cluster left in the temp
                    temp.pop_front();  // pop out the first cluster
                }
            }

        }else{
            this->_clusters.push_back(*it);
        }

        temp.clear();
    }
    // once we finished the first scan using the low frequency clusters
    // we need to do pairwise comparison between the big clusters
    // merge them if necessary
    /*
    if(pairwise){
        for(auto iter = this->_clusters.begin(); iter != this->_clusters.end(); iter++){
            auto inner = iter;
            advance(inner,1);
            while(inner != this->_clusters.end()){
                if((*iter)->center()->isSimilar(*((*inner)->center().get()))){
                    (*iter)->merge(*((*inner).get()));
                    inner = this->_clusters.erase(inner);
                }
                else
                    inner++;
            }
        }
    }*/
    /*
    int checktotal(0);
    for(auto iter = this->_clusters.begin(); iter != this->_clusters.end(); iter++)
        checktotal += (*iter)->size();
    if(checktotal != total){
        cout<<checktotal<<'\t'<<total<<endl;
        exit(0);
    }
    */
}
}

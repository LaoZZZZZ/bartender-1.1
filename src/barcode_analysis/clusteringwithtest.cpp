#include "clusteringwithtest.h"


#include "cluster.h"
#include "clusteralgorithm.h"
#include "clustermergerpooltester.h"
#include "clustermergerunpooltester.h"
#include "clustermergeronesampletester.hpp"

#include <list>
#include <memory>

using namespace std;
namespace barcodeSpace {
ClusteringWithTest::ClusteringWithTest(size_t cutoff,size_t klen,
                                       size_t dist_thres,
                                       std::shared_ptr<ClusterMergerTester> tester)
    : clusterAlgorithm(cutoff, klen, dist_thres), _tester(tester)
{

}

void ClusteringWithTest::split(const list<std::shared_ptr<cluster>>& cls){

    for(const auto& c : cls){
        if(c->size() <= this->_cutoff){
            this->_antenna.push_back(c);
        }else{
            this->_clusters.push_back(c);
        }
    }
    this->_antenna.sort([](const std::shared_ptr<cluster>& c1,
                        const std::shared_ptr<cluster>& c2) {return c1->size() > c2->size();});

}
/**
  * Given a list of clusters, split them into
  * two groups, one contains high frequent barcode
  * The other goup contain low frequent barcode
*/
void ClusteringWithTest::cleanSplit(const list<std::shared_ptr<cluster>>& cls){

    for(const auto& c : cls){
        if(c->size() <= this->_cutoff){
            this->_antenna.push_back(c);
        }else{
            this->_clusters.push_back(c);
        }
    }
    this->_antenna.sort([](const std::shared_ptr<cluster>& c1,
                        const std::shared_ptr<cluster>& c2) {return c1->size() < c2->size();});
    auto iter = _antenna.begin();
    while (iter != _antenna.end()){
        bool found = false;
        for (const auto& large_c : _clusters) {
            if (this->clusterDistance(*iter, large_c) <= this->_dist_thres) {
                found = true;
                break;
            }
        }
        if (!found) {
            _clusters.push_back(*iter);
            iter = _antenna.erase(iter);
        } else {
            ++iter;
        }
    }
}

void ClusteringWithTest::clusterImp(const std::list<std::shared_ptr<cluster>>& cs){
    this->reset();
    this->split(cs);
    //this->cleanSplit(cs);
    // Put all large clusters into _clusters
    // for comparison.

    typedef list<std::shared_ptr<cluster>>::iterator cluster_iterator;
    for(const auto& small_c : this->_antenna) {
        std::list<cluster_iterator> similar_clusters;
        size_t closest_distance = this->_klen + 1;
        std::shared_ptr<cluster> best;
        for(auto large_c = this->_clusters.begin(); large_c != this->_clusters.end(); ++large_c) {
        //for(auto& large_c : this->_clusters) {
            size_t d = this->clusterDistance(small_c, *large_c);
            // update the best matched cluster.
            if (d < closest_distance) {
                best = *large_c;
                closest_distance = d;
            } else if (d == closest_distance) {
                if (best->size() < (*large_c)->size()) {
                    best = *large_c;
                }
            }
            // they are similar.
            if (d <= this->_dist_thres) {
                similar_clusters.push_back(large_c);
            }
        }
        // Can not find similar clusters.
        if (closest_distance > this->_dist_thres) {
            assert(similar_clusters.empty());
            this->_clusters.push_back(small_c);
        } else {

            best->merge(*small_c);

            similar_clusters.sort([](const cluster_iterator& iter1,
                                     const cluster_iterator& iter2) {return (*iter1)->size() > (*iter2)->size();});
            // Do pairwise comparision between those similar large clusters.
            // merge them if necessary.
            auto firstcluster = similar_clusters.begin();
            std::shared_ptr<cluster> c = *(*firstcluster);
            //Pop out the first element
            similar_clusters.pop_front();

            // Compare the first cluster in the temp list with the remaining clusters in the temp list.
            // merge them if necessary and erase the merges clusters from the global cluster list (_cluster).
            while(!similar_clusters.empty()){
                firstcluster = similar_clusters.begin();
                while(firstcluster != similar_clusters.end()){
                    //merge the two clusters if they are similar
                    // and they pass the merge test, which currently is the two sample proportion test.
                    size_t temp_d = this->clusterDistance(*(*firstcluster), c);
                    if (temp_d <= this->_dist_thres &&
                        _tester->shouldMerge(c->bpFrequency(), (*(*firstcluster))->bpFrequency())) {
                        c->merge(*((*firstcluster)->get()));
                        this->_clusters.erase(*firstcluster);
                        firstcluster = similar_clusters.erase(firstcluster);
                    }else{
                        ++firstcluster;
                    }
                }
                if (!similar_clusters.empty()) {
                    c = *(similar_clusters.front()); //pick up the first cluster left in the temp.
                    similar_clusters.pop_front();  // pop out the first cluster.
                }
            }
        }
    }

}
}

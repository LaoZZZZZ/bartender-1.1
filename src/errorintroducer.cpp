#include "errorintroducer.h"

#include "barcodemutationgenerator.h"
#include "clusteralgorithm.h"
#include "typdefine.h"

#include <cassert>
#include <list>
#include <memory>
#include <vector>

using namespace std;
namespace barcodeSpace {
ErrorIntroducer::ErrorIntroducer(size_t cutoff,size_t klen,
                                 size_t dist_thres) :
    clusterAlgorithm(cutoff, klen, dist_thres), _mutator(klen)
{

}

void ErrorIntroducer::clusterImp(const std::list<std::shared_ptr<cluster> >& cs){
    this->reset();
    this->cleanSplit(cs);
    vector<kmer> mutated_centers(3 * this->_klen,0);
    for(const auto& small_c : this->_antenna) {
        std::shared_ptr<cluster> best;
        _mutator.mutateCenterInplace(small_c->center()->center(),
                                     mutated_centers);
        for (const kmer& m : mutated_centers) {
            if (this->_large_cluster_table.find(m) !=
                    this->_large_cluster_table.end()) {
                if (!best.get() || best->size() <
                        this->_large_cluster_table[m]->size()) {
                    best = this->_large_cluster_table[m];
                }
            }
        }
        // There is a match.
        if (best.get()) {
            best->merge(*small_c);
        } else {
            this->_large_cluster_table.insert({small_c->center()->center(), small_c});
        }
    }
    for (const auto& c : _large_cluster_table) {
        this->_clusters.push_back(c.second);
    }
}

void ErrorIntroducer::cleanSplit(const std::list<std::shared_ptr<cluster> >& cs) {
    for(const auto& c : cs){
        if(c->size() < this->_cutoff){
            this->_antenna.push_back(c);
        }else{
            this->_large_cluster_table.insert({c->center()->center(), c});
        }
    }
    BarcodeMutationGenerator mutator(this->_klen);
    this->_antenna.sort([](const std::shared_ptr<cluster>& c1,
                        const std::shared_ptr<cluster>& c2) {return c1->size() > c2->size();});
    vector<kmer>    mutated_barcodes(3 * this->_klen, 0);
    auto iter = _antenna.begin();
    while (iter != _antenna.end()) {
        bool found = false;
        mutator.mutateCenterInplace((*iter)->center()->center(), mutated_barcodes);
        for ( const kmer& m : mutated_barcodes) {
            if (this->_large_cluster_table.find(m) !=
                    this->_large_cluster_table.end()) {
                found = true;
                break;
            }
        }
        if (!found) {
            this->_large_cluster_table.insert({(*iter)->center()->center(), *iter});
            iter = _antenna.erase(iter);
        } else {
            ++iter;
        }
    }
}
}   // namespace barcodeSpace

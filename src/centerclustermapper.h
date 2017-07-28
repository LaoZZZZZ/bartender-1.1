#ifndef CENTERCLUSTERMAPPER_H
#define CENTERCLUSTERMAPPER_H

#include "typdefine.h"
#include "barcodecluster.hpp"

#include <cassert>
#include <list>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>


namespace barcodeSpace {

class CenterClusterMapper
{
public:
    typedef BarcodeCluster Cluster;
    CenterClusterMapper(int max_id);

    const std::list<std::string>& myCenter(size_t c_id) {
        return _cluster_2_center[c_id];
    }
    // Checks if it has the center in the dictionary.
    bool hasCenter(const std::string& c) {
        return _center_2_cluster.find(c) != _center_2_cluster.end();
    }

    // Checks if it has the cluster.
    bool hasCluster(size_t c_id) {
        if (c_id >= _cluster_2_instance.size()) {
            throw std::out_of_range("The id exceeds the maximum possible id.");
        }
        return _cluster_2_instance[c_id].get() != NULL;
    }

    // RClusterResultProcessoremoves the cluster and its centers.
    void removeCluster(size_t cid) {
        if (_numOfClusters <= 0) {
            return;
        }
        assert(cid < _cluster_2_instance.size());
        if (_cluster_2_instance[cid].get()) {
            _cluster_2_instance[cid].reset();
            for (const auto& c : _cluster_2_center[cid]) {
                _center_2_cluster.erase(c);
            }
            _cluster_2_center[cid].clear();
        }
        _numOfClusters -= 1;
    }

    std::shared_ptr<Cluster> getClusterByCenter(const std::string& c) {
        if (hasCenter(c))
            return _cluster_2_instance[_center_2_cluster[c]];
        else {
            return NULL;
        }
    }

    // Gets the cluster pointer.
    const std::shared_ptr<Cluster>& getClusterByID(size_t id) {
        if (id >= _cluster_2_instance.size()) {
            throw std::out_of_range("The id exceeds the maximum possible id.");
        }
        return _cluster_2_instance[id];
    }

    // Adds mapping pair between the cluster and its centers.
    void addPair(const std::shared_ptr<Cluster>& cl, const std::vector<std::string>& centers) {
        assert(cl.get());
        size_t id = cl->ClusterID();
        if (id >= _cluster_2_instance.size()) {
            throw std::out_of_range("The id exceeds the maximum possible id.");
        }
        if (_cluster_2_instance[id].get()) {
            throw std::runtime_error("Already have cluster " + std::to_string(id) + "when adding it to linkmapper!\n");
        }
        _numOfClusters += 1;
        _cluster_2_instance[id] = cl;
        for (const auto& c : centers) {
            _cluster_2_center[id].push_back(c);
            if (_center_2_cluster.find(c) != _center_2_cluster.end() &&
		_center_2_cluster[c] != cl->ClusterID()) {
                throw std::runtime_error("Two custers share the same center(cluster ids are: " + std::to_string(_center_2_cluster[c]) +
                                         " and " + std::to_string(cl->ClusterID()) + ").\n");
            }
            _center_2_cluster[c] = id;
        }
    }

    size_t numberOfClusters() const {return _numOfClusters;}

private:
    std::vector<std::list<std::string>>     _cluster_2_center;
    std::unordered_map<std::string, int>    _center_2_cluster;
    std::vector<std::shared_ptr<Cluster>>   _cluster_2_instance;
    size_t _numOfClusters;
};
}   // namespace barcodeSpace
#endif // CENTERCLUSTERMAPPER_H

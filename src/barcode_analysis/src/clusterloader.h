#ifndef CLUSTERLOADER_H
#define CLUSTERLOADER_H

#include "cluster.h"
#include "csvreader.h"

#include <memory>
#include <string>
#include <vector>
#include <list>
#include <unordered_map>

namespace barcodeSpace {
// Loads the clusters from the file.
class ClusterLoader
{
public:
    ClusterLoader(const std::string& cluster_filename,
                  const std::string& frequency_filename);
    bool LoadClusters(std::unordered_map<int, std::list<std::shared_ptr<cluster>>>* clusters);
private:
    CSVReader   _cluster_loader;    // file handler of the cluster file.
    CSVReader   _frequency_loader;  // file handler of the frequency table file.
    std::string      _cluster_filename;
    std::string      _frequency_filename;
};
}   // namespace barcodeSpace
#endif // CLUSTERLOADER_H

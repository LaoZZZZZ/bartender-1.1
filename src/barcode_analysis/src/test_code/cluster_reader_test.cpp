#include <iostream>
#include <cassert>
#include <memory>
#include <string>
#include <vector>
#include <array>
#include <list>

#include "../dictatorcenter.h"
#include "../kmers_bitwisetransform.h"
#include "../typdefine.h"
#include "../cluster.h"
#include "../dictatorcluster.h"
#include "../clusterloader.h"

using std::string;
using std::endl;
using std::cout;
using std::shared_ptr;
using std::vector;
using std::array;
using std::list;

using barcodeSpace::DictatorCenter;
using barcodeSpace::kmersBitwiseTransform;
using barcodeSpace::cluster;
using barcodeSpace::dictatorCluster;
using barcodeSpace::ClusterLoader;

int main() {
    const string cluster_file = "../../data/clusters_cluster.csv";
    const string frequency_file = "../../data/clusters_bp_frequency.csv";
    size_t klen = 20;
    ClusterLoader loader(cluster_file, frequency_file, klen);
    std::list<std::shared_ptr<cluster>> clusters;    
    bool success = loader.LoadClusters(&clusters);
    cout << clusters.size() <<endl;
    
    return 0;

}

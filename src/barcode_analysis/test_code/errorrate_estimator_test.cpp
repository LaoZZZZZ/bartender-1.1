#include <iostream>
#include <cassert>
#include <memory>
#include <string>
#include <vector>
#include <array>
#include <list>

#include "../typdefine.h"
#include "../cluster.h"
#include "../errorrateestimator.h"
#include "../clusterloader.h"

using std::string;
using std::endl;
using std::cout;
using std::shared_ptr;
using std::vector;
using std::array;
using std::list;

using barcodeSpace::cluster;
using barcodeSpace::ClusterLoader;
using barcodeSpace::ErrorRateEstimator;
int main() {
    const string cluster_file = "../../simulation_data/clusters_cluster.csv";
    const string frequency_file = "../../simulation_data/clusters_bp_frequency.csv";
    size_t klen = 20;
    ClusterLoader loader(cluster_file, frequency_file, klen);
    std::list<std::shared_ptr<cluster>> clusters;    
    bool success = loader.LoadClusters(&clusters);
    assert(success);    

    // Estimate the error rate 
    double entropy_thres = 0.2;
    int cluster_size = 10;
    int minimum_barcodes = 100000;
    ErrorRateEstimator estimator(entropy_thres, cluster_size, minimum_barcodes);
    estimator.Estimate(clusters);
    cout << estimator.ErrorRate() << endl;
    
    return 0;

}

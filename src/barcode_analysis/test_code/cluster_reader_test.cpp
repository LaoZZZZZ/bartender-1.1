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


using namespace std;
using namespace barcodeSpace;
int main() {
    const string cluster_file = "/Users/lukez/Documents/barcode_single_end/simulation_data/trucated_cluster_cluster.csv";
    const string frequency_file = "/Users/lukez/Documents/barcode_single_end/simulation_data/trucated_cluster_quality.csv";
    ClusterLoader loader(cluster_file, frequency_file);
    std::unordered_map<int,std::list<std::shared_ptr<cluster>>> clusters;    
    bool success = loader.LoadClusters(&clusters);
    for (const auto& c : clusters) {
	cout << c.first << '\t' << c.second.size() << endl;
    }
    
    return 0;

}


#include "clusteringdriver.hpp"
#include "clusterpruner.hpp"
#include "clusteralgorithm.h"
#include "clusteroutput.h"
#include "csvreader.h"
#include "errorrateestimator.h"
#include "formats.h"
#include "inputprocessor.hpp"
#include "rawbarcodeprocessor.hpp"
#include "pcrprocessor.hpp"
#include "mergebycenters.h"
#include "timer.h"
#include "util.h"
#include "rawbarcodeloader.hpp"

#include <algorithm>
#include <iostream>
#include <string>
#include <memory>

using namespace std;
using namespace barcodeSpace;

// The whole clustering process.
// 1. Loads the read from the sequence file and filter out those unqualified read
// based on the constraints on the barcode region.
// 2. Clusters those valid barcodes using the first k basepair.
// 3. Recursively Clusters those initial clusters based the second k basepair until
// the number of cluster does not change or change is very small.

struct CompareObject {
    bool operator()(const std::shared_ptr<BarcodeCluster>& a, const std::shared_ptr<BarcodeCluster>& b) {
        return a->size() < b->size();
    }
};

void drive(std::string barcodefile,  // original read file
           size_t freq_cutoff,   // frequency cutoff
           double error_rate,
           size_t seedlen,      // seed len
           std::string outprefix,
           double entropy_threshold, // Entopy value for considering as mixture position
           size_t maximum_centers, // The maximum centers for each cluster
           double zvalue = 4.0,
           TESTSTRATEGY test_method = TWOPROPORTIONUNPOOLED,
           double entropy_threshold_for_error = 0.33, // the majority bp accounts at least 95%.
           // The least size of cluster that will be considered as candidate when estimating the sequencing error
           size_t cluster_size_threshold_for_error = 20,
           // The total number of base pair for estimating sequencing error.
           size_t number_barcode_for_error_estimator = 1000000,
           bool head = false){
    

    //1. Determine the file format and check its correctness.
    cout << "Loading barcodes from the file" << endl;
    Timer* timer = new realTimer(cout);
    cout << "Load barcode from the file" << endl;
    RawBarcodeLoader loader(barcodefile);
    loader.process();
    cout << "It takes ";
    delete timer;
    cout << " to load the barcodes from " << barcodefile << endl;
    
    std::pair<size_t, size_t> barcode_length_range = loader.lengthRange();
    BarcodePool::createInstance(loader.barcodeTable());
    std::shared_ptr<BarcodePool> barcode_pool = BarcodePool::getAutoInstance();
    std::list<shared_ptr<BarcodeCluster>> clusters;
    BarcodeCluster::setBarcodePool(barcode_pool);
    ClusterPruner cluster_pruner(entropy_threshold,
                                 maximum_centers,
                                 zvalue,
                                 0.1, // percentage which viewed as potential centers.
                                 freq_cutoff,
                                 error_rate);
    for (size_t blen = barcode_length_range.first; blen <= barcode_length_range.second; ++ blen) {
	cout << "Start to clustering barcode with length " << blen << endl;
        ClusteringDriver cluster_drive(blen, seedlen, error_rate, zvalue, test_method);
        cluster_drive.clusterDrive(BarcodePool::getAutoInstance());
        
        std::list<std::shared_ptr<BarcodeCluster>> cur_clusters = cluster_drive.clusters();
        cluster_pruner.prune(cur_clusters);
	cout << "Identified " << cluster_pruner.prunedClusters().size() << " barcodes with length " << blen << endl;
        //merger->merge(cur_clusters, error_estimator->Entropies());
        clusters.insert(clusters.end(),
                        cluster_pruner.prunedClusters().begin(),
                        cluster_pruner.prunedClusters().end());
    }
    
    if (!clusters.empty()) {
        std::shared_ptr<ErrorRateEstimator> error_estimator(
                new ErrorRateEstimator(entropy_threshold_for_error,
                                       cluster_size_threshold_for_error,
                                       number_barcode_for_error_estimator));
        cout << endl << endl;
        cout << "***(Overall error rate estimated from the clustering result)***" << endl;
        error_estimator->Estimate(clusters, false);
        cout << "The estimated error rate is " << error_estimator->ErrorRate() << endl;
        cout<<"starting to dump clusters to file with prefix "<< outprefix <<endl;
        ClusterOutput out(outprefix);
        // Dumps the cluster information.
        out.WriteToFile(clusters, barcode_pool,barcode_length_range.second);
        
        // Remove the PCR effects
        
        cout << "Start to remove pcr effects" << endl;
        PCRProcessor pcr_dealer;
        pcr_dealer.process(clusters,barcode_pool);
	cluster_pruner.prune(clusters);
        ClusterOutput out_pcr(outprefix + "_pcr");
        // Dumps the cluster information.
        out_pcr.WriteToFile(cluster_pruner.prunedClusters(), barcode_pool,barcode_length_range.second);
    }

}

int main(int argc,char* argv[])
{
    Timer* t = new realTimer(cout);
    assert(argc >= 3);
    //1. first argument is the sequence file
    string sequencefile(argv[1]);
    //2. out put file
    string outprefix(argv[2]);
    size_t freq_cutoff = 1;
    
    if(argc >= 4)
        freq_cutoff = atoi(argv[3]);
    
    double error_rate = 0.01;
    if (argc >= 5) {
        error_rate = atof(argv[4]);
    }

    double zvalue = 4.0;
    if (argc >= 6) {
        zvalue = atof(argv[5]);
    }

       
    
    size_t seedlen = 5;
    if(argc >= 7)
        seedlen = atoi(argv[6]);
    
    TESTSTRATEGY pool = TWOPROPORTIONUNPOOLED;
    if (argc >= 8) {
        pool = static_cast<TESTSTRATEGY>(atoi(argv[7]));
    }

    double entropy_threshold = 0.33;
    if (argc >= 9) {
        entropy_threshold = atof(argv[8]);
    }
    
    size_t maximum_centers = 4;
    if (argc >= 10) {
        maximum_centers = atoi(argv[9]);
    }
    drive(sequencefile,
          freq_cutoff,
          error_rate,
          seedlen,
          outprefix,
          entropy_threshold,
          maximum_centers,
          zvalue,
          pool
          );
    delete t;
    return 0;
}


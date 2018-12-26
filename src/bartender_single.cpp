#include "clusteringdriver.hpp"
#include "barcodepoolstatistics.hpp"
#include "clusterpruner.hpp"
#include "clusteralgorithm.h"
#include "clusteroutput.h"
#include "csvreader.h"
#include "errorrateestimator.h"
#include "errorestimator.hpp"
#include "formats.h"
#include "inputprocessor.hpp"
#include "pcrprocessor.hpp"
#include "mergebycenters.h"
#include "timer.h"
#include "util.h"
#include "split_util.h"
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
           size_t seedlen,      // seed len
           size_t step,		// the distance between two adjacent seeds
           size_t num_threads, /// number of threads
           size_t distance,    // The hamming distance threshold
           std::string outprefix, // output prefix
           double entropy_threshold, // Entopy value for considering as mixture position
           size_t maximum_centers, // The maximum centers for each cluster
           StrandDirection strand_direction = FORWARD_DIRECTION, // the strand direction that should be considered. Default is forward only
           double zvalue = 4.0,
           TESTSTRATEGY test_method = TWOPROPORTIONUNPOOLED,
           double entropy_threshold_for_error = 0.33, // the majority bp accounts at least 95%.
           // The least size of cluster that will be considered as candidate when estimating the sequencing error
           size_t cluster_size_threshold_for_error = 20,
           // The total number of base pair for estimating sequencing error.
           int number_barcode_for_error_estimator = 1000000,
           bool head = false){
    

    //1. Determine the file format and check its correctness.
    //   Load the barcode from the input file
    cout << "Loading barcodes from the file" << endl;
    Timer* timer = new realTimer(cout);
    RawBarcodeLoader loader(barcodefile, strand_direction);
    loader.process();
    cout << "It takes ";
    delete timer;
    cout << " to load the barcodes from " << barcodefile << endl;
    
    //2.Calculate the position weight matrix and the entropy.
    //  Estimate the sequence error from the spacers.
    std::pair<size_t, size_t> barcode_length_range = loader.lengthRange();
    std::cout << "Shortest barcode length: " << barcode_length_range.first << std::endl << "Longest barcode: " << barcode_length_range.second << std::endl;
    BarcodePool::createInstance(loader.barcodeTable());
    std::shared_ptr<BarcodePool> barcode_pool = BarcodePool::getAutoInstance();
    BarcodeCluster::setBarcodePool(barcode_pool);
    loader.clear();
    // If the estimated sequence error is larger the default value.
    // Then the default error rate will be used for the following analysis.
    // A warning will be emitted in this case.
    double default_error_rate = 0.01;
    ////////////////////////////////This is part might be added back in the future////////////////////////
    ///////////////////////////////Please don't delete it/////////////////////////////////////////////////
    /*
    double error_entropy_threshold = Entropy({90,10,0,0});
    BarcodeStatistics pool_stats(barcode_pool);
    int largest_barcode_group = 0;
    int largest_group_size = 0;
    for (const auto& pwm : pool_stats.AllFullFrequencyTables()) {
        int temp = std::accumulate(pwm.second[0].begin(), pwm.second[0].end(),0);
        if (temp > largest_group_size) {
            largest_group_size = temp;
            largest_barcode_group = pwm.first;
        }
    }
    cout << "The barcode group that is used to estimate the sequence error : " << largest_barcode_group << '\t' << largest_group_size << endl;
    ErrorEstimator initial_error_estimator(number_barcode_for_error_estimator,
                                           error_entropy_threshold);
    
    double error_rate =initial_error_estimator.estimate(
                pool_stats.FullFrequencyTable(largest_barcode_group),
                                                        pool_stats.FullEntropy(largest_barcode_group));
    if (error_rate > default_error_rate || error_rate == 0.0) {
        std::cerr << "The estimated error rate(" << error_rate << ") is higher than the normal range" << std:: endl;
        std::cerr << "There might be some problem in the data and the default error_rate(0.01) will be used in the clustering process!" << std::endl;
    } else {
        default_error_rate = error_rate;
        std::cout << "The estimated sequence error from the spacers is " << error_rate << std::endl;
    }
    */ 
    std::list<shared_ptr<BarcodeCluster>> clusters;
    ClusterPruner cluster_pruner(entropy_threshold,
                                 maximum_centers,
                                 zvalue,
                                 0.1, // percentage which viewed as potential centers.
                                 freq_cutoff,
                                 default_error_rate);
    // Cluster each barcode group.
    // The group is defined as all barcode with same length.
    Timer* cluster_timer = new realTimer(cout);
    for (size_t blen = barcode_length_range.first; blen <= barcode_length_range.second; ++ blen) {
        cout << "Start to group barcode with length " << blen << endl;
        ClusteringDriver cluster_drive(blen, seedlen, step, num_threads, default_error_rate, zvalue, test_method, distance,0);
        cluster_drive.clusterDrive(BarcodePool::getAutoInstance());
        std::list<std::shared_ptr<BarcodeCluster>> cur_clusters = cluster_drive.clusters();
        cluster_pruner.prune(cur_clusters);
        cout << "Identified " << cluster_pruner.prunedClusters().size() << " barcodes with length " << blen << endl;
        //merger->merge(cur_clusters, error_estimator->Entropies());
        clusters.insert(clusters.end(),
                        cluster_pruner.prunedClusters().begin(),
                        cluster_pruner.prunedClusters().end());
    }
    
    cout << "The clustering process takes ";
    delete cluster_timer;
    cout << endl;
    
    if (!clusters.empty()) {
        cout<<"Start to dump clusters to file with prefix "<< outprefix <<endl;
        ClusterOutput out(outprefix);
        // Dumps the cluster information.
        out.WriteToFile(clusters, barcode_pool,barcode_length_range.second);
        
        // Remove the PCR effects
        cout << "Start to remove pcr effects" << endl;
        PCRProcessor pcr_dealer;
        pcr_dealer.process(clusters,barcode_pool);
        
        // Merge clusters that share the same center.
        MergeByCenters merger;
        merger.merge(clusters);
        //cluster_pruner.prune(clusters);
        ClusterOutput out_pcr(outprefix + "_pcr");
        cout << "***(Overall error rate estimated from the clustering result)***" << endl;
        std::shared_ptr<ErrorRateEstimator> error_estimator(
             new ErrorRateEstimator(entropy_threshold_for_error,
                                    cluster_size_threshold_for_error,
                                    number_barcode_for_error_estimator));
        cout << "Total number of clusters after removing PCR effects: " << merger.clusters().size() << endl;
        error_estimator->Estimate(merger.clusters(), false); // not silent, print out the information
        cout << "The estimated error rate is " << error_estimator->ErrorRate() << endl;
 
        // Dumps the cluster information post-pcr processing.
        if (pcr_dealer.numberOfReplicates() != 0) {
            cout <<"starting to dump clusters to file with prefix "<< outprefix + "_pcr" << endl;
            cout << "Removed " << pcr_dealer.numberOfReplicates() << " reads based on the initial clustering result." << std::endl;
            out_pcr.WriteToFile(merger.clusters(), barcode_pool,barcode_length_range.second);
        }
    }
}

int main(int argc,char* argv[])
{
    Timer* t = new realTimer(cout);
    if (argc < 3) {
	std::cerr << "Please use bartender_single_com script instead! "
                  << "Please refer to README about how to use Bartender." << std::endl;
        return 1;
    }
    //1. first argument is the sequence file
    string sequencefile(argv[1]);
    //2. output file
    string outprefix(argv[2]);
    size_t freq_cutoff = 1;
    
    if(argc >= 4)
        freq_cutoff = atoi(argv[3]);

    double zvalue = 4.0;
    if (argc >= 5) {
        zvalue = atof(argv[4]);
    }

    size_t seedlen = 5;
    if(argc >= 6)
        seedlen = atoi(argv[5]);
    
    size_t step = seedlen;
    if (argc >= 7)
	step = atoi(argv[6]);
    
    size_t num_threads = 1;
    if (argc >= 8)
	num_threads = atoi(argv[7]);
    
    size_t distance = 2;
    if (argc >= 9)
	distance = atoi(argv[8]);
    
    StrandDirection direction = FORWARD_DIRECTION;
    if (argc >= 10) {
        int dir = atoi(argv[9]);
        assert(dir == 0 || dir == 1);
        if (dir == 0) {
            direction = BOTH_DIRECTION;
        }
    }
    
    TESTSTRATEGY pool = TWOPROPORTIONUNPOOLED;
    if (argc >= 11) {
        pool = static_cast<TESTSTRATEGY>(atoi(argv[10]));
    }

    double entropy_threshold = 0.46;
    if (argc >= 12) {
        entropy_threshold = atof(argv[11]);
    }
    
    size_t maximum_centers = 4;
    if (argc >= 13) {
        maximum_centers = atoi(argv[12]);
    }
    
    drive(sequencefile,
          freq_cutoff,
          seedlen,
          step,
          num_threads,
          distance,
          outprefix,
          entropy_threshold,
          maximum_centers,
          direction,
          zvalue,
          pool
          );
    cout << "The overall running time ";
    delete t; 
    std::cout << " seconds." << endl;
    return 0;
}


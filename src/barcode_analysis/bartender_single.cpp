
#include "clusterpipeline.h"
#include "clusterpruner.hpp"
#include "clusteralgorithm.h"
#include "clusteroutput.h"
#include "csvreader.h"
#include "barcodeeditor.hpp"
#include "errorrateestimator.h"
#include "formats.h"
#include "inputprocessor.hpp"
#include "rawbarcodeprocessor.hpp"

#include "mergebycenters.h"
#include "timer.h"
#include "util.h"


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
    bool operator()(const std::shared_ptr<cluster>& a, const std::shared_ptr<cluster>& b) {
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
           TESTSTRATEGY pool = TWOPROPORTIONUNPOOLED,
           double entropy_threshold_for_error = 0.33, // the majority bp accounts at least 95%.
           // The least size of cluster that will be considered as candidate when estimating the sequencing error
           size_t cluster_size_threshold_for_error = 20,
           // The total number of base pair for estimating sequencing error.
           size_t number_barcode_for_error_estimator = 1000000,
           bool head = false){
    

    //1. Determine the file format and check its correctness.
    cout << "Loading the input from the file" << endl;
    CSVReader<string> reader(barcodefile,true);
    vector<string> test_row;
    std::unique_ptr<InputProcessor> processor;
    if (reader.nextRow(&test_row) && test_row.size() <= 2) {
        if (test_row.size() == 2) {
            // Assume there is no head defaultly.
            processor.reset(new RawBarcodeProcessor(barcodefile,false));
	
        } else {
            /*
            if (isdigit(test_row[0][0])) {
                processor.reset(new CountEncodedBarcodeProcessor(barcodefile, false));
            } else {
            	processor.reset(new CountBarcodeProcessor(barcodefile, false));
            }*/
            //throw runtime_error("Does not support the input file format!\n");
        }

    } else {
        stringstream msg;
        msg << "The input file " << barcodefile << " is not in correct format!" << endl;
        throw runtime_error(msg.str());
    }
    
    processor->process();
    cout << "Finished reading the input data" << endl;
    //const vector<pair<string,freq>>& sequence_list = processor->BarcodeList();
    // 2. Find those random positions. Use those bases position as seed for clustering
    /*BarcodeEditor editor(32U);
    editor.editBarcodes(sequence_list);
        */
    const vector<barcodeFreqTable>& barcode_tables = processor->BarcodeTable();
    const vector<unordered_map<kmer, list<string>>>& barcode_line_table = processor->BarcodeList();
    list<std::shared_ptr<cluster>> clusters;
     
    // Later used when the mixturebptester is completed.
    std::shared_ptr<ErrorRateEstimator> error_estimator(
                new ErrorRateEstimator(entropy_threshold_for_error,
                                       cluster_size_threshold_for_error,
                                       number_barcode_for_error_estimator));

    ClusterPruner cluster_pruner(entropy_threshold,
                                 maximum_centers,
                                 zvalue,
                                 0.1, // percentage which viewed as potential centers.
                                 freq_cutoff,
                                 error_rate);

     for (size_t blen = 1; blen < barcode_tables.size(); ++blen) {
        if (barcode_tables[blen].empty())
            continue;
        cout << endl << endl;
        cout << "*********************************************************************" << endl;
        cout << "Clustering those barcodes which have " << blen << " random positions" << endl;

        size_t start = 0;
        size_t klen = blen*2;
        if(seedlen*2 > klen)
            seedlen = 1;
        clusterPipline* pipe = new clusterPipline(start,seedlen*2,klen,
                                                  error_rate, zvalue, pool);
        std::shared_ptr<clusterPipline> pPipe(pipe);
        pPipe->clusterDrive(barcode_tables[blen]);

        const std::list<std::shared_ptr<cluster>>& cur_clusters = pPipe->clusters();
         
        //error_estimator->Estimate(cur_clusters, true);
         cluster_pruner.prune(cur_clusters);
        //merger->merge(cur_clusters, error_estimator->Entropies());
        const std::list<std::shared_ptr<cluster>>& pruned_clusters = cluster_pruner.prunedClusters();
        //std::sort(result.begin(), result.end(), CompareObject());
        cout<<"There are totally " << pruned_clusters.size()<< " clusters with length " << blen
            << " and size no less than " << freq_cutoff <<endl;
        clusters.insert(clusters.end(), pruned_clusters.begin(), pruned_clusters.end());
        cout << "#####################################################################" << endl;
    }
    
    if (!clusters.empty()) {
        cout << endl << endl;
        cout << "***************************(overall)*********************************" << endl;
        error_estimator->Estimate(clusters, false);
        cout << "The estimated error rate is " << error_estimator->ErrorRate() << endl;
        cout<<"starting to dump clusters to file with prefix "<< outprefix <<endl;

        ClusterOutput out(outprefix);
        // Dumps the cluster information.
        out.WriteToFile(clusters, barcode_line_table);
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
    
    TESTSTRATEGY pool = TWOPROPORTIONUNPOOLED;
    if (argc >= 7) {
        pool = static_cast<TESTSTRATEGY>(atoi(argv[6]));
    }
    
    size_t seedlen = 5;
    if(argc >= 8)
        seedlen = atoi(argv[7]);
    
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


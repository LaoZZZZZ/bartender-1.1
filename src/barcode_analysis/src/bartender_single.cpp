
#include "clusterpipeline.h"
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
           double p_value,  // p_value used to check whether the mixture frequency is high enough to be a center
           size_t maximum_centers, // The maximum centers for each cluster
           double zvalue = 10.0,
           bool pool = false,
           double entropy_threshold_for_error = 0.33, // the majority bp accounts at least 95%.
           // The least size of cluster that will be considered as candidate when estimating the sequencing error
           size_t cluster_size_threshold_for_error = 20,
           // The total number of base pair for estimating sequencing error.
           size_t number_barcode_for_error_estimator = 100000,
           bool head = false){
    

    //1. Determine the file format and check its correctness.
    cout << "Loading the input from the file" << endl;
    CSVReader reader(barcodefile,true);
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
    /* // the ultimate formats
    std::shared_ptr<MergeByCenters> merger(
                new MergeByCenters(entropy_threshold,
                                   maximum_centers,
                                   p_value,
                                   error_rate));
    */
    // currently the error_rate is set to 0.1 cause the mixturebptester is not finished yet.
    std::shared_ptr<MergeByCenters> merger(
                new MergeByCenters(entropy_threshold,
                                   maximum_centers,
                                   p_value,
                                   0.1)); // currently the error_rate is set to 0.1
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
                                                  freq_cutoff,zvalue, pool);
        std::shared_ptr<clusterPipline> pPipe(pipe);
        pPipe->clusterDrive(barcode_tables[blen]);

        const std::list<std::shared_ptr<cluster>>& cur_clusters = pPipe->clusters();
        error_estimator->Estimate(cur_clusters, true);
        merger->merge(cur_clusters, error_estimator->Entropies());
        const std::list<std::shared_ptr<cluster>>& merged_clusters = merger->clusters();
        //std::sort(result.begin(), result.end(), CompareObject());
        cout<<"There are totally " << merged_clusters.size()<< " clusters with length " << blen
            << " and size no less than " << freq_cutoff <<endl;
        clusters.insert(clusters.end(), merged_clusters.begin(), merged_clusters.end());
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
    size_t seedlen = 5;
    if(argc >= 6)
        seedlen = atoi(argv[5]);
    
    double entropy_threshold = 0.33;
    if (argc >= 7) {
        entropy_threshold = atof(argv[6]);
    }
    
    double p_value = 0.01;
    if (argc >= 8) {
        p_value = atof(argv[7]);
    }

    size_t maximum_centers = 4;
    if (argc >= 9) {
        maximum_centers = atoi(argv[8]);
    }
    double zvalue = 10.0;
    if (argc >= 10) {
        zvalue = atof(argv[9]);
    }
    bool pool = false;
    if (argc >= 11) {
        pool = atoi(argv[10]);
    }
    drive(sequencefile,
          freq_cutoff,
          error_rate,
          seedlen,
          outprefix,
          entropy_threshold,
          p_value,
          maximum_centers,
          zvalue,
          pool
          );
    delete t;
    return 0;
}


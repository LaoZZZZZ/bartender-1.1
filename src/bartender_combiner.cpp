//
//  bartender_combiner.cpp
//  barcode_project
//
//  Created by luzhao on 2/3/16.
//  Copyright © 2016 luzhao. All rights reserved.
//

#include <stdio.h>

#include "barcodecluster.hpp"
#include "clusteroutput.h"
#include "clustercenterlinkgenerator.h"
#include "errorrateestimator.h"
#include "multipletimepointsprocessor.h"
#include "csvoutput.h"
#include "mergebycenters.h"
#include "centerrecalibrator.h"


#include <iostream>
#include <list>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;
using namespace barcodeSpace;


void drive(const vector<pair<string, string>>& cluster_files,
           double entropy_threshold_for_error,
           size_t cluster_size_threshold_for_error,
           size_t number_barcode_for_error_estimator,
           string outputprefix,
           size_t csize_filter) {
    if (cluster_files.empty()) {
        std::cerr << "There is not cluster results to be combined! Exit the program." << std::endl;
        exit(0);
    }
    std::shared_ptr<ErrorRateEstimator> error_estimator(new ErrorRateEstimator(entropy_threshold_for_error,
                                                                               cluster_size_threshold_for_error,
                                                                               number_barcode_for_error_estimator));
    std::shared_ptr<ClusterCenterLinkGenerator> link_generator(new ClusterCenterLinkGenerator());

    std::shared_ptr<MergeByCenters> center_merger(new MergeByCenters());
    
    MultipleTimePointsProcessor processor(cluster_files, error_estimator, link_generator, center_merger, csize_filter);
    processor.process();
    const unordered_map<int, list<shared_ptr<BarcodeCluster>>>& result_clusters = processor.result();
    const unordered_map<int, vector<double>>& combined_error = processor.compoundErrorRate();
    const unordered_map<int, vector<double>>& original_error = processor.singleErrorRate();
    list<shared_ptr<BarcodeCluster>> clusters;
    int frequency_number_column = 0;
    for (const auto& batch : result_clusters) {
        clusters.insert(clusters.end(), batch.second.begin(), batch.second.end());
        if (!batch.second.empty()){
            frequency_number_column = max(frequency_number_column, batch.first);
        }
    }
    cout << clusters.size() << endl;
    cout << "Starting to dump the result to " << outputprefix << endl;
    ClusterOutput dumper(outputprefix);
    dumper.WriteToFile(clusters,frequency_number_column);
    
    vector<double> example;
    for (const auto& p : combined_error) {
        example = p.second;
    }
    list<string> header;
    header.push_back("Barcode_length");
    stringstream ss;
    for (size_t i = 0; i < example.size(); ++i) {
        ss.str("");
        ss << "time_point_" << example.size() - i;
        header.push_back(ss.str());
    }
    CSVOutput<double> error_dumper(outputprefix + "_combined.csv");
    for (const auto& p : combined_error) {
        list<double> row;
        row.push_back(p.first);
        row.insert(row.end(), p.second.begin(), p.second.end());
        error_dumper.Write(row);
    }
    error_dumper.Close();
    
    CSVOutput<double> single_error_dumper(outputprefix + "_single.csv");
    for (const auto& p : original_error) {
        list<double> row;
        row.push_back(p.first);
        row.insert(row.end(), p.second.begin(), p.second.end());
        single_error_dumper.Write(row);
    }
    single_error_dumper.Close();
}


int main(int argc, char* argv[]) {
    
    if (argc <= 5 || argc % 2 != 1) {
        cout << "Please check the input parameters!" << endl;
    }
    
    //1. Size cutoff.
    size_t csize_filter = atoi(argv[1]);
    
    //2. output prefix
    string outprefix = string(argv[2]);
    
    //3. the input files
    vector<pair<string, string>> cluster_results;
    int index = 3;
    while (index < argc) {
        string cluster_result = string(argv[index]);
        ++index;
        assert(index < argc);
        string frequency_result = string(argv[index]);
        cluster_results.push_back({cluster_result, frequency_result});
        index += 1;
    }
    //1. Entropy for the error estimation.
    
    double entropy_for_error = 0.33;
    
    //2. Cluster size for error estimation
    size_t cluster_size = 20;
    
    //3. Number of barcodes that needs to be used for error estimation.
    size_t barcode_num = 1000000;


    drive(cluster_results,
          entropy_for_error,
          cluster_size,
          barcode_num,
          outprefix,
          csize_filter);
    return 0;
}


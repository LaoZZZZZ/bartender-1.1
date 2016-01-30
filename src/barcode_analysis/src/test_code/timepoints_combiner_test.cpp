#include "../cluster.h"
#include "../clusteroutput.h"
#include "../clustercenterlinkgenerator.h"
#include "../errorrateestimator.h"
#include "../multipletimepointsprocessor.h"
#include "../csvoutput.h"
#include "../mergebycenters.h"

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
           size_t maximum_centers,
           double mixture_entropy_threshold,
           double p_value,
           string outputprefix,
           size_t csize_filter) {
    if (cluster_files.empty()) {
        std::cerr << "There is not cluster results to be combined! Exit the program." << std::endl;
        exit(0);
    }
    std::shared_ptr<ErrorRateEstimator> error_estimator(new ErrorRateEstimator(entropy_threshold_for_error,
                                                                               cluster_size_threshold_for_error,
                                                                               number_barcode_for_error_estimator));
    std::shared_ptr<ClusterCenterLinkGenerator> link_generator(new ClusterCenterLinkGenerator(mixture_entropy_threshold,
                                                                                              maximum_centers,
                                                                                              p_value,
                                                                                              0.3));

    std::shared_ptr<MergeByCenters> center_merger(new MergeByCenters(mixture_entropy_threshold,
                                                                     maximum_centers,
                                                                     p_value,
                                                                     0.3));
    MultipleTimePointsProcessor processor(cluster_files, error_estimator, link_generator, center_merger, csize_filter);
    processor.process();
    const unordered_map<int, list<shared_ptr<cluster>>>& result_clusters = processor.result();
    const unordered_map<int, vector<double>>& combined_error = processor.compoundErrorRate();
    const unordered_map<int, vector<double>>& original_error = processor.singleErrorRate();
    list<shared_ptr<cluster>> clusters;
    int frequency_number_column = 0;
    for (const auto& batch : result_clusters) {
        clusters.insert(clusters.end(), batch.second.begin(), batch.second.end());
	if (!batch.second.empty())
		frequency_number_column = max(frequency_number_column, batch.first);
    }
    cout << clusters.size() << endl;
    ClusterOutput dumper(outputprefix, true);
    dumper.WriteToFile(clusters, cluster_files.size() + 4 , frequency_number_column + 1);
	
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
    CSVOutput error_dumper(outputprefix + "_combined.csv", header);
    for (const auto& p : combined_error) {
        list<kmer> row;
        row.push_back(p.first);
        row.insert(row.end(), p.second.begin(), p.second.end());
        error_dumper.Write(row);
    }
    error_dumper.Close();

    CSVOutput single_error_dumper(outputprefix + "_single.csv", header);
    for (const auto& p : original_error) {
        list<kmer> row;
        row.push_back(p.first);
        row.insert(row.end(), p.second.begin(), p.second.end());
        single_error_dumper.Write(row);
    }
    single_error_dumper.Close();
}


int main(int argc, char* argv[]) {
    assert(argc >= 8);

    //1. Entropy for the error estimation.
    double entropy_for_error = atof(argv[1]);

    //2. Cluster size for error estimation
    size_t cluster_size = atoi(argv[2]);

    //3. Number of barcodes that needs to be used for error estimation.
    size_t barcode_num = atoi(argv[3]);

    //4. The maximum center that a cluster possibly has.
    size_t center_number = atoi(argv[4]);

    //5. Entropy for center calibration.
    double entropy_for_center = atof(argv[5]);

    //6. p-value for testing if a mixture position has potential multiple centers
    double p_value = atof(argv[6]);


    size_t csize_filter = atoi(argv[7]);

    //8. output prefix
    string outprefix = string(argv[8]);
    
    //9. the filter thresdhold of cluster size
    vector<pair<string, string>> cluster_results;
    int index = 9;
    while (index < argc) {
        string cluster_result = string(argv[index]);
        ++index;
        assert(index < argc);
        string frequency_result = string(argv[index]);
        cluster_results.push_back({cluster_result, frequency_result});
        index += 1;
    }
    drive(cluster_results,entropy_for_error,cluster_size,barcode_num,center_number,entropy_for_center,p_value, outprefix, csize_filter);
    return 0;
}


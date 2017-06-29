#include <iostream>
#include <boost/regex.hpp>
#include <string>
#include <memory>
#include <algorithm>
#include "../clusterpipeline.h"
#include "../pattern.h"
#include "../timer.h"
#include "../clusteralgorithm.h"
#include "../barcodeextractor.h"
#include "../clusteroutput.h"
#include "../meansequentialestimator.h"
#include "../singleendbarcodeprocessor.h"
#include "../barcodecounter.h"
#include "../formats.h"
#include "../testSimulation.h"
#include "../util.h"

#include "../errorrateestimator.h"
#include "../clusterpruner.hpp"

using namespace std;
using boost::regex;
using std::unique_ptr;
using barcodeSpace::file_format;
using barcodeSpace::clusterPipline;
using barcodeSpace::ClusterOutput;
using barcodeSpace::cluster;
using barcodeSpace::BarcodeCounter;
using barcodeSpace::SingleEndBarcodeProcessor;
using barcodeSpace::loadData;
using barcodeSpace::loadDataFromCSV;
using barcodeSpace::outputPositionWeightMatrix;
using barcodeSpace::ErrorRateEstimator;
using barcodeSpace::ClusterPruner;
using barcodeSpace::MeanSequentialEstimator;
using namespace barcodeSpace;
// The whole clustering process.
// 1. Loads the read from the sequence file and filter out those unqualified read
// based on the constraints on the barcode region.
// 2. Clusters those valid barcodes using the first k basepair.
// 3. Recursively Clusters those initial clusters based the second k basepair until
// the number of cluster does not change or change is very small.


int main(int argc,char* argv[])
{

  
  
  //string simulatedBarcode("/home/laozzzzz/Documents/barcode_single_end/simulation_data/Simulated_barcode_freq.txt");
  //string outprefix("/home/laozzzzz/Documents/barcode_single_end/simulation_data/clusters");

  string simulatedBarcode(argv[1]);
  string outprefix(argv[2]);
  string outfile = outprefix;
  outfile += ".csv";
  barcodeFreqTable table;
  std::cout<< "loading the simulation barcode!" << std::endl;
  vector<unordered_map<kmer, std::list<string>>> b2sequence;
  loadDataFromCSV(simulatedBarcode,table, b2sequence);
  std::cout << "Finished loading. table size: " << table.size() <<  std::endl;
  size_t start = 0;
  size_t span = 10;
  size_t klen = 40;
  //double zvalue = 1.64;
  double zvalue = atof(argv[3]);
  TESTSTRATEGY pool = static_cast<TESTSTRATEGY>(atoi(argv[4]));
  //size_t cutoff = atoi(argv[6]);
  size_t cutoff = 1;
  double error_rate = 0.02;
  double stoppingThres = 0.01;
  //size_t distance_thres = 3;
  //size_t distance_thres = atoi(argv[5]);
  size_t distance_thres = 2;
  barcodeSpace::CLUSTERTYPE type = barcodeSpace::CLUSTERTYPE::DICTATOR;
  cout << "Initialize pipeline" << std::endl;
  clusterPipline* pipe = new clusterPipline(start, span, klen, error_rate, zvalue, pool, stoppingThres);
  cout << "Finished initialization " << std::endl;
  std::shared_ptr<clusterPipline> pPipe(pipe);
  std::cout << "Start to cluster" << std::endl;
  pPipe->clusterDrive(table);
  std::cout << "finished clustering" << std::endl;
  const list<std::shared_ptr<cluster>>& clusters = pPipe->clusters();
  cout<<clusters.size()<<endl;
  cout << "Start to estimate the sequence error " << endl;
  MeanSequentialEstimator mean_estimator;
  for (const auto& c : clusters) {
	mean_estimator.Add(c->size());
  }
  std::shared_ptr<ErrorRateEstimator> error_estimator(new ErrorRateEstimator(0.3,
                                                                             50,//static_cast<int>(mean_estimator.GetMean()),
                                                                             100000));
  error_estimator->Estimate(clusters, true);
  cout << "Estimated sequence error rate: " << error_estimator->ErrorRate() << endl;
  cout << "Starting to merge clusters by centers" << endl;
  /*
  MergeByCenters merger(0.33,
                        4,
                        0.01,
                        0.3);
  merger.merge(clusters, error_estimator->Entropies());
  
  cout << "Finished merging" << endl;
  */
  ClusterPruner pruner(0.48,
		       4,
		       4.0,
		       0.1,
		       cutoff,
		       0.02);
  pruner.prune(clusters); 
  const list<std::shared_ptr<cluster>>& ultimate_clusters = pruner.prunedClusters();
  cout << ultimate_clusters.size() << endl;
  error_estimator->Estimate(ultimate_clusters, false);
  cout << "Error rate after pruning: " << error_estimator->ErrorRate() << endl; 
  ClusterOutput output_eng(outprefix);
  output_eng.WriteToFile(ultimate_clusters,b2sequence);
  
  return 0;
}

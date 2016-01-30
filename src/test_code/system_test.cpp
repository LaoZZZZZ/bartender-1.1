#include <iostream>
#include <boost/regex.hpp>
#include <string>
#include <memory>
#include <algorithm>
#include "../clusterpipeline.h"
#include "../pattern.h"
#include "../timer.h"
#include "../clusteralgorithm.h"
#include "../binaryoutfilebuf.h"
#include "../binaryinfilebuf.h"
#include "../barcodeextractor.h"
#include "../testSimulation.h"
#include "../clusteroutput.h"
#include "../singleendbarcodeprocessor.h"
#include "../barcodecounter.h"
#include "../testSimulation.h"
#include "../formats.h"
#include "../util.h"

using boost::regex;
using std::unique_ptr;
using std::list;
using std::vector;
using std::string;

using barcodeSpace::Sequence;
using barcodeSpace::file_format;
using barcodeSpace::clusterPipline;
using barcodeSpace::ClusterOutput;
using barcodeSpace::cluster;
using barcodeSpace::BarcodeCounter;
using barcodeSpace::SingleEndBarcodeProcessor;
using barcodeSpace::loadData;
using barcodeSpace::outputPositionWeightMatrix;
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

void drive(std::string seqfile,  // origina read file
           double qualityThres,  // quality threshold
           size_t freq_cutoff,   // frequency cutoff
           size_t seedlen,      // seed len
           std::string outprefix,
           const boost::regex& pattern,
           bool head = true){
    Timer* t = new realTimer(cout);


    //1. first argument is the sequence file

    //2. file format is fastq
    file_format format = file_format::FASTQ;
    //3. quality threshold
    SingleEndBarcodeProcessor single_end_processor(seqfile,
                                                   pattern,
                                                   format,
                                                   qualityThres);
    single_end_processor.Extract();
    const unordered_map<size_t, BarcodeCounter*>& stats = single_end_processor.AllBarcodeTables();

    std::list<std::shared_ptr<cluster>> clusters;
    size_t frequency_number_column = 0;
    for(auto iter = stats.begin(); iter != stats.end(); iter++){
        size_t start = 0;
        size_t klen = iter->first*2;
        if(seedlen*2 > klen)
            seedlen = 1;
        clusterPipline* pipe = new clusterPipline(start,seedlen*2,klen,freq_cutoff);
        std::shared_ptr<clusterPipline> pPipe(pipe);
        pPipe->clusterDrive(iter->second->BarcodeTable());
        const std::list<std::shared_ptr<cluster>>& temp_clusters = pPipe->clusters();
        clusters.insert(clusters.end(), temp_clusters.begin(), temp_clusters.end());

        //std::sort(result.begin(), result.end(), CompareObject());
        cout<<"There are totally " << temp_clusters.size() <<" clusters with length " << iter->first <<endl;
	if (!temp_clusters.empty())
        	frequency_number_column = max(frequency_number_column, iter->first);	
        
    }
    if (!clusters.empty()) {
        cout << "There are totally " << clusters.size() << " clusters!" << endl;
       	cout<<"Starting to dump the cluster to file "<< outprefix <<endl;
       	ClusterOutput out(outprefix, head);
        std::cout << frequency_number_column << std::endl;
       	// Dumps the cluster information.
       	out.WriteToFile(clusters, 5, frequency_number_column + 1);
    }

    // Dumps the frequency information to the file
   // outputPositionWeightMatrix(outprefix + ".bp", result);
    delete t;

}
int main(int argc,char* argv[])
{

    boost::regex pattern("(.ACC|T.CC|TA.C|TAC.)(A|T|C|G|N){4,7}(AA)(A|T|C|G|N){4,7}(AA)(A|T|C|G|N){4,7}(TT)(A|T|C|G|N){4,7}(.TAA|A.AA|AT.A|ATA.)",boost::regex::ECMAScript);

    assert(argc >= 3);
    //1. first argument is the sequence file
    string sequencefile(argv[1]);
    //2. out put file
    string outprefix(argv[2]);
    size_t freq_cutoff = 10;
    if(argc >= 4)
        freq_cutoff = atoi(argv[3]);
    double qualityThres = 30;
    if(argc >= 5)
    //3. quality threshold
        qualityThres = (atoi(argv[4]));
    size_t seedlen = 5;
    if(argc >= 6)
        seedlen = atoi(argv[5]);
    drive(sequencefile,
          qualityThres,
          freq_cutoff,
          seedlen,
          outprefix,
          pattern);
    return 0;
}


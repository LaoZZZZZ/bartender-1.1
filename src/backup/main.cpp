#include <iostream>
#include <boost/regex.hpp>
#include <string>
#include "clusterpipeline.h"
#include "pattern.h"
#include "timer.h"
#include "clusteralgorithm.h"
#include "binaryoutfilebuf.h"
#include "binaryinfilebuf.h"
#include "barcodeextractor.h"
#include "testSimulation.h"
#include "clusteroutput.h"
using namespace std;
using namespace boost;
using namespace prefixMatching;

// whole process
void drive(std::string seqfile,  // origina read file
           double qualityThres,  // quality threshold
           size_t freq_cutoff,   // frequency cutoff
           size_t seedlen,      // seed len
           std::string outprefix){
    Timer* t = new realTimer(cout);

    boost::regex pattern("(.ACC|T.CC|TA.C|TAC.)(A|T|C|G|N){4,7}(AA)(A|T|C|G|N){4,7}(AA)(A|T|C|G|N){4,7}(TT)(A|T|C|G|N){4,7}(.TAA|A.AA|AT.A|ATA.)",boost::regex::ECMAScript);


    //1. first argument is the sequence file

    //2. file format is fastq
    file_format format = FASTQ;
    //3. quality threshold
    barcodeExtractor* temp = new barcodeExtractor(seqfile,pattern,format,qualityThres);
    std::shared_ptr<barcodeExtractor> counter(temp);
    const unordered_map<size_t,std::shared_ptr<barcodeCounting>>& stats = counter->bins();
    list<std::shared_ptr<cluster>>  result({});
    for(auto iter = stats.begin(); iter != stats.end(); iter++){

        size_t start = 0;
        size_t klen = iter->first*2;
        if(seedlen*2 > klen)
            seedlen = 1;
        clusterPipline* pipe = new clusterPipline(start,seedlen*2,klen,freq_cutoff);
        std::shared_ptr<clusterPipline> pPipe(pipe);
        pPipe->clusterDrive(iter->second->barcodes());
        result.insert(result.end(),pPipe->clusters().begin(),pPipe->clusters().end());

    }
    cout<<"There are totally " << result.size()<<" clusters" <<endl;
    string outfile(outprefix);
    outfile += ".csv";
    cout<<"starting to dump the cluster to file "<< outfile<<endl;
    std::vector<std::string> column({"colum1"});
    clusterOutput out(outfile,column);
    out.write(result);
    delete t;
}
int main(int argc,char* argv[])
{



    string simulatedBarcode("/home/laozzzzz/barcode_project/data/Simulated_barcode_freq.txt");
    string outfile("/home/laozzzzz/barcode_project/data/clusters.csv");
    kmers_sparseHash<kmer,freq> table;
    loadData(simulatedBarcode,table);
    //cout<<table.size()<<endl
    size_t start = 0;
    size_t span = 10;
    size_t klen = 40;
    size_t cutoff = 10;
    CLUSTERTYPE type = DICTATOR;
    clusterPipline* pipe = new clusterPipline(start,span,klen,cutoff,type);
    std::shared_ptr<clusterPipline> pPipe(pipe);
    pPipe->clusterDrive(table);

    outputCluster(outfile,pPipe->clusters());
    cout<<pPipe->clusters().size()<<endl;
    /*
    string truth = "/home/laozzzzz/barcode_project/data/True_barcode_freq.txt";
    kmers_sparseHash<kmer,freq> truecluster;
    loadData(truth,truecluster);
    cout<<truecluster.size()<<endl;
    */
/*
    fstream h("text.csv",fstream::in);
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
    drive(sequencefile,qualityThres,freq_cutoff,seedlen,outprefix);
*/
    return 0;
}


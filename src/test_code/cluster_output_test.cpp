#include <iostream>
#include <cassert>
#include <memory>
#include <string>
#include <vector>
#include <array>
#include "../dictatorcenter.h"
#include "../kmers_bitwisetransform.h"
#include "../typdefine.h"
#include "../cluster.h"
#include "../dictatorcluster.h"
#include "../clusteroutput.h"

using std::string;
using std::endl;
using std::cout;
using std::shared_ptr;
using std::vector;
using std::array;

using barcodeSpace::DictatorCenter;
using barcodeSpace::kmersBitwiseTransform;
using barcodeSpace::cluster;
using barcodeSpace::dictatorCluster;
using barcodeSpace::ClusterOutput;

int main() {
    std::list<std::shared_ptr<cluster>> clusters;
    string seq("AGACT"); 
    kmer k;
    freq f = 5;
    kmersBitwiseTransform::getInstance()->seq_2_bitwise(seq, k, 5);    
    const std::shared_ptr<DictatorCenter> center(new DictatorCenter(k,5));
    std::shared_ptr<cluster> first_cluster(new dictatorCluster(center,{f}));
    clusters.push_back(first_cluster);
    
    // Create another cluster and merge with the first one.
    string another_seq("AGCCG");
    kmer another_k;
    freq another_f = 3;
    kmersBitwiseTransform::getInstance()->seq_2_bitwise(another_seq, another_k, 5);    
    const std::shared_ptr<DictatorCenter> another_center(new DictatorCenter(another_k, 5));
    std::shared_ptr<cluster> second_cluster(new dictatorCluster(another_center, {another_f}));
    clusters.push_back(second_cluster);
 
    string outprefix = "/home/laozzzzz/Documents/barcode_single_end/test_code/test";
    bool head = true;
    ClusterOutput output_eng(outprefix, head);
    output_eng.WriteToFile(clusters);
    
    return 0;

}

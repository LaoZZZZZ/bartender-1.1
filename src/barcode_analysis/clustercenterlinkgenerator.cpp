#include "clustercenterlinkgenerator.h"

#include <iostream>
#include <string>
#include <vector>
#include <array>
#include "kmers_bitwisetransform.h"

using namespace std;
namespace barcodeSpace {

void ClusterCenterLinkGenerator::printFrequency(const vector<array<int,4>>& frequencies) {
    cout << "*****************************" << endl;

    for(const auto& bp : frequencies) {
        cout << '{';
        for (const auto& f : bp) {
            cout << f << ',';
            }
        cout << '}' << ',' <<endl;
    }
    cout << "*****************************" << endl;

}
ClusterCenterLinkGenerator::ClusterCenterLinkGenerator(double entropy_threshold,
                                                       int maximum_centers,
                                                       double p_value,
                                                       double error_rate) :
   _recalibrator(entropy_threshold, maximum_centers, p_value, error_rate)
{
}
void ClusterCenterLinkGenerator::generateCenters() {
    std::vector<kmer> centers;
    auto entropy_iter = _entropies->begin();
    for (const auto& c : *_clusters) {
        _recalibrator.IdentifyCenters(c->bpFrequency(), *entropy_iter, &centers);
        _center_cluster_link->addPair(c, centers);
        ++entropy_iter;
        centers.clear();
    }
}
}   // namespace barcodeSpace

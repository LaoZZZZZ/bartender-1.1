#include <algorithm>
#include <iostream>
#include <cassert>
#include <list>
#include <memory>
#include <string>
#include <vector>
#include <array>
#include "../dictatorcenter.h"
#include "../kmers_bitwisetransform.h"
#include "../typdefine.h"
#include "../cluster.h"
#include "../dictatorcluster.h"
#include "../centerclustermapper.h"
#include "../clustercenterlinkgenerator.h"
#include "../errorrateestimator.h"
#include "../timepointsmerger.h"
using std::string;
using std::endl;
using std::cout;
using std::shared_ptr;
using std::vector;
using std::array;
using std::list;
using barcodeSpace::DictatorCenter;
using barcodeSpace::kmersBitwiseTransform;
using barcodeSpace::cluster;
using barcodeSpace::dictatorCluster;
using barcodeSpace::CenterClusterMapper;
using barcodeSpace::ClusterCenterLinkGenerator;
using barcodeSpace::ErrorRateEstimator;
using barcodeSpace::TimePointsMerger;


 
void printFrequency(const vector<array<int,4>>& frequencies) {
    cout << "*****************************" << endl;
    for(const auto& bp : frequencies) {
	for (const auto& f : bp) {
		cout << f << '\t';
        }
	cout << endl;
    } 
    cout << "*****************************" << endl;
 
}
void printCenter(const vector<kmer>& centers) {
    for(const auto& c : centers) {
	string temp;
        kmersBitwiseTransform::getInstance()->bitwise_2_seq(c, temp, 5);
        cout << temp << endl;
    }   
}

void printCenter(const list<kmer>& centers) {
    for(const auto& c : centers) {
	string temp;
        kmersBitwiseTransform::getInstance()->bitwise_2_seq(c, temp, 5);
        cout << temp << endl;
    }   
}
ClusterCenterLinkGenerator link_generator(0.33, 4, 0.05, 0.1);
ErrorRateEstimator error_estimator(0.9, 5, 20);  

std::shared_ptr<CenterClusterMapper> getlink(list<shared_ptr<cluster>>& clusters) {
	error_estimator.Estimate(clusters);
        link_generator.Generate(clusters, error_estimator.Entropies());
	return link_generator.CenterClusterLink();
}
list<shared_ptr<cluster>> testCase(list<shared_ptr<cluster>>& clusters_1,
				   list<shared_ptr<cluster>>& clusters_2,
				   int time_points) {
	std::shared_ptr<CenterClusterMapper> link1 = getlink(clusters_1);
	std::shared_ptr<CenterClusterMapper> link2 = getlink(clusters_2);
	TimePointsMerger merger(link1, link2, 5, time_points);
	merger.merge();
	return merger.mergedClusters();
}
int main() {
    list<shared_ptr<cluster>> clusters_1;
    list<shared_ptr<cluster>> clusters_2;
    ////////////////////////////////////////////////////////
    /////////////////////Empty clusters case ///////////////
    ////////////////////////////////////////////////////////
    list<shared_ptr<cluster>> clusters = testCase(clusters_1, clusters_2, 0);
    assert(clusters.empty());
    ////////////////////////////////////////////////////////
    /////////////////////each has one cluster////////////////////////
    ///////////////////////////////////////////////////////
    string seq("AAAAA"); 
    kmer k;
    freq f = 5;
    kmersBitwiseTransform::getInstance()->seq_2_bitwise(seq, k, 5);    
    const std::shared_ptr<DictatorCenter> center(new DictatorCenter(k,5));
    std::shared_ptr<cluster> first_cluster(new dictatorCluster(center,{f}));
    clusters_1.push_back(first_cluster);    
    seq = "AAAAT";
    k = 0;
    kmersBitwiseTransform::getInstance()->seq_2_bitwise(seq, k, 5);    
    const std::shared_ptr<DictatorCenter> second_center(new DictatorCenter(k,5));
 
    const vector<array<int,4>> frequency_table_2(
	{{17, 3, 0, 0},
	{20, 0, 0, 0},
	{20, 0, 0, 0},
	{20, 0, 0, 0},
	{0, 0, 0, 20}});
    std::shared_ptr<cluster> second_cluster(new dictatorCluster(second_center, {20}, frequency_table_2, 2));
    clusters_2.push_back(second_cluster);

    clusters = testCase(clusters_1, clusters_2, 1);
    vector<array<int,4>> frequency_table = clusters.front()->bpFrequency();
    error_estimator.Estimate(clusters);
    link_generator.Generate(clusters, error_estimator.Entropies());
    shared_ptr<CenterClusterMapper> link = link_generator.CenterClusterLink();
    cout << clusters.size() << endl;
    for (const auto& c : clusters) {
    	list<kmer> mycenters = link->myCenter(c->ClusterID());
        printFrequency(c->bpFrequency());
    	printCenter(mycenters); 
    }
     ////////////////////////////////////////////////////////
    /////////////////////// Three time points ///////////////////
    ///////////////////////////////////////////////////////
    clusters_2.clear();
    seq = "AAAAC";
    k = 0;
    kmersBitwiseTransform::getInstance()->seq_2_bitwise(seq, k, 5);    
    const std::shared_ptr<DictatorCenter> third_center(new DictatorCenter(k,5));
    const vector<array<int,4>> frequency_table_3(
	{{14, 3, 3, 0},
	{20, 0, 0, 0},
	{20, 0, 0, 0},
	{20, 0, 0, 0},
	{0, 20, 0,0 }});

    std::shared_ptr<cluster> third_cluster(new dictatorCluster(third_center, {20}, frequency_table_3, 3));
    clusters_2.push_back(third_cluster);
    clusters = testCase(clusters, clusters_2, 2);
    cout << clusters.size() << endl;
    error_estimator.Estimate(clusters);
    link_generator.Generate(clusters, error_estimator.Entropies());
    link = link_generator.CenterClusterLink();
    list<kmer> mycenters;
    for (const auto& c : clusters) {
    	mycenters = link->myCenter(c->ClusterID());
        printFrequency(c->bpFrequency());
    	printCenter(mycenters); 
    }
  //  printCenter(centers);
    ////////////////////////////////////////////////////////
    /////////////////////// Four time points ///////////////////
    ///////////////////////////////////////////////////////
    clusters_2.clear();
    seq = "ATAAG";
    k = 0;
    kmersBitwiseTransform::getInstance()->seq_2_bitwise(seq, k, 5);    
    const std::shared_ptr<DictatorCenter> four_center(new DictatorCenter(k,5));
    const vector<array<int,4>> frequency_table_4(
	{{17, 3, 0, 0},
	{0, 0, 0, 20},
	{20, 0, 0, 0},
	{20, 0, 0, 0},
	{0, 3, 17,0 }});

    
    std::shared_ptr<cluster> four_cluster(new dictatorCluster(four_center, {20}, frequency_table_4, 4));
    clusters_2.push_back(four_cluster);

    clusters = testCase(clusters, clusters_2, 3);
    error_estimator.Estimate(clusters);
    link_generator.Generate(clusters, error_estimator.Entropies());
    link = link_generator.CenterClusterLink();
    cout << clusters.size() << endl;
    for (const auto& c : clusters) {
	cout << c->columns().size() << endl;
	vector<array<int,4>> frequency_table = c->bpFrequency();
    	printFrequency(frequency_table);
    	mycenters = link->myCenter(c->ClusterID());
    	printCenter(mycenters); 
 
    }
    ////////////////////////////////////////////////////////
    /////////////////////// Fifth time point ///////////////////
    ///////////////////////////////////////////////////////
    clusters_2.clear();
    seq = "ATAAG";
    k = 0;
    kmersBitwiseTransform::getInstance()->seq_2_bitwise(seq, k, 5);    
    const std::shared_ptr<DictatorCenter> five_center(new DictatorCenter(k,5));
    const vector<array<int,4>> frequency_table_5(
	{{17, 3, 0, 0},
	{0, 0, 0, 20},
	{20, 0, 0, 0},
	{20, 0, 0, 0},
	{0, 3, 17,0 }});

    
    std::shared_ptr<cluster> five_cluster(new dictatorCluster(five_center, {20}, frequency_table_5, 4));
    clusters_2.push_back(five_cluster);

    clusters = testCase(clusters, clusters_2, 4);
    error_estimator.Estimate(clusters);
    link_generator.Generate(clusters, error_estimator.Entropies());
    link = link_generator.CenterClusterLink();
    cout << clusters.size() << endl;
    for (const auto& c : clusters) {
	cout << c->columns().size() << endl;
	vector<array<int,4>> frequency_table = c->bpFrequency();
    	printFrequency(frequency_table);
    	mycenters = link->myCenter(c->ClusterID());
    	printCenter(mycenters); 
 
    }
        return 0;
}

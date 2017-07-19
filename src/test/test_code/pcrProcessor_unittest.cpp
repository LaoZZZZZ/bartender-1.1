#include "../../pcrprocessor.hpp"
#include "../../barcodecluster.hpp"
#include "gtest/gtest.h"

#include <algorithm>
#include <numeric>
#include <iostream>
#include <cassert>
#include <memory>
#include <string>
#include <vector>
#include <array>
using namespace std;
using namespace barcodeSpace;

class PcrProcessorTest : public ::testing::Test {
protected:
    std::shared_ptr<BarcodePool> poolPtr;
    std::shared_ptr<PCRProcessor> pcrDealerPtr;

    virtual void SetUp() {
        unordered_map<string, vector<string>> raw_barcode(
            {{"AAAAA", {"AAA","AAC", "AAC", "ATC", "ATC"}},
             {"AAAAT", {"AAA", "AAC"}},
             {"AACAT", {"TCA", "TCA", "TCA", "TTC"}},
             {"TAAAA", {"TCA", "TCA", "TCA", "TTC", "TTT"}},
             {"TCCCC", {"ATC", "TCA", "TCA", "TTT"}}
            });
        BarcodePool::createInstance(raw_barcode);
        poolPtr = BarcodePool::getAutoInstance();
        BarcodeCluster::setBarcodePool(poolPtr);
        pcrDealerPtr.reset(new PCRProcessor());
    }
};

TEST_F(PcrProcessorTest, withPcrDup) {
    list<std::shared_ptr<BarcodeCluster>> clusters;
    
    ////////////////////////////////////////////////////////
    /////////////////////One center////////////////////////
    ///////////////////////////////////////////////////////
    std::shared_ptr<BarcodeCluster> first_cluster(new BarcodeCluster(poolPtr->barcodeIndex("AAAAA")));
    std::shared_ptr<BarcodeCluster> second_cluster(new BarcodeCluster(poolPtr->barcodeIndex("AAAAT")));
    std::shared_ptr<BarcodeCluster> third_cluster(new BarcodeCluster(poolPtr->barcodeIndex("TAAAA")));
    std::shared_ptr<BarcodeCluster> fourth_cluster(new BarcodeCluster(poolPtr->barcodeIndex("AACAT")));
    std::shared_ptr<BarcodeCluster> fifth_cluster(new BarcodeCluster(poolPtr->barcodeIndex("TCCCC")));

    // merge clusters with AAAAA, AAAAT, TAAAA
    first_cluster->merge(second_cluster);
    first_cluster->merge(third_cluster);
    clusters.push_back(first_cluster);
    clusters.push_back(fourth_cluster);
    clusters.push_back(fifth_cluster);

    pcrDealerPtr->process(clusters, poolPtr);
    ASSERT_EQ(6, clusters.front()->size());
    ASSERT_EQ(3, poolPtr->primers(poolPtr->barcodeIndex("AAAAA")).size());
    ASSERT_EQ(0, poolPtr->primers(poolPtr->barcodeIndex("AAAAT")).size());
    ASSERT_EQ(3, poolPtr->primers(poolPtr->barcodeIndex("TAAAA")).size());
    ASSERT_EQ(2, poolPtr->primers(poolPtr->barcodeIndex("AACAT")).size());
    ASSERT_EQ(3, poolPtr->primers(poolPtr->barcodeIndex("TCCCC")).size());
}

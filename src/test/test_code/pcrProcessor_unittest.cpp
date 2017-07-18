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
             {"TAAAA", {"TCA", "TCA", "TCA", "TTC", "TTT"}}});
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
    std::shared_ptr<BarcodeCluster> first_cluster(new BarcodeCluster(0));
    std::shared_ptr<BarcodeCluster> second_cluster(new BarcodeCluster(1));
    std::shared_ptr<BarcodeCluster> third_cluster(new BarcodeCluster(2));
    std::shared_ptr<BarcodeCluster> four_cluster(new BarcodeCluster(3));
    first_cluster->merge(second_cluster);
    first_cluster->merge(third_cluster);
    first_cluster->merge(four_cluster);
    clusters.push_back(first_cluster);
    pcrDealerPtr->process(clusters, poolPtr);
    ASSERT_EQ(6, clusters.front()->size());
    ASSERT_EQ(3, poolPtr->primers(0).size());
    ASSERT_EQ(0, poolPtr->primers(1).size());
    ASSERT_EQ(0, poolPtr->primers(2).size());
    ASSERT_EQ(3, poolPtr->primers(3).size());
}

#include "../../clustersbucketer.hpp"
#include "../../barcodepool.hpp"

#include <iostream>
#include <list>
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <math.h>
#include "gtest/gtest.h"
using namespace std;
using namespace barcodeSpace;

class ClusterBucketerTest : public ::testing::Test {
protected:
    std::shared_ptr<ClusterBucketer> clusterBucketerPtr;
    std::list<shared_ptr<BarcodeCluster>> clusters;
    size_t seedsLength = 3U;
    size_t stepSize = 3U;
    vector<int> seeds{0, 1, 2, 3, 4};

    virtual void SetUp() {
        unordered_map<string, vector<string>> raw_barcode(
            {{"AAAAA", {"AAA","AAC", "AAC", "ATC", "ATC"}},
             {"AAAAT", {"AAA", "AAC"}},
             {"AACAT", {"TCA", "TCA", "TCA", "TTC"}},
             {"TAAAA", {"TCA", "TCA", "TCA", "TTC"}}
            });
        BarcodePool::createInstance(raw_barcode);
        std::shared_ptr<BarcodePool> pool = BarcodePool::getAutoInstance();
        BarcodeCluster::setBarcodePool(pool);
        for(size_t index = 0; index < pool->size(); ++ index){
            std::shared_ptr<BarcodeCluster> ptemp(new BarcodeCluster(index));
            clusters.push_back(ptemp);
        }
        ClusterBucketer* bucketer = new ClusterBucketer(seeds, seedsLength, stepSize);
        clusterBucketerPtr.reset(bucketer);
    }
};

TEST_F(ClusterBucketerTest, success) {
    ASSERT_EQ(2, clusterBucketerPtr->round());
    clusterBucketerPtr->shatter(clusters);
    std::vector<ClusterBucketer::CBin> bins = clusterBucketerPtr->Bins();
    ASSERT_EQ(pow(4, seedsLength), bins.size());
    // two clusters are in the first bin in the first round
    ASSERT_EQ(2, bins[0].size());
    // cluster AACAT is in the second bin in first round
    ASSERT_EQ(1, bins[1].size());
    // cluster TAAAA is in the fourth eighth bin in the first round
    ASSERT_EQ(1, bins[48].size());
    ASSERT_FALSE(clusterBucketerPtr->done());
    
    clusterBucketerPtr->shatter(clusters);
    bins = clusterBucketerPtr->Bins();
    // two clusters are in the first bin in the second round
    // they are AAAAA and TAAAA
    ASSERT_EQ(2, bins[0].size());
    // cluster AAAAT is in the fourth bin in second round
    ASSERT_EQ(1, bins[3].size());
    // cluster AACAT is in the 19th bin in the second round.
    ASSERT_EQ(1, bins[19].size());
    ASSERT_TRUE(clusterBucketerPtr->done());
}

TEST_F(ClusterBucketerTest, emptyList) {
    clusters.clear();
    clusterBucketerPtr->shatter(clusters);
    std::vector<ClusterBucketer::CBin> bins = clusterBucketerPtr->Bins();
    ASSERT_EQ(pow(4, seedsLength), bins.size());
    for(const auto& bin : bins) {
        ASSERT_TRUE(bin.empty());
    }
}

TEST_F(ClusterBucketerTest, seedLengthLargerThanStepSize) {
    size_t seedsLength = 3U;
    size_t stepSize = 1U;
    ClusterBucketer* bucketer = new ClusterBucketer(seeds, seedsLength, stepSize);
    clusterBucketerPtr.reset(bucketer);
    ASSERT_EQ(3, clusterBucketerPtr->round());
    clusterBucketerPtr->shatter(clusters);
    std::vector<ClusterBucketer::CBin> bins = clusterBucketerPtr->Bins();
    // two clusters are in the first bin in the first round
    ASSERT_EQ(2, bins[0].size());
    // cluster AACAT is in the second bin in first round
    ASSERT_EQ(1, bins[1].size());
    // cluster TAAAA is in the fourth eighth bin in the first round
    ASSERT_EQ(1, bins[48].size());
    ASSERT_FALSE(clusterBucketerPtr->done());

    clusterBucketerPtr->shatter(clusters);
    bins = clusterBucketerPtr->Bins();
    // two clusters are in the first bin in the second round
    // they are AAAAA and TAAAA, AAAAT
    ASSERT_EQ(3, bins[0].size());
    // cluster AAAAT is in the fourth bin in second round
    ASSERT_EQ(1, bins[4].size());
    ASSERT_FALSE(clusterBucketerPtr->done());
    
    clusterBucketerPtr->shatter(clusters);
    bins = clusterBucketerPtr->Bins();
    // two clusters are in the first bin in the second round
    // they are AAAAA and TAAAA
    ASSERT_EQ(2, bins[0].size());
    // cluster AAAAT is in the fourth bin in second round
    ASSERT_EQ(1, bins[3].size());
    // cluster AACAT is in the 19th bin in the second round.
    ASSERT_EQ(1, bins[19].size());
    ASSERT_TRUE(clusterBucketerPtr->done());
}

TEST_F(ClusterBucketerTest, oneRound) {
    size_t seedsLength = 5U;
    size_t stepSize = 1U;
    ClusterBucketer* bucketer = new ClusterBucketer(seeds, seedsLength, stepSize);
    clusterBucketerPtr.reset(bucketer);
    ASSERT_EQ(1, clusterBucketerPtr->round());
    clusterBucketerPtr->shatter(clusters);
    std::vector<ClusterBucketer::CBin> bins = clusterBucketerPtr->Bins();
    // clusters AAAAA is in the first bin in the first round
    ASSERT_EQ(1, bins[0].size());
    // cluster AAAAT is in the fourth bin in first round
    ASSERT_EQ(1, bins[3].size());
    // cluster TAAAA is in the 768th bin in the first round
    ASSERT_EQ(1, bins[768].size());
    // cluster AACAT is in the 19th bin in the first round.
    ASSERT_EQ(1, bins[19].size());
    ASSERT_TRUE(clusterBucketerPtr->done());
}

TEST_F(ClusterBucketerTest, seedLengthLargerThanSeedsCandidate) {
    size_t seedsLength = 10U;
    size_t stepSize = 1U;
    ClusterBucketer* bucketer = new ClusterBucketer(seeds, seedsLength, stepSize);
    clusterBucketerPtr.reset(bucketer);
    ASSERT_EQ(1, clusterBucketerPtr->getIndexers().size());
    ASSERT_EQ(seeds.size(), clusterBucketerPtr->getIndexers()[0]->seeds().size());
}


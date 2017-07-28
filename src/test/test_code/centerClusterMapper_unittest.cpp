#include "../../centerclustermapper.h"
#include "../../barcodepool.hpp"

#include "gtest/gtest.h"
#include <string>
#include <vector>
#include <stdexcept>
#include <array>

using std::string;
using std::vector;
using std::array;
using std::runtime_error;
using barcodeSpace::CenterClusterMapper;
using barcodeSpace::BarcodeCluster;
using barcodeSpace::BarcodePool;


TEST(CenterClusterMapperTest, empty) {
    CenterClusterMapper centerClusterMapper(10);
    
    ASSERT_FALSE(centerClusterMapper.hasCluster(0));
    ASSERT_FALSE(centerClusterMapper.hasCenter("ACTA"));
    ASSERT_EQ(0, centerClusterMapper.numberOfClusters());
    
    ASSERT_THROW(centerClusterMapper.getClusterByID(10), std::out_of_range);
}

TEST(CenterClusterMapperTest, addAndRemoveCluster) {
    CenterClusterMapper centerClusterMapper(10);
    
    const vector<array<int,4>> frequency_table({{{17, 3, 0, 0}},
        {{20, 0, 0, 0}},
        {{20, 0, 0, 0}},
        {{20, 0, 0, 0}},
        {{0, 0, 0, 20}}});
    std::shared_ptr<BarcodeCluster> barcodeClusterPtr(new BarcodeCluster("ACTAA", frequency_table, 0));
    centerClusterMapper.addPair(barcodeClusterPtr, {"ACTAA", "CCTAA"});
    
    ASSERT_TRUE(centerClusterMapper.hasCluster(0));
    ASSERT_TRUE(centerClusterMapper.hasCenter("ACTAA"));
    ASSERT_TRUE(centerClusterMapper.hasCenter("CCTAA"));
    ASSERT_FALSE(nullptr == centerClusterMapper.getClusterByID(0).get());
    ASSERT_FALSE(nullptr == centerClusterMapper.getClusterByCenter("ACTAA").get());
    ASSERT_FALSE(nullptr == centerClusterMapper.getClusterByCenter("CCTAA").get());
    ASSERT_TRUE(nullptr == centerClusterMapper.getClusterByCenter("AAAAA").get());
    ASSERT_EQ(1, centerClusterMapper.numberOfClusters());

    centerClusterMapper.removeCluster(0);
    ASSERT_FALSE(centerClusterMapper.hasCluster(0));
    ASSERT_FALSE(centerClusterMapper.hasCenter("ACTAA"));
    ASSERT_FALSE(centerClusterMapper.hasCenter("CCTAA"));
    ASSERT_TRUE(nullptr == centerClusterMapper.getClusterByCenter("CCTAA").get());
    ASSERT_TRUE(nullptr == centerClusterMapper.getClusterByCenter("ACTAA").get());
    ASSERT_EQ(0, centerClusterMapper.numberOfClusters());
}

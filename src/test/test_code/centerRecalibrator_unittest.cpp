#include <algorithm>
#include <numeric>
#include <iostream>
#include <cassert>
#include <memory>
#include <string>
#include <vector>
#include <array>
#include "../../centerrecalibrator.h"
#include "gtest/gtest.h"
using namespace std;
using barcodeSpace::CenterRecalibrator;

class CenterCalibratorTest : public ::testing::Test {
protected:
    std::unique_ptr<CenterRecalibrator> centerRecalibratorPtr;
    
    virtual void SetUp() {
        centerRecalibratorPtr.reset(new CenterRecalibrator(0.33, // entropy threshold
                                                           2, // maximum center
                                                           0.05, // p-value
                                                           0.1)); // sequence error
    }
};

TEST_F(CenterCalibratorTest, oneCenter) {
    const vector<array<uint64_t, 4>> frequency_table({{{20, 0, 0, 0}},
        {{0, 0, 20, 0}},
        {{20, 0, 0, 0}},
        {{0, 20, 0, 0}},
        {{0, 0, 0, 20}}});
    vector<string> centers;
    ASSERT_FALSE(centerRecalibratorPtr->IdentifyCenters(frequency_table, &centers));
    ASSERT_EQ(1, centers.size());
    ASSERT_EQ("AGACT", centers.front());
}
 
TEST_F(CenterCalibratorTest, twoCenters) {
    const vector<array<uint64_t, 4>> frequency_table({{{17, 3, 0, 0}},
        {{0, 0, 20, 0}},
        {{20, 0, 0, 0}},
        {{0, 20, 0, 0}},
        {{0, 0, 0, 20}}});
    vector<string> centers;
    ASSERT_FALSE(centerRecalibratorPtr->IdentifyCenters(frequency_table, &centers));
    ASSERT_EQ(2, centers.size());
    std::unordered_set<string> expectedCenters({"AGACT", "CGACT"});
    for (const auto& c : centers) {
        ASSERT_FALSE(expectedCenters.find(c) == expectedCenters.end());
    }
}

TEST_F(CenterCalibratorTest, threeCenters) {
    const vector<array<uint64_t, 4>> frequency_table({
        {{14, 3, 3, 0}},
        {{0, 0, 20, 0}},
        {{20, 0, 0, 0}},
        {{0, 20, 0, 0}},
        {{0, 0, 0, 20}}});
    vector<string> centers;
    ASSERT_TRUE(centerRecalibratorPtr->IdentifyCenters(frequency_table, &centers));
    // it only return 2 centers although there are potential three centers based on the criteria
    ASSERT_EQ(2, centers.size());
    std::unordered_set<string> expectedCenters({"AGACT", "CGACT", "GGACT"});
    for (const auto& c : centers) {
        ASSERT_FALSE(expectedCenters.find(c) == expectedCenters.end());
    }
}

#include <algorithm>
#include <numeric>
#include <iostream>
#include <cassert>
#include <memory>
#include <string>
#include <vector>
#include <array>
#include "../../seedselector.hpp"
#include "../../entropyseedselector.hpp"
#include "../../bpfrequencytracker.hpp"

#include "gtest/gtest.h"
using namespace std;
using barcodeSpace::SeedSelector;
using barcodeSpace::EntropySeedSelector;
using barcodeSpace::BPFrequencyTracker;

class SeedSelectorTest : public ::testing::Test {
protected:
    static const int BARCODE_LENGTH = 5;
    std::unique_ptr<SeedSelector> seedSelectorPtr;
    BPFrequencyTracker bpFrequencyTracker;
    
    SeedSelectorTest() :bpFrequencyTracker(BARCODE_LENGTH) {
    }
    
    virtual void SetUp() {
        seedSelectorPtr.reset(new EntropySeedSelector(BARCODE_LENGTH /*barcode lentgh*/, 0.33 /*entropy threshold*/));
    }
};

TEST_F(SeedSelectorTest, onePositions) {
    bpFrequencyTracker.addFrequency("AAAAA");
    bpFrequencyTracker.addFrequency("AAAAT");
    bpFrequencyTracker.addFrequency("AAAAC");
    bpFrequencyTracker.addFrequency("AAAAG");
    vector<int> seedPositions = seedSelectorPtr->getSeedsPositions(bpFrequencyTracker);
    
    ASSERT_EQ(1, seedPositions.size());
    ASSERT_EQ(4, seedPositions.front());
}

TEST_F(SeedSelectorTest, empty) {
    vector<int> seedPositions = seedSelectorPtr->getSeedsPositions(bpFrequencyTracker);
    
    ASSERT_TRUE(seedPositions.empty());
}

TEST_F(SeedSelectorTest, multiplePositions) {
    bpFrequencyTracker.addFrequency("AAAAA");
    bpFrequencyTracker.addFrequency("AAAAT");
    bpFrequencyTracker.addFrequency("AAAAC");
    bpFrequencyTracker.addFrequency("AAAGG");
    bpFrequencyTracker.addFrequency("AAAGA");
    bpFrequencyTracker.addFrequency("AAGGC");

    vector<int> seedPositions = seedSelectorPtr->getSeedsPositions(bpFrequencyTracker);
    
    ASSERT_EQ(3, seedPositions.size());
    ASSERT_EQ(3, seedPositions[0]);
    ASSERT_EQ(4, seedPositions[1]);
    ASSERT_EQ(2, seedPositions[2]);
}


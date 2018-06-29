#include <iostream>
#include <cassert>
#include <string>
#include "../../bpfrequencytracker.hpp"
#include "gtest/gtest.h"

using std::string;
using barcodeSpace::BPFrequencyTracker;

TEST(BpFrequencyTracker, marginalFrequency) {
    BPFrequencyTracker bpFrequencyTracker(5);
    bpFrequencyTracker.addFrequency("AAAAA", 5);
    std::vector<std::array<uint64_t, 4>> pwm = bpFrequencyTracker.getMarginalFrequencyTable();
    for (const auto& position : pwm) {
        ASSERT_EQ(1, position[0]);
        ASSERT_EQ(0, position[1]);
        ASSERT_EQ(0, position[2]);
        ASSERT_EQ(0, position[3]);
    }
    ASSERT_EQ(5, bpFrequencyTracker.totalFrequency());
    
    bpFrequencyTracker.addFrequency("TTTTA", 5);
    pwm = bpFrequencyTracker.getMarginalFrequencyTable();
    for (size_t pos = 0; pos < 4; ++pos) {
        ASSERT_EQ(1, pwm[pos][0]);
        ASSERT_EQ(1, pwm[pos][3]);
        ASSERT_EQ(0, pwm[pos][2]);
        ASSERT_EQ(0, pwm[pos][1]);

    }
    ASSERT_EQ(2, pwm[4][0]);
    ASSERT_EQ(10, bpFrequencyTracker.totalFrequency());
}

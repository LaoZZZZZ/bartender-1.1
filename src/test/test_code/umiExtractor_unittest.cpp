#include <iostream>
#include <cassert>
#include <vector>
#include "../../UmiExtractor.hpp"
#include "../../sequence.h"
#include "gtest/gtest.h"
using barcodeSpace::UmiExtractor;
using barcodeSpace::UmiConfig;
using barcodeSpace::Sequence;
using std::vector;

TEST(extractUmi, success) {
    UmiConfig umiconfig(0, 2);
    vector<UmiConfig> umiConfigs;
    umiConfigs.push_back(umiconfig);

    Sequence read("read_id", "ATCAGTA", "helloll");
    UmiExtractor umiExtractor(umiConfigs);
    const string extractedUmi = umiExtractor.extractUmi(read);
    ASSERT_EQ("AT", extractedUmi);
}

TEST(extractMultipleUmi, success) {
    UmiConfig umiConfig(0, 2);
    UmiConfig umiConfig2(4, 2);
    vector<UmiConfig> umiConfigs;
    umiConfigs.push_back(umiConfig);
    umiConfigs.push_back(umiConfig2);

    Sequence read("read_id", "ATCAGTA", "helloll");
    UmiExtractor umiExtractor(umiConfigs);
    const string extractedUmi = umiExtractor.extractUmi(read);
    ASSERT_EQ("ATGT", extractedUmi);
}



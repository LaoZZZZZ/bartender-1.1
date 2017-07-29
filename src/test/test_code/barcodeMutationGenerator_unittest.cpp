#include "../../barcodemutationgenerator.h"
#include "../../util.h"
#include "gtest/gtest.h"
#include <string>
#include <vector>
#include <stdexcept>
#include <unordered_set>
using std::string;
using std::vector;
using std::unordered_set;
using barcodeSpace::hammingDist;
using barcodeSpace::BarcodeMutationGenerator;

TEST(BarcodeMutationGenerator, mutateCenterInplace) {
    BarcodeMutationGenerator barcodeMutationGenerator(5);
    
    const string center = "AACAT";
    std::vector<string> mutated;
    barcodeMutationGenerator.mutateCenterInplace(center, mutated);
    ASSERT_EQ(3 * 5, mutated.size());
    ASSERT_EQ(3 * 5, unordered_set<string>(mutated.begin(), mutated.end()).size());
    // The distance should be 1
    for(const auto& mutation : mutated) {
        ASSERT_EQ(1, hammingDist(mutation, center));
    }
    
    const string emptyCenter = "";
    mutated.clear();
    barcodeMutationGenerator.mutateCenterInplace(emptyCenter, mutated);
    ASSERT_TRUE(mutated.empty());
}

TEST(BarcodeMutationGenerator, mutateCenter) {
    BarcodeMutationGenerator barcodeMutationGenerator(5);
    
    const string center = "AGCTA";
    std::vector<string> mutated = barcodeMutationGenerator.mutateCenter(center);
    ASSERT_EQ(3 * 5, mutated.size());
    ASSERT_EQ(3 * 5, unordered_set<string>(mutated.begin(), mutated.end()).size());
    
    const string emptyCenter = "";
    mutated = barcodeMutationGenerator.mutateCenter(emptyCenter);
    ASSERT_TRUE(mutated.empty());
}



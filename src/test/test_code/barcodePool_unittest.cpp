#include "../../barcodepool.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include "gtest/gtest.h"

using namespace std;
using namespace barcodeSpace;

TEST(BarcodePool, countWithDup) {
    unordered_map<string, vector<string>> raw_barcode({
        {"AAAAA", {"AAA","AAC", "AAC", "ATC", "ATC"}},
        {"AAAAT", {"AAA", "AAC"}},
        {"AACAT", {"TCA", "TCA", "TCA", "TTC"}}});
    unordered_map<string, size_t> frequencies({{"AAAAA", 5}, {"AAAAT", 2}, {"AACAT", 4}});
    BarcodePool::createInstance(raw_barcode);
    std::shared_ptr<BarcodePool> pool = BarcodePool::getAutoInstance();
    ASSERT_FALSE(pool.get() == nullptr);
    ASSERT_EQ(3, pool->size());
    for (size_t i = 0; i < pool->size(); ++i) {
        ASSERT_TRUE(frequencies.count(pool->barcode(i)) != 0);
        ASSERT_EQ(frequencies[pool->barcode(i)], pool->barcodeFrequency(i));
    }
    ASSERT_EQ(11, pool->NumOfRawBarcode());
    ASSERT_EQ(0, pool->NumOfReplicates());
}

TEST(BarcodePool, countWithoutDup) {
    unordered_map<string, vector<string>> raw_barcode({
        {"AAAAA", {"AAA","AAC","ATC",}},
        {"AAAAT", {"AAA", "AAC"}},
        {"AACAT", {"TCA", "TTC"}}
    });
    unordered_map<string, size_t> frequencies({{"AAAAA", 3}, {"AAAAT", 2}, {"AACAT", 2}});
    BarcodePool::createInstance(raw_barcode);
    std::shared_ptr<BarcodePool> pool = BarcodePool::getAutoInstance();
    ASSERT_FALSE(pool.get() == nullptr);
    ASSERT_EQ(3, pool->size());
    for (size_t i = 0; i < pool->size(); ++i) {
        ASSERT_TRUE(frequencies.count(pool->barcode(i)) != 0);
        ASSERT_EQ(frequencies[pool->barcode(i)], pool->barcodeFrequency(i));
    }
    ASSERT_EQ(7, pool->NumOfRawBarcode());
    ASSERT_EQ(0, pool->NumOfReplicates());
}

TEST(BarcodePool, countEmpty) {
    unordered_map<string, vector<string>> raw_barcode;
    BarcodePool::createInstance(raw_barcode);
    std::shared_ptr<BarcodePool> pool = BarcodePool::getAutoInstance();
    ASSERT_FALSE(pool.get() == nullptr);
    ASSERT_EQ(0, pool->size());
    ASSERT_EQ(0, pool->NumOfRawBarcode());
    ASSERT_EQ(0, pool->NumOfReplicates());
}

#include <iostream>
#include <cassert>
#include <vector>
#include "../../kmers_dictionary.h"
#include "gtest/gtest.h"
using barcodeSpace::kmersDictionary;

TEST(ComplementTest, success) {
    kmersDictionary* dict = kmersDictionary::getInstance();
    EXPECT_EQ('T', dict->complement('A'));
    EXPECT_EQ('G', dict->complement('C'));
    EXPECT_EQ('C', dict->complement('G'));
    EXPECT_EQ('A', dict->complement('T'));
}

TEST(Dna2Asc, success) {
    kmersDictionary* dict = kmersDictionary::getInstance();
    EXPECT_EQ(0, dict->asc2dna('A'));
    EXPECT_EQ(1, dict->asc2dna('C'));
    EXPECT_EQ(2, dict->asc2dna('G'));
    EXPECT_EQ(3, dict->asc2dna('T'));
}

TEST(Asc2Dna, success) {
    kmersDictionary* dict = kmersDictionary::getInstance();
    EXPECT_EQ('A', dict->dna2asc(0));
    EXPECT_EQ('C', dict->dna2asc(1));
    EXPECT_EQ('G', dict->dna2asc(2));
    EXPECT_EQ('T', dict->dna2asc(3));
}


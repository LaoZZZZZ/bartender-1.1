#include <iostream>
#include <cassert>
#include <vector>
#include "../../kmers_dictionary.h"
#include "gtest/gtest.h"
using barcodeSpace::kmersDictionary;

TEST(ComplementTest, success) {
    kmersDictionary* dict = kmersDictionary::getInstance();
    EXPECT_EQ('T', dict->complement('A'));
    EXPECT_EQ('N', dict->complement('N'));
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
    EXPECT_EQ(0, dict->asc2dna('N'));
    EXPECT_EQ(20, dict->asc2dna('U'));
}

TEST(Asc2Dna, success) {
    kmersDictionary* dict = kmersDictionary::getInstance();
    EXPECT_EQ('A', dict->dna2asc(0));
    EXPECT_EQ('C', dict->dna2asc(1));
    EXPECT_EQ('G', dict->dna2asc(2));
    EXPECT_EQ('T', dict->dna2asc(3));
}

TEST(isNucleotide, success) {
    kmersDictionary* dict = kmersDictionary::getInstance();
    EXPECT_TRUE(dict->isNucleotide('c'));
    EXPECT_TRUE(dict->isNucleotide('C'));
    EXPECT_TRUE(dict->isNucleotide('a'));
    EXPECT_TRUE(dict->isNucleotide('A'));
    EXPECT_TRUE(dict->isNucleotide('G'));
    EXPECT_TRUE(dict->isNucleotide('g'));
    EXPECT_TRUE(dict->isNucleotide('T'));
    EXPECT_TRUE(dict->isNucleotide('t'));
    EXPECT_TRUE(dict->isNucleotide('n'));
    EXPECT_TRUE(dict->isNucleotide('N'));

    EXPECT_FALSE(dict->isNucleotide('1'));
}

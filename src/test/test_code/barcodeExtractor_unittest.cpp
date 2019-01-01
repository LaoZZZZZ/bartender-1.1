#include <iostream>
#include <cassert>
#include <vector>
#include <regex>
#include <memory>
#include <string>
#include "../../barcodeextractor.h"
#include "../../sequence.h"
#include "gtest/gtest.h"

using barcodeSpace::BarcodeExtractor;
using barcodeSpace::Sequence;
using barcodeSpace::StrandDirection;
using std::regex;
using std::shared_ptr;
using std::string;

class BarcodeExtractorTest : public ::testing::Test {
protected:
    std::shared_ptr<BarcodeExtractor> barcodeextractor;

    virtual void SetUp() {
        const size_t SPLITTED_PARTS = 3;
        const string regularExpression = "(ACGT)([A|T|C|G|N]{4,7})(TGCA)";
        const string PRECEEDING = "ACGT";
        const string SUCCEEDING = "TGCA";
        const std::regex pattern(regularExpression);
        this->barcodeextractor.reset(new BarcodeExtractor(pattern, PRECEEDING, SUCCEEDING, SPLITTED_PARTS, barcodeSpace::BOTH_DIRECTION));
    }
};

TEST_F(BarcodeExtractorTest, exactMatchBothDirection) {
    // sequence =CCG + ACGT(pre) + ACCTAGC + TGCA(succ)
    // also use the sequence as quality sequence for convenience.
    Sequence read("read_id", "CCGACGTACCTAGCTGCA", "CCGACGTACCTAGCTGCA");
    ASSERT_EQ(barcodeSpace::FORWARD, barcodeextractor->ExtractBarcode(read));
    Sequence expectedExtractedRead("read_id", "ACCTAGC", "ACCTAGC");
    ASSERT_EQ(expectedExtractedRead, read);
    
    // sequence =CCG + ACGT(pre) + TGCA(succ)
    Sequence noMatch("read_id", "CCGACGTTGCA", "CCGACGTTGCA");
    ASSERT_EQ(barcodeSpace::FAIL, barcodeextractor->ExtractBarcode(noMatch));
    ASSERT_EQ(noMatch, noMatch);
    
    // reverse complement
    // sequence = TGCA + GCTAGGT + ACGT + CGG
    Sequence revComplement("read_id", "TGCAGCTAGGTACGTCGG", "TGCAGCTAGGTACGTCGG");
    ASSERT_EQ(barcodeSpace::REVERSE_COMPLEMENT, barcodeextractor->ExtractBarcode(revComplement));
    expectedExtractedRead.set("read_id", "ACCTAGC", "TGGATCG");
    ASSERT_EQ(expectedExtractedRead, revComplement);
}

// test the case that the patterns allows 1-bp mismatch in preceeding sequence.
TEST_F(BarcodeExtractorTest, inexactMatchBothDirection) {
    // reset the extractor with new regular expression.
    const string regularExpression = "(ACG.|AC.T|A.GT|.CGT)([A|T|C|G|N]{4,7})(TGCA)";
    const std::regex pattern(regularExpression);
    const string PRECEEDING = "ACGT";
    const string SUCCEEDING = "TGCA";
    const size_t SPLITTED_PARTS = 3;
    this->barcodeextractor.reset(new BarcodeExtractor(pattern, PRECEEDING, SUCCEEDING, SPLITTED_PARTS, barcodeSpace::BOTH_DIRECTION));
    Sequence read("read_id", "CCGAGGTACCTAGCTGCA", "CCGAGGTACCTAGCTGCA");
    ASSERT_EQ(barcodeSpace::FORWARD, barcodeextractor->ExtractBarcode(read));
    Sequence expectedExtractedRead("read_id", "ACCTAGC", "ACCTAGC");
    ASSERT_EQ(expectedExtractedRead, read);
    
    // sequence =CCG + ACGT(pre) + TGCA(succ)
    Sequence noMatch("read_id", "CCGAGGTTGCA", "CCGAGGTTGCA");
    ASSERT_EQ(barcodeSpace::FAIL, barcodeextractor->ExtractBarcode(noMatch));
    ASSERT_EQ(noMatch, noMatch);
    
    // reverse complement
    // sequence = TGCA + GCTAGGT + ACGT + CGG
    Sequence revComplement("read_id", "TGCAGCTAGGTACCTCGG", "TGCAGCTAGGTACGTCGG");
    ASSERT_EQ(barcodeSpace::REVERSE_COMPLEMENT, barcodeextractor->ExtractBarcode(revComplement));
    // The read sequence should be same with the reve complement but the quality will be the reverse.
    expectedExtractedRead.set("read_id", "ACCTAGC", "TGGATCG");
    ASSERT_EQ(expectedExtractedRead, revComplement);
}

TEST_F(BarcodeExtractorTest, emptyRead) {
    // sequence =CCG + ACGT(pre) + ACCTAGC + TGCA(succ)
    // also use the sequence as quality sequence for convenience.
    Sequence read("read_id", "", "");
    ASSERT_EQ(barcodeSpace::FAIL, barcodeextractor->ExtractBarcode(read));
    ASSERT_EQ(read, read);
}

TEST_F(BarcodeExtractorTest, patternWithSpacesBothDirection) {
    const string regularExpression = "(ACG.|AC.T|A.GT|.CGT)([A|T|C|G|N]{4,7})(AC)([A|T|C|G|N]{4,7})(TGCA)";
    const std::regex pattern(regularExpression);
    const string PRECEEDING = "ACGT";
    const string SUCCEEDING = "TGCA";
    const size_t SPLITTED_PARTS = 5;
    this->barcodeextractor.reset(new BarcodeExtractor(pattern, PRECEEDING, SUCCEEDING, SPLITTED_PARTS, barcodeSpace::BOTH_DIRECTION));
    // sequence = CCG + ACGT + ACCTAGC + AC + ACCT + TGCA
    Sequence read("read_id", "CCGACGTACCTAGCACACCTTGCA", "CCGAGGTACCTAGCACACCTTGCA");
    ASSERT_EQ(barcodeSpace::FORWARD, barcodeextractor->ExtractBarcode(read));
    Sequence expectedExtractedRead("read_id", "ACCTAGCACCT", "ACCTAGCACCT");
    ASSERT_EQ(expectedExtractedRead, read);
    
    // reverse complement
    // sequence = TGCA + AGGT + GT + GCTAGGT + ACGT + CGG
    Sequence revComplement("read_id", "TGCAAGGTGTGCTAGGTACGTCGG", "TGCAAGGTGTGCTAGGTACGTCGG");
    ASSERT_EQ(barcodeSpace::REVERSE_COMPLEMENT, barcodeextractor->ExtractBarcode(revComplement));
    expectedExtractedRead.set("read_id", "ACCTAGCACCT", "TGGATCGTGGA");
    ASSERT_EQ(expectedExtractedRead, revComplement);
}

TEST_F(BarcodeExtractorTest, forwardDirectionOnly) {
    // reset the extractor with new regular expression.
    const string regularExpression = "(ACG.|AC.T|A.GT|.CGT)([A|T|C|G|N]{4,7})(TGCA)";
    const std::regex pattern(regularExpression);
    const string PRECEEDING = "ACGT";
    const string SUCCEEDING = "TGCA";
    const size_t SPLITTED_PARTS = 3;
    this->barcodeextractor.reset(new BarcodeExtractor(pattern, PRECEEDING, SUCCEEDING, SPLITTED_PARTS, barcodeSpace::FORWARD_DIRECTION));
    Sequence read("read_id", "CCGAGGTACCTAGCTGCA", "CCGAGGTACCTAGCTGCA");
    ASSERT_EQ(barcodeSpace::FORWARD, barcodeextractor->ExtractBarcode(read));
    Sequence expectedExtractedRead("read_id", "ACCTAGC", "ACCTAGC");
    ASSERT_EQ(expectedExtractedRead, read);
 
    // reverse complement
    // sequence = TGCA + GCTAGGT + ACGT + CGG
    Sequence revComplement("read_id", "TGCAGCTAGGTACCTCGG", "TGCAGCTAGGTACGTCGG");
    ASSERT_EQ(barcodeSpace::FAIL, barcodeextractor->ExtractBarcode(revComplement));
}

TEST_F(BarcodeExtractorTest, reverseDirectionOnly) {
    // reset the extractor with new regular expression.
    const string regularExpression = "(ACG.|AC.T|A.GT|.CGT)([A|T|C|G|N]{4,7})(TGCA)";
    const std::regex pattern(regularExpression);
    const string PRECEEDING = "ACGT";
    const string SUCCEEDING = "TGCA";
    const size_t SPLITTED_PARTS = 3;
    this->barcodeextractor.reset(new BarcodeExtractor(pattern, PRECEEDING, SUCCEEDING, SPLITTED_PARTS, barcodeSpace::REVERSE_DIRECTION));
    Sequence read("read_id", "CCGAGGTACCTAGCTGCA", "CCGAGGTACCTAGCTGCA");
    ASSERT_EQ(barcodeSpace::FAIL, barcodeextractor->ExtractBarcode(read));
    
    // reverse complement
    // sequence = TGCA + GCTAGGT + ACGT + CGG
    Sequence revComplement("read_id", "TGCAGCTAGGTACCTCGG", "TGCAGCTAGGTACGTCGG");
    ASSERT_EQ(barcodeSpace::REVERSE_COMPLEMENT, barcodeextractor->ExtractBarcode(revComplement));
    Sequence expectedExtractedRead("read_id", "ACCTAGC", "TGGATCG");
    ASSERT_EQ(expectedExtractedRead, revComplement);
}

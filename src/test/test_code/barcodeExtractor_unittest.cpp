#include <iostream>
#include <cassert>
#include <vector>
#include <boost/regex.hpp>
#include <memory>
#include <string>
#include "../../barcodeextractor.h"
#include "../../sequence.h"
#include "gtest/gtest.h"
#include <iostream>
using barcodeSpace::BarcodeExtractor;
using barcodeSpace::Sequence;
using boost::regex;
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
        const boost::regex pattern(regularExpression);
        this->barcodeextractor.reset(new BarcodeExtractor(pattern, PRECEEDING, SUCCEEDING, SPLITTED_PARTS));
    }
};

TEST_F(BarcodeExtractorTest, exactMatch) {
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
TEST_F(BarcodeExtractorTest, inexactMatch) {
    const string regularExpression = "(ACG.|AC.T|A.GT|.CGT)([A|T|C|G|N]{4,7})(TGCA)";
    const boost::regex pattern(regularExpression);
    const string PRECEEDING = "ACGT";
    const string SUCCEEDING = "TGCA";
    const size_t SPLITTED_PARTS = 3;

    this->barcodeextractor.reset(new BarcodeExtractor(pattern, PRECEEDING, SUCCEEDING, SPLITTED_PARTS));
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
    expectedExtractedRead.set("read_id", "ACCTAGC", "TGGATCG");
    ASSERT_EQ(expectedExtractedRead, revComplement);
}
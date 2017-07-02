#include <iostream>
#include <cassert>
#include <vector>
#include <boost/regex.hpp>
#include <memory>
#include <string>
#include "../../barcodeextractor.h"
#include "../../sequence.h"
#include "gtest/gtest.h"

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
        const string PATTERN = "(ACGT)(A|T|C|G|N){4,7}(TGCA)";
        const string PRECEEDING = "ACGT";
        const string SUCCEEDING = "TGCA";
        const boost::regex pattern(PATTERN);
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
}
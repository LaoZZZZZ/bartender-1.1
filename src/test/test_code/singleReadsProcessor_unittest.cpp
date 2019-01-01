#include <iostream>
#include <cassert>
#include <vector>
#include <memory>
#include <string>
#include "../../SingleReadProcessorWithUmi.hpp"
#include "../../singlereadsprocessor.hpp"
#include "../../formats.h"
#include "../../UmiExtractor.hpp"
#include "../../barcodeextractor.h"
#include "gtest/gtest.h"

#include <iostream>
using barcodeSpace::SingleReadProcessorWithUmi;
using barcodeSpace::SingleReadsProcessor;
using barcodeSpace::UmiExtractor;
using barcodeSpace::BarcodeExtractor;
using barcodeSpace::UmiConfig;
using barcodeSpace::file_format;
using std::shared_ptr;
using std::string;
using std::vector;

class SingleReadsProcessorTest : public ::testing::Test {
protected:
    std::shared_ptr<SingleReadsProcessor> singleReadsProcessorPtr;

    virtual void SetUp() {
        const size_t parts = 3;
        const file_format format = barcodeSpace::FASTQ;
        double qualityThres = 0;
        // This file contains 5 reads, 4 of them has valid barcode.
        const string readsFile = "../resource/reads/singleReadsProcessorTest.fq";
        const string output = "../resource/singleReadsProcessTestOutput";
        std::regex pattern("(ACGT)([A|T|C|G|N]{4,7})(TGCA)", std::regex::ECMAScript);
        const string preceeding = "ACGT";
        const string suceeding = "TGCA";
        std::shared_ptr<BarcodeExtractor> barcodeExtractor(
                new BarcodeExtractor(pattern,preceeding, suceeding, parts));
        
        UmiConfig umiconfig(0, 3);
        vector<UmiConfig> umiConfigs;
        umiConfigs.push_back(umiconfig);
        UmiExtractor* umiExtractor = new UmiExtractor(umiConfigs);
        std::shared_ptr<UmiExtractor> UmiExtractorPtr;
        UmiExtractorPtr.reset(umiExtractor);
        SingleReadsProcessor* singleReadsProcessor = new SingleReadProcessorWithUmi(readsFile, barcodeExtractor, format, output, qualityThres, UmiExtractorPtr);
        singleReadsProcessorPtr.reset(singleReadsProcessor);
    }
};

TEST_F(SingleReadsProcessorTest, exactMatch) {
    singleReadsProcessorPtr->extract();
    ASSERT_EQ(5, singleReadsProcessorPtr->TotalReads());
    ASSERT_EQ(4, singleReadsProcessorPtr->TotalBarcodes());
    ASSERT_EQ(4, singleReadsProcessorPtr->TotalQualifiedBarcodes());
}

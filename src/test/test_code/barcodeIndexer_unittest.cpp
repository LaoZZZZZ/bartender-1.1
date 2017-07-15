#include "../../barcodeindexer.hpp"

#include "gtest/gtest.h"
#include <string>
#include <vector>
#include <stdexcept>

using std::string;
using std::vector;
using std::runtime_error;
using barcodeSpace::BarcodeIndexer;

TEST(BarcodeIndexer, success) {
    BarcodeIndexer barcodeIndexer({0,1,2});
    const string barcode = "AACAT";
    ASSERT_EQ(1, barcodeIndexer.GetIndex("AACAT"));
    ASSERT_EQ(16, barcodeIndexer.GetIndex("CAAAT"));
    ASSERT_THROW(barcodeIndexer.GetIndex("aa"), runtime_error);
    ASSERT_THROW(barcodeIndexer.GetIndex(""), runtime_error);
}

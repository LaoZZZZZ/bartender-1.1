#include "../../sequence.h"
#include "gtest/gtest.h"

#include <string>
using barcodeSpace::Sequence;
using std::string;

TEST(sequenceSubstr, success) {
    Sequence read("read_id", "ATCAGTA", "helloll");
    
    ASSERT_EQ(Sequence(), read.subRead(0, 0));
    ASSERT_EQ(Sequence(), read.subRead(-1, 0));
    ASSERT_EQ(Sequence(), read.subRead(0, read.length()));
    ASSERT_EQ(Sequence(), read.subRead(0, read.length() + 1));
    ASSERT_EQ(Sequence(), read.subRead(0, -1));
              
    ASSERT_EQ(Sequence(read.id(), string("T"), string("e")), read.subRead(1, 1));
    ASSERT_EQ(Sequence(read.id(), string("TC"), string("el")), read.subRead(1, 2));
    ASSERT_EQ(Sequence("read_id", "ATCAGT", "hellol"), read.subRead(0, read.length() - 1));
}

TEST(sequenceEqual, success) {
    Sequence read1("read_id", "ATCAGTA", "helloll");
    Sequence read2("read_id", "ATCAGTA", "helloll");

    ASSERT_TRUE(read1 == read2);
    read1.clear();
    ASSERT_FALSE(read1 == read2);
    
    read1.quality() = "hellola";
    ASSERT_FALSE(read1 == read2);
}

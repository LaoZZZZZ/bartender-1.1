#include <iostream>
#include <cassert>
#include <string>
#include "../../util.h"
#include "gtest/gtest.h"

using std::string;
using barcodeSpace::toUpper;
using barcodeSpace::reverseComplementInplace;
using barcodeSpace::reverseComplement;

TEST(toUpper, success) {
	const string lowercase("abc");
	ASSERT_EQ("ABC", toUpper(lowercase));
	ASSERT_EQ("", toUpper(""));
}

TEST(reverseComplementInPlace, success) {
    string seq("ATCG");
    reverseComplementInplace(seq);
    ASSERT_EQ("CGAT", seq);
    reverseComplementInplace(seq);
    ASSERT_EQ("ATCG", seq);
    seq = "";
    reverseComplementInplace(seq);
    ASSERT_EQ("", seq);
}

TEST(reverseComplement, success) {
    string seq("TTTTT");
    assert(reverseComplement(seq) == "AAAAA");
    seq = "CGTA";
    assert(reverseComplement(seq) == "TACG");
    seq = "CGTAC";
    assert(reverseComplement(seq) == "GTACG");
    assert(reverseComplement("") == "");
}

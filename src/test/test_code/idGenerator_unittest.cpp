#include <iostream>
#include <cassert>
#include <string>
#include "../../idgenerator.h"
#include "gtest/gtest.h"

using std::string;
using barcodeSpace::IDGenerator;

TEST(IDGenerator, empty) {
    IDGenerator idGenerator({});
    ASSERT_FALSE(idGenerator.isAvailable(0));
    ASSERT_EQ(0, idGenerator.nextID());
}

TEST(IDGenerator, nonConsecutive) {
    IDGenerator idGenerator({1,4,6,7});
    
    ASSERT_FALSE(idGenerator.isAvailable(1));
    ASSERT_FALSE(idGenerator.isAvailable(4));
    ASSERT_EQ(0, idGenerator.nextID());
    ASSERT_EQ(2, idGenerator.nextID());
    ASSERT_EQ(3, idGenerator.nextID());
    ASSERT_EQ(5, idGenerator.nextID());
    ASSERT_EQ(8, idGenerator.nextID());
}

TEST(IDGenerator, consecutive) {
    IDGenerator idGenerator({0,1,2,3});
    
    ASSERT_FALSE(idGenerator.isAvailable(1));
    ASSERT_FALSE(idGenerator.isAvailable(0));
    ASSERT_FALSE(idGenerator.isAvailable(4));
    
    ASSERT_EQ(4, idGenerator.nextID());

}

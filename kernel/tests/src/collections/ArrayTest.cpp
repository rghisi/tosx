//
// Created by ghisi on 17.09.22.
//

#include <gtest/gtest.h>
#include <cstdint>
#include "../../../avr/src/collections/Array.h"

TEST(Array, ShouldReturnTrue_WhenComparingTwoEqualArrays) {
    uint8_t dataSize = 0x04;
    uint8_t data[] = {'D', 'A', 'T', 'A'};
    uint8_t otherData[] = {'D', 'A', 'T', 'A'};

    bool equality = Array<uint8_t>::isEquals(data, otherData, dataSize);

    ASSERT_TRUE(equality);
}

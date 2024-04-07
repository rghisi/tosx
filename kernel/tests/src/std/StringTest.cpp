//
// Created by ghisi on 04.09.22.
//

#include <gtest/gtest.h>
#include "../../../avr/src/std/String.h"

TEST(String, ShouldReturnIndexOfFirstOccurence_WhenProvidedCharExistsInString) {
    auto string = "abcde";

    auto indexOfC = String::findFirst('c', string);
    ASSERT_EQ(2, indexOfC);
}

TEST(String, ShouldReturnMinusOne_WhenProvidedCharDoesNotExistInString) {
    auto string = "abcde";

    auto indexOfC = String::findFirst('y', string);
    ASSERT_EQ(-1, indexOfC);
}

TEST(String, ShouldReturnMinusOne_WhenProvidedStringIsEmpty) {
    auto string = "";

    auto indexOfC = String::findFirst('y', string);
    ASSERT_EQ(-1, indexOfC);
}

TEST(String, ShouldReturnMinusOne_WhenProvidedStringIsNullptr) {
    auto string = nullptr;

    auto indexOfC = String::findFirst('y', string);
    ASSERT_EQ(-1, indexOfC);
}

TEST(String, ShouldReturnIndexOfFirstOccurence_WhenProvidedCharExistsInStringNonConst) {
    auto string = new char[6] {'a', 'b', 'c', 'd', 'e', 0};

    auto indexOfC = String::findFirst('c', string);
    ASSERT_EQ(2, indexOfC);
}

TEST(String, ShouldReturnNumberOfOccurrences_WhenProvidedCharExistsInString) {
    auto countA = String::count('a', "abracadabra");
    ASSERT_EQ(5, countA);

    auto countE = String::count('e', "abracadabra");
    ASSERT_EQ(0, countE);
}

TEST(String, ShouldReturnIndexOfFirstOccurenceAfterStartIndex_WhenProvidedCharExistsInStringAfterStartIndex) {
    auto string = "abracadabra";
    ASSERT_EQ(0, String::find('a', string, 0));
    ASSERT_EQ(3, String::find('a', string, 1));
    ASSERT_EQ(5, String::find('a', string, 4));
    ASSERT_EQ(7, String::find('a', string, 6));
    ASSERT_EQ(10, String::find('a', string, 8));
    ASSERT_EQ(-1, String::find('a', string, 11));
}

TEST(String, ShouldReturnStringParts_WhenSeparatorExistsInString) {
    auto string = "one two three";
    auto parts = String::split(' ', string);
    ASSERT_STREQ("one", parts[0]);
    ASSERT_STREQ("two", parts[1]);
    ASSERT_STREQ("three", parts[2]);
}

//
// Created by ghisi on 17.09.22.
//

#include <gtest/gtest.h>
#include "collections/FragmentedText.h"

TEST(FragmentedText, ShouldHaveSizeZero_WhenInitialized) {
    auto text = new FragmentedText();

    ASSERT_EQ(0, text->size());
    ASSERT_STREQ("", text->fullText());
}

TEST(FragmentedText, ShouldHaveSizeNonZero_WhenAppendedWithChar) {
    auto text = new FragmentedText();
    text->append('a');
    ASSERT_EQ(1, text->size());
    text->append('b');
    ASSERT_EQ(2, text->size());
    text->append('c');
    ASSERT_EQ(3, text->size());

    ASSERT_STREQ("abc", text->fullText());
}

TEST(FragmentedText, ShouldHaveSizeNonZero_WhenAppendedWithArrayOfChar) {
    auto text = new FragmentedText();
    text->append("abc");
    ASSERT_EQ(3, text->size());
    ASSERT_STREQ("abc", text->fullText());
}

TEST(FragmentedText, ShouldReturnFragments_WhenBrokenIntoFragments) {
    auto text = new FragmentedText();
    text->append("one");
    text->brk();
    text->append("two");
    text->brk();
    text->append("three");
    text->brk();
    text->append("relatively long fullText here!!");

    ASSERT_EQ(4, text->fragments());
    ASSERT_STREQ("one", text->fragment(0));
    ASSERT_STREQ("two", text->fragment(1));
    ASSERT_STREQ("three", text->fragment(2));
    ASSERT_STREQ("relatively long fullText here!!", text->fragment(3));
    ASSERT_STREQ("onetwothreerelatively long fullText here!!", text->fullText());
}

TEST(FragmentedText, ShouldDeleteAllFragments_WhenCleared) {
    auto text = new FragmentedText();
    text->append("one");
    text->brk();
    text->append("two");
    text->brk();
    text->append("three");
    text->brk();
    text->append("relatively long fullText here!!");

    text->clear();

    ASSERT_EQ(1, text->fragments());
    ASSERT_STREQ("", text->fragment(0));
    ASSERT_EQ(NULL, text->fragment(1));
    ASSERT_EQ(NULL, text->fragment(2));
    ASSERT_EQ(NULL, text->fragment(1000));
    ASSERT_STREQ("", text->fullText());
}
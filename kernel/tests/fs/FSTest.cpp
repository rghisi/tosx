//
// Created by ghisi on 17.09.22.
//

#include <gtest/gtest.h>
#include <span>
#include "fs/FSDir.h"

TEST(FileSystem, Should__When) {
    auto fs = FSDir("/");
    fs.mkdir("bin");
    fs.mkdir("usr");

    FSDir expectedArray[2] = {
            FSDir("bin", &fs),
            FSDir("usr", &fs)
    };

    std::span<FSDir> expected(expectedArray);
    auto actual = fs.list();
    ASSERT_STREQ("/", fs.name());
    ASSERT_EQ(expected.size(), actual.size());
    ASSERT_EQ(expected[0], actual[0]);
    ASSERT_STREQ("bin", actual[0].name());
    ASSERT_EQ(expected[1], actual[1]);
    ASSERT_STREQ("usr", actual[1].name());
}

TEST(FileSystem, ShouldReturnAbsolutePath_WhenPwdIsCalled) {
    auto fs = FSDir("/");
    auto leafNode = fs.mkdir("usr").mkdir("local").mkdir("lib");

    ASSERT_STREQ("/usr/local/lib", leafNode.pwd().get());
}

TEST(FileSystem, ShouldCreateFile_WhenMkfileIsCalled) {
    auto fs = FSDir("/");
    fs.mkfile("file.txt");

    ASSERT_STREQ("/usr/local/lib", leafNode.pwd().get());
}

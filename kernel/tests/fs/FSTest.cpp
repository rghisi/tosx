//
// Created by ghisi on 17.09.22.
//

#include <gtest/gtest.h>
#include <span>
#include "fs/Dir.h"

TEST(FileSystem, Should__When) {
    auto fs = Dir("/");
    fs.mkdir("bin");
    fs.mkdir("usr");

    auto actual = fs.list();
    ASSERT_STREQ("/", fs.name());
    ASSERT_EQ(2, actual.size());
    ASSERT_STREQ("bin", actual[0]->name());
    ASSERT_STREQ("usr", actual[1]->name());
}

TEST(FileSystem, ShouldReturnAbsolutePath_WhenPwdIsCalled) {
    auto fs = new Dir("/");
    auto leafNode = fs->mkdir("usr")->mkdir("local")->mkdir("lib");

    ASSERT_STREQ("/usr/local/lib", leafNode->path().get());
}

TEST(FileSystem, ShouldCreateFile_WhenMkfileIsCalled) {
    auto fs = new Dir("/");
    auto etc_share = fs->mkdir("etc")->mkdir("share");
    auto file = etc_share->mkfile("file.txt");

    ASSERT_STREQ("/etc/share/file.txt", file->path().get());
}

TEST(FileSystem, ShouldCreateFileWithPointer_WhenMkfileIsCalled) {
    auto *ptr = new char[10];
    auto fs = new Dir("/");
    auto etc_share = fs->mkdir("etc")->mkdir("share");
    auto file = etc_share->mkfile("file.txt", ptr);

    ASSERT_STREQ("/etc/share/file.txt", file->path().get());
    ASSERT_EQ(ptr, file->ptr);

    delete[] ptr;
}

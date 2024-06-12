//
// Created by ghisi on 17.09.22.
//

#include <gtest/gtest.h>
#include <span>
#include "fs/FileSystemService.h"

TEST(FileSystemService, Should_return_root_When_finding_for_root_usage) {
    auto fsService = FileSystemService::instance();
    auto dir = fsService->getDir("/bin/xpto");
    ASSERT_EQ(nullptr, dir);
    auto root = fsService->getDir("/");
    root->mkdir("bin")->mkdir("xpto");
    dir = fsService->getDir("/bin/xpto");
    ASSERT_STREQ("xpto", dir->name());
}
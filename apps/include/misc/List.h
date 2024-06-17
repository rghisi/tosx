//
// Created by ghisi on 19.11.23.
//

#ifndef TOSX_LIST_H
#define TOSX_LIST_H

#include "cstdint"
#include "fs/FileSystemService.h"
#include "shell/CommandLine.h"

class List {
public:
    static int_fast8_t run(char* args) {
        auto commandLine = CommandLine(args);
        auto dirName = commandLine.hasParameters() ? commandLine.parameter(0) : ROOT;
        auto dir = FileSystemService::instance()->getDir(dirName);
        if (dir == nullptr) {
            printf("%.*s No such file or directory\r\n", dirName.length(), dirName.data());
            return 1;
        }
        auto nodes = dir->list();
        if (nodes.size() > 0) {
            auto i = 0;
            for (auto &app: nodes) {
                printf("%s\t", app->name());
                i++;
                if (i % 6 == 0) {
                    i = 0;
                    printf("\r\n");
                }
            }
            printf("\r\n");
        }

        return 0;
    }
private:
    static constexpr const std::string_view ROOT = std::string_view("/");
};
#endif //TOSX_LIST_H

//
// Created by ghisi on 19.11.23.
//

#ifndef TOSX_LIST_H
#define TOSX_LIST_H

#include "cstdint"
#include "fs/FileSystemService.h"

class List {
public:
    static int_fast8_t run(char* args) {
        auto bin = FileSystemService::instance()->getDir("/bin");
        auto nodes = bin->list();
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

        return 0;
    }
};
#endif //TOSX_LIST_H

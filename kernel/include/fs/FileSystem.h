//
// Created by ghisi on 11.06.24.
//

#ifndef TOSX_FILESYSTEM_H
#define TOSX_FILESYSTEM_H


#include "fs/FileSystem.h"
#include "Dir.h"

class FileSystem {
public:
    virtual void Mount(Dir *toDir) = 0;
};


#endif //TOSX_FILESYSTEM_H

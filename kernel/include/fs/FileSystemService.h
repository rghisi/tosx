//
// Created by ghisi on 19.05.24.
//

#ifndef TOSX_FILESYSTEMSERVICE_H
#define TOSX_FILESYSTEMSERVICE_H


#include "Node.h"
#include "Dir.h"

class FileSystemService {
protected:
    explicit FileSystemService();

public:
    FileSystemService(FileSystemService &other) = delete;
    void operator=(const FileSystemService &) = delete;
    static FileSystemService *instance();
    Dir *getDir(const char *path);

private:
    static FileSystemService *_instance;
    Dir root = Dir("/");
};


#endif //TOSX_FILESYSTEMSERVICE_H

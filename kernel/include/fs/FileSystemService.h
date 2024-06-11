//
// Created by ghisi on 19.05.24.
//

#ifndef TOSX_FILESYSTEMSERVICE_H
#define TOSX_FILESYSTEMSERVICE_H


#include "Node.h"
#include "Dir.h"
#include "FileSystem.h"

class FileSystemService {
protected:
    explicit FileSystemService();

public:
    enum class MountOption {
        READ_ONLY,
        READ_WRITE
    };
    FileSystemService(FileSystemService &other) = delete;
    void operator=(const FileSystemService &) = delete;
    static FileSystemService *instance();
    Dir *getDir(const char *path);
    Dir *getDir(std::string_view path);

    void Mount(FileSystem *fileSystem, Dir *intoDir, MountOption mountOption);

private:
    static FileSystemService *_instance;
    Dir root = Dir("/");
};


#endif //TOSX_FILESYSTEMSERVICE_H

//
// Created by ghisi on 19.05.24.
//

#include <ranges>
#include "fs/FileSystemService.h"

FileSystemService *FileSystemService::_instance = nullptr;

FileSystemService::FileSystemService() {

}

FileSystemService *FileSystemService::instance() {
    if (_instance == nullptr) {
        _instance = new FileSystemService();
    }

    return _instance;
}

Dir *FileSystemService::getDir(const char *path) {
    auto pathView = std::string_view(path);

    return getDir(pathView);
}

void FileSystemService::Mount(FileSystem *fileSystem, Dir *intoDir, MountOption mountOption) {
    fileSystem->Mount(intoDir);
}

Dir *FileSystemService::getDir(std::string_view path) {
    if (path.starts_with('/')) {
        path.remove_prefix(1);
        return root.getDir(path);
    }

    return nullptr;
}

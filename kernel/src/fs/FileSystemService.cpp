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
    auto pathView = std::make_unique<std::string_view>(path);
    if (pathView->starts_with('/')) {
        pathView->remove_prefix(1);
        return root.getDir(std::move(pathView));
    }

    return nullptr;
}

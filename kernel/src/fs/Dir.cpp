//
// Created by ghisi on 19.05.24.
//


#include "fs/Dir.h"

Dir::Dir(const char *name) : Node(name) {}

Dir::Dir(const char *name, Dir *parent) : Node(name, parent) {}

Dir *Dir::mkdir(const char *name) {
    auto dir = new Dir(name, this);
    children.push_back(dir);
    return dir;
}

std::span<Node*> Dir::list() {
    return children;
}

File *Dir::mkfile(const char *name) {
    auto file = new File(name, this);
    children.push_back(file);
    return file;
}

File *Dir::mkfile(const char *name, void *ptr) {
    auto file = new File(name, this, ptr);
    children.push_back(file);
    return file;
}

Dir *Dir::getDir(std::unique_ptr<std::string_view> path) {
    if (path->empty() || path->compare(std::string_view(name())) == 0) {
        return this;
    }
    auto dirName = path->substr(0, path->size());
    auto nextSlash = path->find('/');
    if (nextSlash != std::string_view::npos) {
        dirName = path->substr(0, nextSlash);
    }
    auto foundDir = std::find_if(children.begin(), children.end(), [dirName](Node *node) {
        auto nodeName = std::string_view(node->name());
        return nodeName.compare(dirName) == 0;
    });

    if (foundDir != children.end()) {
        auto nextDir = static_cast<Dir*>(*foundDir);
        path->remove_prefix(nextSlash + 1);
        return nextDir->getDir(std::move(path));
    }

    return nullptr;
}

void Dir::Link(Node *node) {
    children.push_back(node);
}

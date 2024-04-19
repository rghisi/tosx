//
// Created by ghisi on 18.04.24.
//

#ifndef TOSX_DIR_H
#define TOSX_DIR_H

#include <cstdint>
#include <span>
#include <vector>
#include "Node.h"
#include "File.h"

class Dir: public Node {
public:
    explicit Dir(const char *name);
    Dir(const char *name, Dir *parent);
    Dir *mkdir(const char *name);
    File *mkfile(const char *name);
    File *mkfile(const char *name, void *ptr);
    std::span<Node*> list();

private:
    std::vector<Node*> children = {};
};

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

#endif //TOSX_DIR_H

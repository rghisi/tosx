//
// Created by ghisi on 18.04.24.
//

#ifndef TOSX_FSDIR_H
#define TOSX_FSDIR_H

#include <ostream>
#include "FSNode.h"

class FSDir: public FSNode {
public:
    explicit FSDir(const char *name);
    FSDir(const char *name, FSDir *parent);
    FSDir mkdir(const char *name);
    std::span<FSNode> list();
private:
    std::vector<FSNode> children = {};
};

FSDir::FSDir(const char *name) : FSNode(name) {}

FSDir::FSDir(const char *name, FSDir *parent) : FSNode(name, parent) {}

FSDir FSDir::mkdir(const char *name) {
    const FSDir &x = FSDir(name, this);
    children.push_back(x);
    return x;
}

std::span<FSNode> FSDir::list() {
    return children;
}

#endif //TOSX_FSDIR_H

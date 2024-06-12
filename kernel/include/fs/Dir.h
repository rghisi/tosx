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

    Dir *findDir(std::string_view childName);

    Dir *getDir(std::string_view path);

    void Link(Node *node);

private:
    std::vector<Node*> children = {};
};

#endif //TOSX_DIR_H

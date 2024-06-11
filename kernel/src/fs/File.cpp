//
// Created by ghisi on 19.05.24.
//

#include "fs/File.h"

File::File(const char *name) : Node(name) {}

File::File(const char *name, Node *parent): Node(name, parent) {}

File::File(const char *name, Node *parent, void *ptr): Node(name, parent) {
    this->ptr = ptr;
}
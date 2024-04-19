//
// Created by ghisi on 19.04.24.
//

#ifndef TOSX_FILE_H
#define TOSX_FILE_H

#include "Node.h"

class File: public Node {
public:
    explicit File(const char *name);
    File(const char *name, Node *parent);
    File(const char *name, Node *parent, void *ptr);
    void *ptr = nullptr;
};

File::File(const char *name) : Node(name) {}

File::File(const char *name, Node *parent): Node(name, parent) {}

File::File(const char *name, Node *parent, void *ptr): Node(name, parent) {
    this->ptr = ptr;
}


#endif //TOSX_FILE_H

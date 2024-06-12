//
// Created by ghisi on 18.04.24.
//

#ifndef TOSX_NODE_H
#define TOSX_NODE_H

#include <cstdint>
#include <cstring>
#include <memory>
#include <vector>

class Node {
public:
    explicit Node(const char *name);
    Node(const char *name, Node *parent);
    const char *name();
    std::unique_ptr<char[]> path();
    bool operator==(const Node &rhs) const;
    bool operator!=(const Node &rhs) const;

protected:
    const char *nodeName;
    Node *parent = nullptr;
};

#endif //TOSX_NODE_H

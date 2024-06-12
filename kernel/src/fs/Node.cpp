//
// Created by ghisi on 19.05.24.
//

#include "fs/Node.h"

Node::Node(const char *name) {
    this->nodeName = name;
}

Node::Node(const char *name, Node *parent) {
    this->nodeName = name;
    this->parent = parent;
}

bool Node::operator==(const Node &rhs) const {
    return nodeName == rhs.nodeName &&
           parent == rhs.parent;
}

bool Node::operator!=(const Node &rhs) const {
    return !(rhs == *this);
}

const char *Node::name() {
    return nodeName;
}

std::unique_ptr<char[]> Node::path() {
    std::vector<const char*> path;
    auto node = this;
    uint_fast16_t size = 1;
    while (node->parent != nullptr) {
        path.push_back(node->nodeName);
        path.push_back("/");
        size += strlen(node->nodeName) + 1;
        node = node->parent;
    }
    std::unique_ptr<char[]> ret(new char[size]);
    uint_fast16_t index = 0;
    while (!path.empty()) {
        auto a = path.back();
        path.pop_back();
        uint_fast8_t i = 0;
        while (a[i]) {
            ret[index] = a[i];
            i++;
            index++;
        }


    }
    return ret;
}


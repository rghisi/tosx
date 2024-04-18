//
// Created by ghisi on 18.04.24.
//

#ifndef TOSX_FSNODE_H
#define TOSX_FSNODE_H


class FSNode {
public:
    explicit FSNode(const char *name);
    FSNode(const char *name, FSNode *parent);
    const char *name();
    std::unique_ptr<char[]> pwd();
    bool operator==(const FSNode &rhs) const;
    bool operator!=(const FSNode &rhs) const;

protected:
    const char *nodeName;
    FSNode *parent = nullptr;
};

FSNode::FSNode(const char *name) {
    this->nodeName = name;
}

FSNode::FSNode(const char *name, FSNode *parent) {
    this->nodeName = name;
    this->parent = parent;
}

bool FSNode::operator==(const FSNode &rhs) const {
    return nodeName == rhs.nodeName &&
           parent == rhs.parent;
}

bool FSNode::operator!=(const FSNode &rhs) const {
    return !(rhs == *this);
}

const char *FSNode::name() {
    return nodeName;
}

std::unique_ptr<char[]> FSNode::pwd() {
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


#endif //TOSX_FSNODE_H

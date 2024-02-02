#pragma once
#include "directory tree.h"

class TreeDisplayer {
    private:
        static void printNode(const DirectoryTree::Node& node);
        static void printChildren(const DirectoryTree::Node* node, int currentDeepness = 1);
    public:
        static void display(const DirectoryTree& tree);
};

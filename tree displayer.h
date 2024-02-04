#pragma once
#include "directory tree.h"

class TreeDisplayer {
    private:
        static void printNode(const DirectoryTree::Node& node);
        static void printChildren(const DirectoryTree::Node* node, int currentDeepness = 1);
    public:
        static void displayFull(const DirectoryTree& tree);
        static void displayCurrent(const DirectoryTree& tree);
        static void displayHistory(const DirectoryTree& tree);
};

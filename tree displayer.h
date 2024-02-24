#pragma once
#include "directory tree.h"

class TreeDisplayer {
    private:
        static void printPath(const fs::path& path);
        static void printFilename(const fs::path& path);
        static void printChildren(const DirectoryTree::Node* node);
    public:
        static void printCurrentPath(const DirectoryTree& tree);
        static void displayCurrent(const DirectoryTree& tree);
};

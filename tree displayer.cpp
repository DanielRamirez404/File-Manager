#include "tree displayer.h"
#include "directory tree.h"
#include <iostream>
#include <string>
#include <cstddef>

void TreeDisplayer::printPath(const fs::path& path)
{
    std::cout << path.string();
}

void TreeDisplayer::printFilename(const fs::path& path)
{
    std::cout << path.filename().string();
}

void TreeDisplayer::printChildren(const DirectoryTree::Node* node)
{
    for (auto const& entry_it : fs::directory_iterator(node->path))
    {
        std::cout << "  ";
        printFilename(entry_it);
        std::cout << '\n';
    }
}

void TreeDisplayer::printCurrentPath(const DirectoryTree& tree)
{
    printPath(tree.iterator().get()->path);
}

void TreeDisplayer::displayCurrent(const DirectoryTree& tree) 
{
    printFilename(tree.m_iterator.get()->path);
    std::cout << '\n';
    printChildren(tree.m_iterator.get());
}

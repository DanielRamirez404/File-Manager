#include "tree displayer.h"
#include "directory tree.h"
#include <iostream>
#include <string>
#include <cstddef>

void TreeDisplayer::displayFull(const DirectoryTree& tree) 
{
    printNode(*tree.m_root);
    printChildren(tree.m_root.get());
}

void TreeDisplayer::displayCurrent(const DirectoryTree& tree) 
{
    printNode(*tree.m_iterator.m_pointer);
    printChildren(tree.m_iterator.m_pointer);
}

void TreeDisplayer::printNode(const DirectoryTree::Node& node) 
{
    std::cout << node.path.filename().string() << '\n';
}

void TreeDisplayer::printChildren(const DirectoryTree::Node* node, int currentDeepness)
{
    for (DirectoryTree::Node* it { node->firstChild.get() }; it; it = it->nextSibling.get())
    {
        std::cout << std::string(static_cast<size_t>(currentDeepness * 2), ' ');
        printNode(*it);
        printChildren(it, currentDeepness + 1);
    }
}

void TreeDisplayer::displayHistory(const DirectoryTree& tree)
{
    for(auto i : tree.m_iterator.m_history.record)
    {
        printNode(*i);
    }
}

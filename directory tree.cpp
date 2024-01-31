#include "directory tree.h"
#include <filesystem>
#include <memory>

DirectoryTree::DirectoryTree(int deepness) : DirectoryTree(fs::current_path(), deepness) 
{
}

DirectoryTree::DirectoryTree(const fs::path& path, int deepness) : 
    m_root { std::make_shared<Node>( path ) }
{
    addChildren(m_root, deepness);
}

#include <iostream>

void DirectoryTree::addChildren(std::shared_ptr<Node> node, int deepness) 
{

    if (deepness <= 0)
        return;

    node->firstChild = std::make_shared<Node>();
    std::shared_ptr<Node> sibling_it { node->firstChild };

    for (auto const& entry_it : fs::directory_iterator(node->path))
    {
        sibling_it->path = entry_it.path();

        if (!entry_it.is_regular_file())
        {
            addChildren(sibling_it, deepness - 1);
        }

        sibling_it->nextSibling = std::make_shared<Node>();
        sibling_it = sibling_it->nextSibling;
    }
}

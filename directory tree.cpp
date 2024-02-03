#include "directory tree.h"
#include <filesystem>
#include <memory>

DirectoryTree::DirectoryTree(int deepness) : DirectoryTree(fs::current_path(), deepness) 
{
}

DirectoryTree::DirectoryTree(const fs::path& path, int deepness) : 
    m_root { std::make_unique<Node>( path ) }, m_iterator{ m_root.get() }
{
    addChildren(m_root.get(), deepness);
}

void DirectoryTree::addChildren(Node* node, int deepness) 
{
    if (deepness <= 0)
        return;
    
    Node* sibling_it {};

    for (auto const& entry_it : fs::directory_iterator(node->path))
    {
        std::unique_ptr<Node>& nodeToAdd { (sibling_it) ? sibling_it->nextSibling : node->firstChild };
        nodeToAdd = std::make_unique<Node>(entry_it.path());

        if (!entry_it.is_regular_file())
            addChildren(nodeToAdd.get(), deepness - 1);

        sibling_it = nodeToAdd.get();
    }
}

void DirectoryTree::iterateToSibling()
{
    m_iterator = (m_iterator->nextSibling) ? m_iterator->nextSibling.get() : m_iterator;
}

void DirectoryTree::iterateToChild()
{
    m_iterator = (m_iterator->firstChild) ? m_iterator->firstChild.get() : m_iterator;
}

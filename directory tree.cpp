#include "directory tree.h"
#include <filesystem>
#include <memory>

void DirectoryTree::History::add(Node* node)
{
    record.push_back(node);
    current = --record.end();
}

void DirectoryTree::History::deleteLast()
{
    record.pop_back();
    current = --record.end();
}

void DirectoryTree::History::reset()
{
    record.clear();
    current = --record.end();
}

DirectoryTree::DirectoryTree(int deepness) : DirectoryTree(fs::current_path(), deepness) 
{
}

DirectoryTree::DirectoryTree(const fs::path& path, int deepness) : 
    m_root { std::make_unique<Node>( path ) }, m_iterator{ m_root.get() }
{
    m_history.add(m_iterator);
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
    m_history.add(m_iterator);
}

void DirectoryTree::iterateToChild()
{
    m_iterator = (m_iterator->firstChild) ? m_iterator->firstChild.get() : m_iterator;
    m_history.add(m_iterator);
}

void DirectoryTree::iterateBack()
{
    if (m_history.current != m_history.record.begin())
    {
        --m_history.current;
        m_iterator = *m_history.current;
    }
}

void DirectoryTree::iterateForward()
{
    if (m_history.current != --m_history.record.end())
    {
        m_history.current++;
        m_iterator = *m_history.current;
    }
}

void DirectoryTree::resetIterator()
{
    if (m_iterator != m_root.get())
    {
        m_iterator = m_root.get();
        m_history.add(m_iterator);
    }
}

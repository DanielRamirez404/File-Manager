#include "directory tree.h"
#include <filesystem>
#include <memory>

void DirectoryTree::History::add(const Node* node)
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

DirectoryTree::DirectoryTree() : DirectoryTree(fs::current_path()) 
{
}

DirectoryTree::DirectoryTree(const fs::path& path) : 
    m_root { std::make_unique<Node>( path ) }, m_iterator{ m_root.get() }
{
    m_history.add(m_iterator);
    addChildren(m_root.get());
}

void DirectoryTree::addChildren(Node* node)
{
    Node* sibling_it {};

    for (auto const& entry_it : fs::directory_iterator(node->path))
    {
        std::unique_ptr<Node>& nodeToAdd { (sibling_it) ? sibling_it->nextSibling : node->firstChild };
        nodeToAdd = std::make_unique<Node>(entry_it.path());

        if (!entry_it.is_regular_file())
            addChildren(nodeToAdd.get());

        sibling_it = nodeToAdd.get();
    }
}

const DirectoryTree::Node* DirectoryTree::findChildPath(const Node* node, const fs::path& path) const
{
    for (const Node* it { node->firstChild.get() }; it; it = it->nextSibling.get())
    {
        if (it->path == path)
            return it;

        const Node* childSearch { findChildPath(it, path) };

        if (childSearch)
            return childSearch;
    }
    return nullptr;
}

void DirectoryTree::iterateToNode(const Node* node)
{
    if (m_iterator == node)
        return;
    
    m_iterator = node;

    if (m_history.current != --m_history.record.end())
    {
        m_history.record.remove_if
        (
            [&](const Node* i)
            {
                return i == *m_history.current++;
            }
        );
    }

    m_history.add(m_iterator);
}

const DirectoryTree::Node* DirectoryTree::findPath(const fs::path& path) const
{
    return (m_root->path == path) ? m_root.get() : findChildPath(m_root.get(), path);
}


void DirectoryTree::iterateToSibling()
{
    iterateToNode(m_iterator->nextSibling ? m_iterator->nextSibling.get() : m_iterator);
}

void DirectoryTree::iterateToChild()
{
    iterateToNode(m_iterator->firstChild ? m_iterator->firstChild.get() : m_iterator);
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
        ++m_history.current;
        m_iterator = *m_history.current;
    }
}

void DirectoryTree::resetIterator()
{
    iterateToNode(m_root.get());
}

void DirectoryTree::iterateToPath(const fs::path& path)
{
    iterateToNode(m_iterator->path == path ? m_iterator : findPath(path));
}

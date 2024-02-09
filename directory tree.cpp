#include "directory tree.h"
#include <filesystem>
#include <memory>
#include <algorithm>

DirectoryTree::Iterator::History::History(const Node* pointer) :
    record { pointer }, current { --record.end() }
{
}

void DirectoryTree::Iterator::History::add(const Node* node)
{
    record.push_back(node);
    current = --record.end();
}

void DirectoryTree::Iterator::History::reset()
{
    record.clear();
    current = --record.end();
}

DirectoryTree::Iterator::Iterator(const Node* pointer) :
    m_pointer { pointer }, m_history { pointer }
{
}

void DirectoryTree::Iterator::toNode(const Node* node) const
{
    if (m_pointer == node)
        return;
    
    m_pointer = node;

    while (m_history.current != --m_history.record.end())
        m_history.record.pop_back();

    m_history.add(m_pointer);
}

void DirectoryTree::Iterator::toSibling() const
{
    toNode(m_pointer->nextSibling ? m_pointer->nextSibling.get() : m_pointer);
}

void DirectoryTree::Iterator::toChild() const
{
    toNode(m_pointer->firstChild ? m_pointer->firstChild.get() : m_pointer);
}

void DirectoryTree::Iterator::toRoot() const
{
    toNode(*m_history.record.begin());
}

void DirectoryTree::Iterator::back() const
{
    if (m_history.current != m_history.record.begin())
    {
        --m_history.current;
        m_pointer = *m_history.current;
    }
}

void DirectoryTree::Iterator::forward() const
{
    if (m_history.current != --m_history.record.end())
    {
        ++m_history.current;
        m_pointer = *m_history.current;
    }
}

DirectoryTree::DirectoryTree() : DirectoryTree(fs::current_path()) 
{
}

DirectoryTree::DirectoryTree(const fs::path& path) : 
    m_root { std::make_unique<Node>( path ) }
{
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

const DirectoryTree::Node* DirectoryTree::findPath(const fs::path& path) const
{
    return (m_root->path == path) ? m_root.get() : findChildPath(m_root.get(), path);
}

void DirectoryTree::iterateToPath(const fs::path& path) const
{
    const Node* foundNode { findPath(path) };
    if (foundNode)
        m_iterator.toNode(foundNode);
}

const DirectoryTree::Iterator& DirectoryTree::iterator() const
{
    return m_iterator;
}

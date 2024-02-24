#include "directory tree.h"
#include <filesystem>
#include <memory>
#include <algorithm>

#include <iostream>

DirectoryTree::Iterator::History::History(const Node* pointer) :
    record { pointer }, current { --record.end() }
{
}

void DirectoryTree::Iterator::History::add(const Node* node)
{
    record.push_back(node);
    current = --record.end();
}

void DirectoryTree::Iterator::History::reset(const Node* currentNode)
{
    record.clear();
    add(currentNode);
}

DirectoryTree::Iterator::Iterator(const Node* pointer, DirectoryTree* currentTree) :
    m_pointer { pointer }, m_history { pointer }, m_this_tree { currentTree }
{
}

const DirectoryTree::Node* DirectoryTree::Iterator::get() const
{
    return m_pointer;
}

void DirectoryTree::Iterator::toPath(const fs::path& path) const
{
    const Node* pathNode { m_this_tree->findPath(path) };
    if (!pathNode)
    {
        m_this_tree->m_root = std::make_unique<Node>(path);
        m_this_tree->addChildren(m_this_tree->m_root.get());
        pathNode = m_this_tree->m_root.get();
    }
    toNode(pathNode);
}

void DirectoryTree::Iterator::toNode(const Node* node) const
{
    if (m_pointer == node)
        return;
    
    m_pointer = node;

    while (m_history.current != --m_history.record.end())
        m_history.record.pop_back();

    if (!node->firstChild.get())
        m_this_tree->addChildren(const_cast<Node*>(node));

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

void DirectoryTree::Iterator::toParent() const
{
    if (!m_pointer->path.has_parent_path())
        return;

    auto parent = m_this_tree->findParent(m_pointer);

    if (parent)
    {
        toNode(parent);
        return;
    }

    if (m_pointer == m_this_tree->m_root.get())
    {
        while (m_history.current != --m_history.record.end())
            m_history.record.pop_back();
    
        m_history.record.pop_back();
        m_history.current = --m_history.record.end();

        m_this_tree->addRootParent();     

        toNode(m_this_tree->m_root.get());       
    }
}

void DirectoryTree::Iterator::toRoot() const
{
    toPath(m_this_tree->m_root.get()->path.root_path());
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

void DirectoryTree::addChildren(Node* parent, std::unique_ptr<Node>* existingChild)
{
    Node* sibling_it { nullptr };

    for (auto const& entry_it : fs::directory_iterator(parent->path))
    {

        std::unique_ptr<Node>& nodeToAdd { (sibling_it) ? sibling_it->nextSibling : parent->firstChild };

        nodeToAdd = std::make_unique<Node>(entry_it.path());

        if (existingChild && entry_it.path() == existingChild->get()->path)
            nodeToAdd.swap(*existingChild);

        sibling_it = nodeToAdd.get();
    }
}

void DirectoryTree::addRootParent()
{
    if (!m_root.get()->path.has_parent_path())
        return;

    std::unique_ptr<Node> foundPath { std::make_unique<Node>(m_root.get()->path.parent_path()) };

    m_root.swap(foundPath);
    addChildren(m_root.get(), &foundPath);
}

const DirectoryTree::Node* DirectoryTree::findChild(const Node* node, const fs::path& path) const
{
    for (const Node* it { node->firstChild.get() }; it; it = it->nextSibling.get())
    {
        if (it->path == path)
            return it;

        const Node* childSearch { findChild(it, path) };

        if (childSearch)
            return childSearch;
    }
    return nullptr;
}

const DirectoryTree::Node* DirectoryTree::findParent(const Node* node) const
{
    return findPath(node->path.parent_path());
}

const DirectoryTree::Node* DirectoryTree::findPath(const fs::path& path) const
{
    return (m_root->path == path) ? m_root.get() : findChild(m_root.get(), path);
}

const DirectoryTree::Iterator& DirectoryTree::iterator() const
{
    return m_iterator;
}

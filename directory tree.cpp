#include "directory tree.h"
#include <filesystem>
#include <memory>
#include <algorithm>

DirectoryTree::Iterator::History::History(const Node* pointer) :
    record { pointer }, currentNode { --record.end() }
{
}

const DirectoryTree::Node* DirectoryTree::Iterator::History::current()
{
    return *currentNode;
}

void DirectoryTree::Iterator::History::setCurrentToLast() 
{
    currentNode = --record.end();
}

void DirectoryTree::Iterator::History::add(const Node* node)
{
    record.push_back(node);
    setCurrentToLast();
}

void DirectoryTree::Iterator::History::reset()
{
    const Node* temp { current() };
    record.clear();
    add(temp);
}

void DirectoryTree::Iterator::History::deleteUntilCurrent()
{
    deleteAfterCurrent();
    record.pop_back();
    setCurrentToLast();
}

void DirectoryTree::Iterator::History::deleteAfterCurrent()
{
    while (currentNode != --record.end())
        record.pop_back();
}

DirectoryTree::Iterator::Iterator(const Node* pointer, DirectoryTree* currentTree) :
    m_history { pointer }, m_this_tree { currentTree }
{
}

const DirectoryTree::Node* DirectoryTree::Iterator::get() const
{
    return m_history.current();
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
    if (m_history.current() == node)
        return;
    
    m_history.deleteAfterCurrent();

    if (!node->firstChild.get())
        m_this_tree->addChildren(const_cast<Node*>(node));

    m_history.add(node);
}

void DirectoryTree::Iterator::toSibling() const
{
    toNode(m_history.current()->nextSibling ? m_history.current()->nextSibling.get() : m_history.current());
}

void DirectoryTree::Iterator::toChild() const
{
    toNode(m_history.current()->firstChild ? m_history.current()->firstChild.get() : m_history.current());
}

void DirectoryTree::Iterator::toParent() const
{
    if (!m_history.current()->path.has_parent_path())
        return;

    if (m_history.current() == m_this_tree->m_root.get())
    {
        const fs::path currentPath { m_history.current()->path };
        m_history.deleteUntilCurrent();
        m_this_tree->addRootParent();
        toPath(currentPath);
        toNode(m_this_tree->m_root.get());
    }
    else
    {
        toNode(m_this_tree->findParent(m_history.current()));
    }   
}

void DirectoryTree::Iterator::toRoot() const
{
    toPath(m_this_tree->m_root.get()->path.root_path());
}

void DirectoryTree::Iterator::back() const
{
    if (m_history.currentNode != m_history.record.begin())
        --m_history.currentNode;
}

void DirectoryTree::Iterator::forward() const
{
    if (m_history.currentNode != --m_history.record.end())
        ++m_history.currentNode;
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

    std::unique_ptr<Node> temp { std::make_unique<Node>(m_root.get()->path.parent_path()) };

    m_root.swap(temp);
    addChildren(m_root.get(), &temp);
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

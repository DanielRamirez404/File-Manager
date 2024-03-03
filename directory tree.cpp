#include "directory tree.h"
#include "strings.h"
#include <filesystem>
#include <memory>
#include <algorithm>

DirectoryTree::Iterator::History::History(const Node* pointer) :
    record { pointer }, currentNode { --record.end() }
{
    fs::current_path((*currentNode)->path);
}

const DirectoryTree::Node* DirectoryTree::Iterator::History::current()
{
    return *currentNode;
}

void DirectoryTree::Iterator::History::setCurrentToLast() 
{
    currentNode = --record.end();
    if (current())
        fs::current_path((*currentNode)->path);
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
    if (path.is_relative())
    {
        toPath(fs::absolute(path));
        return;
    }

    if (m_history.current() && path == m_history.current()->path)
        return;

    const Node* foundNode { m_this_tree->findPath(path) };

    (foundNode) ? toNode(foundNode) : toNonExistentNode(path, m_this_tree->m_root.get());
}

void DirectoryTree::Iterator::toNonExistentNode(const fs::path& path, const Node* iterator) const
{
    std::string rootString { String::getLowercase(m_this_tree->m_root.get()->path.string()) };
    std::string iteratorString { String::getLowercase(iterator->path.string()) };
    std::string pathString { String::getLowercase(path.string()) };

    if (String::contains(rootString, pathString))
    {
        while (m_this_tree->m_root.get()->path != path)
            m_this_tree->addRootParent();
            
        toNode(m_this_tree->m_root.get());
    }
    else if (String::contains(pathString, iteratorString))
    {
        for (const auto* sibling_it {iterator->firstChild.get()}; sibling_it; sibling_it = sibling_it->nextSibling.get())
        {
            std::string siblingString { String::getLowercase(sibling_it->path.string()) };

            if (siblingString == pathString)
            {
                toNode(sibling_it);
                return;
            }

            if (String::contains(pathString, siblingString))
            {
                if (!sibling_it->firstChild.get())
                {
                    m_this_tree->addChildren(const_cast<Node*>(sibling_it));
                }
                toNonExistentNode(pathString, sibling_it);
                return;
            }
        }
    }
    else
    {
        m_this_tree->m_root = std::make_unique<Node>(path);
        m_this_tree->addChildren(m_this_tree->m_root.get());
        toNode(m_this_tree->m_root.get());
    }
}

void DirectoryTree::Iterator::toNode(const Node* node) const
{
    if (!node || m_history.current() == node)
        return;
    
    m_history.deleteAfterCurrent();

    if (!node->firstChild.get())
        m_this_tree->addChildren(const_cast<Node*>(node));

    m_history.add(node);
}

void DirectoryTree::Iterator::toSibling() const
{
    toNode(m_history.current()->nextSibling.get());
}

void DirectoryTree::Iterator::toChild() const
{
    toNode(m_history.current()->firstChild.get());
}

void DirectoryTree::Iterator::toParent() const
{
    if (!m_history.current()->path.has_parent_path())
        return;

    if (m_history.current() == m_this_tree->m_root.get())
    {
        const fs::path current { m_history.current()->path };
        m_history.deleteUntilCurrent();
        m_this_tree->addRootParent();
        toPath(current);
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
    {
        --m_history.currentNode;
        fs::current_path((*m_history.currentNode)->path);
    }
        
}

void DirectoryTree::Iterator::forward() const
{
    if (m_history.currentNode != --m_history.record.end())
    {
        ++m_history.currentNode;
        fs::current_path((*m_history.currentNode)->path);
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
        if (!fs::is_directory(entry_it.path()))
            continue;

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

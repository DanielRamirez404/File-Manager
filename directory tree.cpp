#include "directory tree.h"
#include <filesystem>
#include <memory>

DirectoryTree::DirectoryTree(int deepness) : DirectoryTree(fs::current_path(), deepness) 
{
}

DirectoryTree::DirectoryTree(const fs::path& path, int deepness) : 
    m_root { std::make_shared<Node>( path ) }
{
    addPathChildren(m_root, deepness);
}

#include <iostream>

void DirectoryTree::addPathChildren(std::shared_ptr<Node> it, int deepness) 
{
    if (deepness <= 0)
        return;

    for (auto const& dir_entry : fs::directory_iterator(it->path))
    {
        std::cout << dir_entry.path().filename() << '\n';
    }

    addPathChildren(it, deepness - 1);
    
}

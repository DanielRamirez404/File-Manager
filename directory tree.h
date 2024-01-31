#pragma once
#include <memory>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

class DirectoryTree
{
    private:

        struct Node
        {
            fs::path path{};
            std::shared_ptr<Node> firstChild { nullptr };
            std::shared_ptr<Node> nextSibling { nullptr };
        };
        
        std::shared_ptr<Node> m_root { nullptr };

        void addPathChildren(std::shared_ptr<Node> it, int deepness);

    public:
        explicit DirectoryTree(int deepness = 1);
        explicit DirectoryTree(const fs::path& path, int deepness = 1);
        
};

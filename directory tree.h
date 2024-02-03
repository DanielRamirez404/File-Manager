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
            std::unique_ptr<Node> firstChild { nullptr };
            std::unique_ptr<Node> nextSibling { nullptr };
        };

        std::unique_ptr<Node> m_root { nullptr };

        Node* m_iterator{};

        void addChildren(Node* node, int deepness);

    public:
        DirectoryTree(const DirectoryTree&) = delete;
        void operator=(const DirectoryTree&) = delete;
        explicit DirectoryTree(int deepness = 1);
        explicit DirectoryTree(const fs::path& path, int deepness = 1);
        void iterateToSibling();
        void iterateToChild();
        friend class TreeDisplayer;
};

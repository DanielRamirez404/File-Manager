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

        void addChildren(Node* node, int deepness);

    public:
        explicit DirectoryTree(int deepness = 1);
        explicit DirectoryTree(const fs::path& path, int deepness = 1);
        friend void printNode(const Node& node);
        friend void printChildren(const Node* node, int currentDeepness);
        friend void printDirectoryTree(const DirectoryTree& tree);
};

void printNode(const DirectoryTree::Node& node);
void printChildren(const DirectoryTree::Node* node, int currentDeepness = 1);
void printDirectoryTree(const DirectoryTree& tree);
